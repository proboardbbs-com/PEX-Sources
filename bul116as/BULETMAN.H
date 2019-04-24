/*
 PB-GNU BULETMAN ProBoard PEX Version
 Copyright (C) 1995 by Branislav L. Slantchev

 This file is part of the PB-GNU BULETMAN.

 PB-GNU BULETMAN is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2.

 PB-GNU BULETMAN is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with PB-GNU BULETMAN; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#if !defined( __BULETMAN_H )
#define __BULETMAN_H

#include <pb_sdk.h>
#include "effects.h"
#include "center.h"
#include "inifile.h"
#include "dirmanip.h"
#include "getopt.h"

// Maximums
#define MAXPATH   80  /* maximum length of full file path */
#define MAXFILE   9   /* maximum length of root filename */
#define MAXSTR    80  /* maximum string length for reading from INI file */
#define MAXBUL    999 /* maximum number of bulletins */

// entry flags
#define BUL_NOENTRY   0   /* no entry for bulletin number */
#define BUL_UPDATED   1   /* bulletin has been updated */
#define BUL_OLD       2   /* bulletin exists but is old */

// Commands
#define cmRedraw    -1  /* redraw screen (redisplay file) */
#define cmQuit      -2  /* quit application */
#define cmDownload  -3  /* download files */
#define cmNew       -4  /* list updated bulletins */
#define cmEnter     -5  /* [Enter] was pressed  */

// Function prototypes
void fail( const char *fmt, ... );
void setup( char *progName );
int  isUpdated( int date, int time );
void downloadBul( char *rootName );
int  prompt( void );
void autoRead( void );

#endif
