/*
 * This file is part of Listerz v2.0 (VE)
 *
 * Copyright (C) 1995-1997 by Branislav L. Slantchev
 * A Product of Silicon Creations, Inc.
 *
 * This file is distributed under the terms and conditions of the GNU
 * General Public License. For more information, refer to the file
 * Copying.Doc which is included in the archive.
*/
#include "lsz_appl.h"
#include "lsz_area.h"
#include "proboard.h"
#include "stdmac.h"
#include "keyboard.h"
#include "kbcodes.h"
#include "progbar.h"
#include "str.h"
#include "file.h"
#include "febbs.h"
#include "pblsdk.h"

#ifndef PB_SDK
	#include <conio.h>
	#include <string.h>
	#include <dos.h>
#else
	#include "pblibc.h"
#endif

#define FBBS_OFFSET 37
#define NAME_WIDTH  80
#define NAME_NELEM  25
// temp buffer for area names shown in the processing box
static char g_DisplayNames[NAME_NELEM][NAME_WIDTH];
extern Application *application;

static void
UpdatePercent(short percent, const zRect &bounds)
{
	gotoxy(bounds.a.x, bounds.a.y);
	textattr(application->config()->total_DigitColor);
	cprintf("%*d%%", bounds.Width() - 1, percent);
}

static void
UpdateCounter(ulong n, const zRect &bounds)
{
	char src[80], dest[80];

	sprintf(src, "%lu", n);
	strcma(dest, src);
	gotoxy(bounds.a.x, bounds.a.y);
	textattr(application->config()->total_DigitColor);
	cprintf("%*s", bounds.Width(), dest);
}

void
Application::FormatLine(zFbbsLine &fbbs)
{
	char buf[1024];
	int  flags = fbbs.GetFlags();
	long bytes, downs;

	if( flags & zFbbsLine::name )
	{
		m_AreaFiles++;
		bytes = atol(fbbs.GetFileSize());
		downs = atol(&fbbs.GetFileCount()[1]);
		m_AreaBytes += bytes;
		m_AreaFdls  += downs;
		m_TotalFdls += downs;
		m_TotalKbytes += bytes / 1024;
		m_DeltaBytes += (int)(bytes % 1024);
		if( m_DeltaBytes > 1024 )
		{
			m_DeltaBytes -= 1024;
			m_TotalKbytes++;
		}
		UpdateCounter(++m_TotalFiles, m_Config.total_Files);
		UpdateCounter(m_TotalKbytes, m_Config.total_Size);

		sprintf(buf, "%-12s ", fbbs.GetFileName());
		sprintf(buf + strlen(buf), "%8s ", fbbs.GetFileDate());
		sprintf(buf + strlen(buf), "%9s ", fbbs.GetFileSize());
		sprintf(buf + strlen(buf), "%s ", fbbs.GetFileCount());
		sprintf(buf + strlen(buf), "%s", fbbs.GetFileDesc());
	}
	else
	{
		char temp[1024];

		// we only fix (if needed) multi-line descriptions
		strcpy(buf, fbbs.GetFileDesc());
		// special case for programs that use the backspace to start
		// a line, AMU is one of those programs (for the headers)
		if( 0 == strchr(buf, '\b') )
		{
			char   *p;
			size_t  len;

			strcpy(temp, buf);
			memset(buf, ' ', sizeof(buf));
			for( p = temp; strchr(";+! \t", *p); ++p )
			{
				if( EOS == *p ) goto WriteLine;
			}
			len = strlen(p) + 1;
			memcpy(buf + FBBS_OFFSET, p, len);
			buf[FBBS_OFFSET + len] = EOS;
		}
	}
WriteLine:
	fputs(buf, m_fp);
	m_ListerSize = ftell(m_fp);
	UpdateCounter(m_ListerSize, m_Config.total_ListSize);
}

