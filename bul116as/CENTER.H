/*
 BULETMAN ProBoard PEX Version
 Copyright (C) 1995 by Branislav L. Slantchev

 This file is part of the BULETMAN.

 BULETMAN is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2.

 BULETMAN is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with BULETMAN; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#if !defined( __CENTER_H )
#define __CENTER_H

#if defined( __cplusplus )
	extern "C" {
#endif

#if defined( PB_SDK )
	#include <pb_sdk.h>
#else
	#include <stdio.h>
	#include <string.h>
#endif

char *center( char *buf, char *text, int len );

#if defined( __cplusplus )
	}
#endif

#endif
