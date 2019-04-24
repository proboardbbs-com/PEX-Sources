/*
 GRAFFITI ProBoard PEX Version
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

#if !defined( __COLORFIX_H )
#define __COLORFIX_H

/* Caution: the sole purpose of that is to make SetFullColor       */
/* textattr()-compatible. Those definitions won't work elsewhere.  */
/* This will make the SetColor() function operate improperly. Also */
/* if you're using some constats, they'll fail too.                */
#if defined( PB_SDK )
	#undef BLACK
	#undef RED
	#undef GREEN
	#undef YELLOW
	#undef BLUE
	#undef MAGENTA
	#undef CYAN
	#undef WHITE
	#undef BLINK
	#define BLACK        0x00
	#define BLUE         0x01
	#define GREEN        0x02
	#define CYAN         0x03
	#define RED          0x04
	#define MAGENTA      0x05
	#define BROWN        0x06
	#define LIGHTGRAY    0x07
	#define DARKGRAY     0x08
	#define LIGHTBLUE    0x09
	#define LIGHTGREEN   0x0A
	#define LIGHTCYAN    0x0B
	#define LIGHTRED     0x0C
	#define LIGHTMAGENTA 0x0D
	#define YELLOW       0x0E
	#define WHITE        0x0F
	#define BLINK        0x80
#endif

#endif
