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

#if !defined( __COLORS_H )
#define __COLORS_H

/* maximum number of color definitions per file */
#define MAXCOLORDEF 18

/* maximum number of colors allowed per definition */
#define MAXCOLORS 50

/* the color definition structure */
typedef struct{
	char numColors;                 /* number of colors for definition    */
	char onWords;                   /* restart from 0 for each word       */
	unsigned char table[MAXCOLORS]; /* table with the color values to use */
} COLORDEF;

#endif
