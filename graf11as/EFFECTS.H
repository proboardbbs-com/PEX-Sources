/*
 GRAFFITI ProBoard PEX and Standalone EXE Versions
 Copyright (C) 1995 by Branislav L. Slantchev

 This file is part of the GRAFFITI.

 GRAFFITI is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2.

 GRAFFITI is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with GRAFFITI; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#if !defined( __EFFECTS_H )
#define __EFFECTS_H

#if defined( __cplusplus )
	extern "C" {
#endif

#if defined( PB_SDK )
	#include <pb_sdk.h>
	#define DARKGRAY 0x08
	#define LIGHTGRAY 0x07
	#define LIGHTWHITE 0x0F
	#define putch putchar
#else
	#include <conio.h>
	#include <string.h>
	#include <dos.h>
	#define printf cprintf
	#define SetFullColor textcolor
	#define GotoXY gotoxy
	#define LIGHTWHITE WHITE
	typedef unsigned char byte;
#endif

#define MAXPALETTE 5

extern int _delayFade;
extern int _delayFlash;
extern byte _palette[MAXPALETTE];

void flash( char *s, int x, int y );
void fade( char *s, int x, int y );

#if defined( __cplusplus )
	}
#endif

#endif
