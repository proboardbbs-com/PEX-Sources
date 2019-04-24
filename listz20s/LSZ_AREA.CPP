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
#include "lsz_area.h"
#include "lsz_appl.h"
#include "str.h"

#ifndef PB_SDK
	#include <string.h>
	#include <conio.h>
#else
	#include "pblibc.h"
	#include "pblsdk.h"
#endif

// pointer to the current application
// we use it to obtain the config record for colors
extern Application *application;

static void
DrawGroup(const char *name, short maxlen, Boolean off)
{
	char buf[255];

	if( off ) textattr(application->config()->lbox_Color);
	else textattr(application->config()->lbox_Hilite);

	strcent(buf, name, maxlen);
	buf[maxlen] = EOS;
	cprintf(buf);
}

static void
DrawArea(const Area &area, short maxlen, Boolean off)
{
	FILECFG rec = (*(application->bucket()))[area.m_index];
	char    buf[255];

	if( area.m_tagged )
	{
		if( off ) textattr(application->config()->lbox_TagColor);
		else textattr(application->config()->lbox_TagHilite);
		putch(application->config()->lbox_TagChar);
	}

	if( off ) textattr(application->config()->lbox_Color);
	else textattr(application->config()->lbox_Hilite);

	if( !area.m_tagged ) putch(' ');

	strcent(buf, rec.name, maxlen - 1);
	buf[maxlen - 1] = EOS;
	cprintf(buf);
}

/*
 ****************************************************************************
 * the FILEGROUP class (nodes of this type used in the lz_GroupList)
*/
Group::Group(short num, const GROUP_PB &group)
{
	strcpy(m_name, group.name);
	m_num = num;
}

void
Group::select(const zPoint&, short maxlen, Boolean off)
{
	DrawGroup(m_name, maxlen, off);
}

void
Group::draw(const zPoint&, short maxlen)
{
	DrawGroup(m_name, maxlen, True);
}

/*
 ****************************************************************************
 * the file Area class which only stores the index of the area in question
*/
Area::Area(short index)
{
	m_index = index;
}

void
Area::select(const zPoint&, short maxlen, Boolean off)
{
	DrawArea(*this, maxlen, off);
}

void
Area::draw(const zPoint&, short maxlen)
{
	DrawArea(*this, maxlen, True);
}
