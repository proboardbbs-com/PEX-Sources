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

#ifndef __EFFECTS_H
#define __EFFECTS_H

#include "pblib.h"

#ifdef __cplusplus
	extern "C" {
#endif

#ifdef PB_SDK
	#define DARKGRAY 0x08
	#define LIGHTGRAY 0x07
	#define LIGHTWHITE 0x0F
#else
	#include <conio.h>
	#include <string.h>
	#include <dos.h>
	#define LIGHTWHITE WHITE
#endif

#define MAXCOLORS 5

extern int _delayFade;
extern int _delayFlash;
extern byte _palette[MAXCOLORS];

void flash( char *s, int x, int y );
void fade( char *s, int x, int y );

#ifdef __cplusplus
	}
#endif
#endif
