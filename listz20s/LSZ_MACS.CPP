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
#include "str.h"
#include "pblsdk.h"

#ifndef PB_SDK
	#include <string.h>
	#include <time.h>
#else
	#include "pblibc.h"
#endif

static void
MakeTimeStr(char *dest, Boolean date)
{
	time_t     timer  = time(0);
	struct tm *tm     = localtime(&timer);
	char      *format = "%H:%M:%S";

	if( date )
	{
		switch( CurUser->dateFormat )
		{
			case DATE_DDMMYY: format = "%d-%m-%y"; break;
			case DATE_YYMMDD: format = "%y-%m-%d"; break;
			case DATE_MMDDYY: // fall-through to default
			default         : format = "%m-%d-%y"; break;
		}
	}
	strftime(dest, 9, format, tm);
}

// macro formatting: @MACRO<<<|>>>|%%%@

// this uses an internal buffer to store the parsed result. a pointer
// to this buffer is then returned to the caller, with all recognized
// macros expanded already. this buffer is overwritten with each call
char*
Application::ParseLineWithMacros(char *line)
{
	char   buf[1024], *pline, *pbuf, *pmacro, *p;
	size_t len  = 0, replen;
	char   just = 0;

	buf[0] = EOS;
	if( line )
	{
		for( pbuf = buf, pline = line; EOS != *pline; )
		{
			if( '@' == *pline )
			{	// can we find the end of the token
				if( 0 != (pmacro = strtok(pline + 1, "@")) )
				{	// do the actual marco parsing now
					// pmacro points to the macro text
					len = strlen(pmacro) + 2; // get requested length
					if( 0 != (p = strpbrk(pmacro, "<>%")) )
					{	// we have justification characters
						just = *p; // save justification character
						*p = EOS; // cut off the formatting part
					}
					// now we have the macro text only, no formatting
					// len = 0 if it is to be ignored, or has length
					replen = MakeMacro(pbuf, pmacro, len, just);
					pbuf += replen;
					pline += len;
				}
				else
				{
					pline++;  // skip the current '@' char when no macro
				}
			}
			else *pbuf++ = *pline++; // not a '@', copy the character
		}
	}

	*pbuf = EOS;
	return buf;
}

// writes the header/footer info using the fileTemplate file
void
Application::WriteHeaderOrFooter(char *fileTemplate)
{
	FILE *fp = fopen(fileTemplate, "rt");
	char  buf[512];

	if( fp )
	{
		fgets(buf, sizeof(buf), fp);
		while( !feof(fp) )
		{
			fputs(ParseLineWithMacros(buf), m_fp);
			fgets(buf, sizeof(buf), fp);
		}
		fclose(fp);
	}
}

// returns the length of the replacement text
int
Application::MakeMacro(char *dest, char *src, int maxlen, char justChar)
{
	char buf[255], numbuf[50];
	int  retval = 0;

	strupr(src);
	buf[0]    = EOS;
	numbuf[0] = EOS;

	if( STREQU("AREANAME", src) ) strcpy(buf, m_CurArea.name);
	else if( STREQU("PROGID", src) ) sprintf(buf, "%s %s", s_AppName, s_Version);
	else if( STREQU("AREANUM", src) ) sprintf(numbuf, "%d", m_CurAreaNo);
	else if( STREQU("AREAFILES", src) ) sprintf(numbuf, "%lu", m_AreaFiles);
	else if( STREQU("AREASIZE", src) ) sprintf(numbuf, "%lu", m_AreaBytes);
	else if( STREQU("AREASIZEK", src) ) sprintf(numbuf, "%lu", m_AreaBytes/1024);
	else if( STREQU("AREAFDLS", src) ) sprintf(numbuf, "%lu", m_AreaFdls);
	else if( STREQU("TOTALFILES", src) ) sprintf(numbuf, "%lu", m_TotalFiles);
	else if( STREQU("TOTALSIZE", src) ) sprintf(numbuf, "%lu", m_TotalKbytes * 1024L + m_DeltaBytes);
	else if( STREQU("TOTALSIZEK", src) ) sprintf(numbuf, "%lu", m_TotalKbytes);
	else if( STREQU("TOTALFDLS", src) ) sprintf(numbuf, "%lu", m_TotalFdls);
	else if( STREQU("TOTALAREAS", src) ) sprintf(numbuf, "%d", m_TotalAreas);
	else if( STREQU("DATE", src) ) MakeTimeStr(buf, True);
	else if( STREQU("TIME", src) ) MakeTimeStr(buf, False);
	// if we have a number formatted, put the commas
	if( EOS != numbuf[0] ) strcma(buf, numbuf);

	if( maxlen && justChar )
	{	// we must do justification
		buf[maxlen] = EOS; // cut off excess in macro replacement
		if( '<' == justChar ) sprintf(dest, "%-*s", maxlen, buf);
		else if( '%' == justChar ) strcent(dest, buf, maxlen);
		else sprintf(dest, "%*s", maxlen, buf);
		dest[maxlen] = EOS; // just in case
		retval = maxlen;
	}
	else
	{	// straight copying
		retval = strlen(buf);
		strcpy(dest, buf);
	}

	return retval;
}