void
Application::ProcessFileArea(short index)
{
	int      len, y, i = 0;
	char     buf[1024];
	FILE    *fpCur;

	// make room for the new area name
	m_CurArea = (*m_Bucket)[index];
	for( i = 1; i < m_Config.proc_Bounds.Height(); ++i )
	{
		memcpy(g_DisplayNames[i-1], g_DisplayNames[i], NAME_WIDTH);
	}
	memset(g_DisplayNames[i-1], ' ', NAME_WIDTH);
	len = min((int)strlen(m_CurArea.name), m_Config.proc_Bounds.Width());
	memcpy(g_DisplayNames[i-1], m_CurArea.name, len);
	g_DisplayNames[i-1][m_Config.proc_Bounds.Width()] = EOS;
	// display the scrolling area list
	textattr(m_Config.proc_Color);
	for( i = 0, y = m_Config.proc_Bounds.a.y; y < m_Config.proc_Bounds.b.y; )
	{
		gotoxy(m_Config.proc_Bounds.a.x, y++);
		cprintf(g_DisplayNames[i++]);
	}
	// display the current area name
	memset(buf, ' ', m_Config.area_Bounds.Width());
	len = strlen(m_CurArea.name);
	memcpy(buf, m_CurArea.name, len);
	buf[m_Config.area_Bounds.Width()] = EOS;
	if( len > m_Config.area_Bounds.Width() )
	{	// name is too long, indicate that with a trailing "..."
		len = m_Config.area_Bounds.Width();
		buf[len - 1] = '.';
		buf[len - 2] = '.';
		buf[len - 3] = '.';
	}
	textattr(m_Config.area_Color);
	gotoxy(m_Config.area_Bounds.a.x, m_Config.area_Bounds.a.y);
	cprintf(buf);
	// initialize the progress bars and the statistics
	ushort fg = (int)m_Config.area_Progbar.fgColor << 8;
	ushort bg = (int)m_Config.area_Progbar.bgColor << 8;
	fg |= m_Config.area_Progbar.fgChar & 0xff;
	bg |= m_Config.area_Progbar.bgChar & 0xff;
	zProgBar progress(m_Config.area_Progbar.bounds, fg, bg);

	progress.setTotal(file_size(m_CurArea.listpath));
	zFbbsLine fbbs((zFbbsLine::date_format_t)m_CurArea.dateFormat);
	fbbs.SetPath(m_CurArea.filepath, index + 1, Boolean(m_CurArea.copyLocal));

	m_CurAreaNo = index + 1;
	m_AreaFiles = m_AreaBytes = m_AreaFdls = 0L;

	WriteHeaderOrFooter(m_AreaHeader);

	if( 0 != (fpCur = fopen(m_CurArea.listpath, "rt")) )
	{
		long lastOffset = 0L;

		fgets(buf, sizeof(buf), fpCur);
		while( !feof(fpCur) )
		{
			progress.update(ftell(fpCur) - lastOffset);
			UpdatePercent(progress.getPerCent(), m_Config.area_Percent);
			fbbs.SetLine(buf);
			FormatLine(fbbs);
			lastOffset = ftell(fpCur);
			fgets(buf, sizeof(buf), fpCur);
		}
		fclose(fpCur);
	}

	WriteHeaderOrFooter(m_AreaFooter);
}

Boolean
Application::CompressFile(ushort command)
{
	Boolean   retval = True, aborted = False;
	ARCHIVER *parch = NULL;
	char     *extension;

	switch( command )
	{
		case cmZip: parch = &m_Config.zip; extension = ".ZIP"; break;
		case cmRar: parch = &m_Config.rar; extension = ".RAR"; break;
		case cmArj: parch = &m_Config.arj; extension = ".ARJ"; break;
		case cmLha: parch = &m_Config.lha; extension = ".LZH"; break;
		case cmZoo: parch = &m_Config.zoo; extension = ".ZOO"; break;
		case cmNoCompression:
			break;
		case cmExit:
			if( file_exist(m_fileName) ) file_remove(m_fileName);
			aborted = True;
			break;
		default:
			retval = False;
	}

	// execute the archiver command
	if( retval && !aborted )
	{
		char buf[255], fileName[MAXPATH];

		if( parch )
		{	// if we are compressing, do it now
			const char *msg = "please hold while i bundle the list...";

			memset(buf, ' ', m_Config.cbar_Bounds.Width());
			memcpy(buf, msg, strlen(msg));
			buf[m_Config.cbar_Bounds.Width()] = EOS;
			gotoxy(m_Config.cbar_Bounds.a.x, m_Config.cbar_Bounds.a.y);
			textattr(m_Config.cbar_Color);
			cprintf(buf);

			sprintf(buf, "LISTZ%03X%s", NodeNumber, extension);
			strcpy(fileName, pb_getpath(buf, PBDIR_SYS));

			sprintf(buf, "%s*!*Q*Z %s %s %s %s",
				parch->swap ? "*X" : "", parch->path, parch->options,
				fileName, m_fileName);

			#ifdef PB_SDK
				RemoteDisplay(FALSE);
				MenuFunction(MENU_SHELL, buf);
				RemoteDisplay(TRUE);
			#else
				cprintf("\n\rCommand: %s\n\r", buf);
				getch();
			#endif
		}
		else
		{	// no compression, copy path to listerz file
			strcpy(fileName, m_fileName);
		}

		// now fileName has the path to the file for download
		#ifdef PB_SDK
			// check the default protocol first
			clrscr();
			if( !CurUser->defaultProtocol ) MenuFunction(74, "");
			if( CurUser->defaultProtocol )
			{
				sprintf(buf, "/A /F=%s /I /Q /K=%c",
					fileName, CurUser->defaultProtocol);
				MenuFunction(MENU_DOWNLOAD, buf);
			}
		#endif

		// cleanup just in case, we don't want these files here
		if( file_exist(m_fileName) ) file_remove(m_fileName);
		if( file_exist(fileName) ) file_remove(fileName);
	}

	return retval;
}

