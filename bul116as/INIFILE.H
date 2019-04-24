/*
 BULETMAN ProBoard PEX Version
 Copyright (C) 1995 by Branislav L. Slantchev

 This file is part of the PB-GNU Bulletin Manager.

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


#if !defined( __INIFILE_H )
#define __INIFILE_H

/*
 ***************************************************************************
       INI file manipulation and offline versions of Get/SetIniVar
 ***************************************************************************
*/

#if !defined( __PBINC_H )
	#include "pbinc.h"
#endif

#if !defined( PB_SDK )
	#if !defined( __STDIO_H )
		#include <stdio.h>
	#endif
	#if !defined( __STRING_H )
		#include <string.h>
	#endif
	#if !defined( __CTYPE_H )
		#include <ctype.h>
	#endif
	EXTERN bool GetIniVar( char *fname, char *varname, char *value, int max );
	EXTERN bool SetIniVar( char *fname, char *varname, char *value );
#endif

#define MAX_INI_LINE 255

#if defined( __cplusplus )
	extern "C" {
#endif

int IniWrite( const char *fname, const char *secName,
			  const char *varName, const char *value );
int IniReadStr( const char *fname, const char *secName,
				const char *varName, char *value, const char *defaultStr );
int IniReadInt( const char *fname, const char *secName,
				const char *varName, int *value, const int defaultInt );
int IniReadLong( const char *fname, const char *secName,
				const char *varName, long *value, const long defaultLong );
int IniReadBool( const char *fname, const char *secName,
				 const char *varName, bool *value, const bool defaultBool );

#if defined( __cplusplus )
	}
#endif

#endif
