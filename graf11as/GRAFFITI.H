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

#if !defined( __GRAFFITI_H )
#define __GRAFFITI_H

#if !defined( PB_SDK )
	#include <stdio.h>
	#include <string.h>
	#include <dir.h>
	#include <time.h>
	#include <stdlib.h>
	#include <conio.h>
	#include <ctype.h>
	#define WaitKeys getCommand
	#define Input getStr
	#define INPUT_DIGITS 0
	#define INPUT_ALL    0
	void getStr( char *s, int len, int dummy );
#else
	#include <pb_sdk.h>
	#define cprintf printf
	#define putch putchar
	#define gotoxy GotoXY
	#define clreol ClrEol
	#define EXIT_FAILURE
	#define textattr SetFullColor
	#define clrscr() puts( "\f" )
	#define MAXPATH 80
	#include "colorfix.h"
	#include "userd.h"
#endif

#include "colors.h"
#include "effects.h"

#define MAXTEXT  71
#define MAXNAME  36
#define MAXLINES  9
#define DEF_TEXT "AAAAAAAACK! You brute! Knock before entering a ladies room!"
#define DEF_NAME "Screetching Witch"

/* these are magic numbers */
#define STARTX   2
#define STARTY   2

#define __PROGNAME__ "Physical Graffiti"
#define __PROGVER__  "1.01"

typedef struct{
	char   text[MAXTEXT];   /* text entry             */
	char   name[MAXNAME];   /* author name            */
	time_t date;            /* date and time of entry */
	char color;             /* color id               */
} LINE;

char getCommand( char *keylist );
void printWall( LINE wall[], int numLines );
void writeWall( LINE wall[], int numLines );
void printHeader( void );
void printFooter( const char *text );
void printColors( char *text, int clrIndex );
void colorSelection( void );

#endif
