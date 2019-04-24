/*
 * This file is part of mail-x (EXE)
 *
 * Copyright (C) 1995-1997 by Branislav L. Slantchev
 * A Product of Silicon Creations, Inc.
 *
 * This file is distributed under the terms and conditions of the GNU
 * General Public License. For more information, refer to the file
 * Copying.Doc which is included in the archive.
*/
#include "mx_area.h"
#include "pbstruct.h"
#include "stdmac.h"
#include "comdef.h"
#include "str.h"
#include "pblsdk.h"

#ifndef PB_SDK
	#include <string.h>
	#include <stdio.h>
	#include <conio.h>
#else
	#include "pblibc.h"
#endif

extern char    mx_TagChar;
extern int     mx_TagColor;
extern int     mx_TagHilite;
extern int     mx_ListColor;
extern int     mx_ListHilite;

static void
drawArea(short maxlen, Boolean hilite, const Area &area)
{
	char buf[256];

	if( area.m_tagged )
	{
		textattr(hilite ? mx_TagHilite : mx_TagColor);
		putch(mx_TagChar);
		textattr(hilite ? mx_ListHilite : mx_ListColor);
	}
	else
	{
		textattr(hilite ? mx_ListHilite : mx_ListColor);
		putch(' ');
	}

	strcent(buf, area.m_name, maxlen);

	buf[maxlen] = EOS;
	cprintf(buf);
}

//////////////////////////////////////////////////////////////////////////////
Area::Area(short index, const char *name)
{
	m_index = index;
	strcpy(m_name, name);
}

void
Area::draw(const zPoint&, short maxlen)
{
	drawArea(maxlen, False, *this);
}

void
Area::select(const zPoint&, short maxlen, Boolean off)
{
	drawArea(maxlen, Boolean(!off), *this);
}
