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

#if !defined( __USERD_H )
#define __USERD_H

#if defined( PB_SDK )
	#include <pb_sdk.h>
/*	#include "errno.h" */
#else
	#include <stdio.h>
	#include <stdarg.h>
	#include <io.h>
	#include <dir.h>
	#include <sys/stat.h>
	#include <errno.h>
	#include <string.h>
#endif

/* #include "defines.h" */

#define USERDOES_LEN 22

#if !defined( MAXPATH )
	#define MAXPATH      80
#endif

#if !defined( BUFSIZ )
	#define BUFSIZ        512
#endif

int userd_creat( const char *fmt, ... );
int userd_unlink( void );

#endif