// this function creates the area lists and handles the download
void
Application::MakeAreaList()
{
	zListCursor  cursor(*m_AreaList, True);
	ushort       nAreas = 0;
	keyDownEvent event;
	int          i;

	// nothing in the list, don't process
	if( 0 == m_AreaList->size() )
		return;

	m_TotalAreas = 0;
	do
	{
		if( ((Area *)cursor.get())->m_tagged ) m_TotalAreas++;
	}while( cursor++ );

	// nothing tagged for processing
	if( 0 == m_TotalAreas )
		return;

	// display processing area file
	clrscr();
	pb_showfile(m_ProcScreen, NULL, &m_Terminal);

	// initialize the area display name buffers
	for( i = 0; i < NAME_NELEM; ++i )
	{
		memset(g_DisplayNames[i], ' ', NAME_WIDTH);
		g_DisplayNames[i][m_Config.proc_Bounds.Width()] = EOS;
	}

	// initialize the progress bar for the totals
	ushort fg = (int)m_Config.total_Progbar.fgColor << 8;
	ushort bg = (int)m_Config.total_Progbar.bgColor << 8;
	fg |= m_Config.total_Progbar.fgChar & 0xff;
	bg |= m_Config.total_Progbar.bgChar & 0xff;
	zProgBar progress(m_Config.total_Progbar.bounds, fg, bg);
	progress.setTotal((ulong)m_TotalAreas);

	// init the totals and percentage area
	gotoxy(m_Config.total_Percent.a.x, m_Config.total_Percent.a.y);
	textattr(m_Config.total_DigitColor);
	cprintf("%*s%%", m_Config.total_Percent.Width() - 1, "0");

	m_TotalFiles = m_TotalKbytes = m_TotalFdls = m_ListerSize = 0L;
	m_DeltaBytes = 0;
	m_AreaFiles = m_AreaBytes = m_AreaFdls = 0L;

	// flush the keyboard
	m_Keyboard->flush();
	event.keyCode = 0x0000;
	// prepare the listerz file
	m_fp = fopen(m_fileName, "wt");
	if( m_fp )
	{
		WriteHeaderOrFooter(m_MainHeader);
		// process all tagged areas
		cursor.begin();
		do
		{
			Area *areaptr = (Area *)cursor.get();

			if( areaptr->m_tagged )
			{
				ProcessFileArea(areaptr->m_index);
				areaptr->m_tagged = False;
				progress.update(1L);
				UpdatePercent(progress.getPerCent(), m_Config.total_Percent);
				UpdateCounter(++nAreas, m_Config.total_Areas);
			}

			m_Keyboard->readEvent(event);
		}while( cursor++ && kbEsc != event.keyCode );

		WriteHeaderOrFooter(m_MainFooter);
		fclose(m_fp);

		// redisplay area screen
		clrscr();
		pb_showfile(m_MainScreen, NULL, &m_Terminal);
		m_AreaBox->draw();

		// get the format of the archiver to use
		m_ArchiveBar->setState(True);
		for( ;; )
		{
			m_Keyboard->getEvent(event);
			// see if we recognized the command, if we did, this
			// returns True (and the file has been compressed &
			// sent to the user or the thing was aborted)
			if( CompressFile(m_ArchiveBar->handle(event.keyCode)) )
				break;
		}
	}

	// redisplay screen and go back to main loop
	clrscr();
	pb_showfile(m_MainScreen, NULL, &m_Terminal);
	m_AreaBox->draw();
	m_AreaBar->draw();
}
