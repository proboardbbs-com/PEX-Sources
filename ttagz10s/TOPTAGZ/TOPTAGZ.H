/*
 * This file is part of TopTagz
 *
 * Copyright (c) 1996 by Branislav L. Slantchev
 * A Product of Silicon Creations, Inc.
 *
 * This source is distributed under the terms and conditions of the
 * GNU General Public License. A copy of the license is included with
 * this distrbiution (see the file 'Copying.Doc').
 *
 * Contact: 73023.262@compuserve.com
*/

#ifndef __TOPTAGZ_H
#define __TOPTAGZ_H
#include "pblib.h"
#include "class/dlist.h"
#include "class/scankey.h"
#include "toptag.h"

#ifndef PB_SDK
	#include <conio.h>
	#define ClrScr()          clrscr()
	#define SetColor(atrib)   textattr(atrib)
    #define GotoXY(x,y)       gotoxy(x,y)
    #define ClrEol()          clreol()
    #define printf            cprintf
    #define PutChar           putch
#else
	#define ClrScr()          printf("\f")
    #define SetColor(atrib)   SetFullColor(atrib)
    #define PutChar           putchar
#endif

enum TUserAccess	// general access flags for all modes
{
	uaCanSee   = 0x0001,		// can be seen in the listings
    uaCanList  = 0x0002,		// can be listed
    uaCanUse   = 0x0004,		// can be used
    uaNormal   = uaCanSee|uaCanList|uaCanUse,
    uaListOnly = uaCanSee|uaCanList,
    uaHidden   = 0x0000
};

struct ttConfig{
	uchar fillChar;		// fill character, defaults to 'ú'
    uchar fillColor;	// fill color
    uchar tagChar;		// tag character, defaults to arrow
    uchar tagColor;		// tag color
    uchar descColor;	// description color
    uchar numColor;		// num. downloads color
    uchar highColor;	// highlite color for the bar
    uchar normColor;	// normal color for text in bar
    uchar delimColor;	// normal color for brackets
    uchar dlsColor;		// 'dls' text color
    char  prot;			// default protocol
};

#endif