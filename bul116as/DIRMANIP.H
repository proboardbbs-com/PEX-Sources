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


#if !defined( __DIRMANIP_H )
#define __DIRMANIP_H

/*
 ***************************************************************************
                  Drive, directory and file manipulation
 ***************************************************************************
*/
#if !defined( __PBINC_H )
	#include "pbinc.h"
#endif

#if !defined( PB_SDK )
	#if !defined( __STDIO_H )
		#include <stdio.h>
	#endif
	#if !defined( __IO_H )
		#include <io.h>
	#endif
	#if !defined( __STRING_H )
		#include <string.h>
	#endif
	#if !defined( __STAT_H )
		#include <sys/stat.h>
	#endif
	#if !defined( __TIME_H )
		#include <time.h>
	#endif
	#if !defined( __STDLIB_H )
		#include <stdlib.h>
	#endif
	#if !defined( __FCNTL_H )
		#include <fcntl.h>
	#endif
#endif

#if !defined( __ASMDEF_H )
	#include "asmdef.h"
#endif

typedef struct{
	char res[21];
	char attr;
	 int time, date;
	long size;
	char name[13];
}file_s;

typedef int (*FOREACH_FPTR)(const char *, file_s *);

/*
 * File attribute masks for the file_s structure.
*/
#define FS_NORMAL 0x00
#define FS_RDONLY 0x01
#define FS_HIDDEN 0x02
#define FS_SYSTEM 0x04
#define FS_VOLUME 0x08
#define FS_DIR    0x10
#define FS_ARCH   0x20
#define FS_ALL    0x37

/*
 * Share flags for the fsopen function's shflag parameter
*/
#if !defined( __SHARE_H )
	#if !defined( PB_SDK )
		#include <share.h>
	#else
		#define SH_COMPAT   0x0000
		#define SH_DENYRW   0x0010
		#define SH_DENYWR   0x0020
		#define SH_DENYRD   0x0030
		#define SH_DENYNONE 0x0040
		#define SH_DENYNO   SH_DENYNONE
	#endif
#endif

#if defined( __cplusplus )
	extern "C" {
#endif

int   drvready( int drive );
int   drvrwr( int drive );
int   drvalid( int drive );
int   drvget( void );
int   drvset( int drive );
int   chdrive( int drive );
int   chdir( const char *path );
int   mkdir( const char *path );
int   rmdir( const char *path );
int   mkdirs( char *path );
long  tell( int handle );
int   rmfile( const char *path );
int   cpfile( const char *dest, const char *src );
int   feach( const char *path, FOREACH_FPTR func );
char *chext( char *dest, const char *src, const char *ext );
char *tempname( void );
char *tmpname( char *path );
char *basename( char *path );
char *basedir( const char *path );
word  fgetw( FILE *fp );
dword fgetdw( FILE *fp );

#if defined( __cplusplus )
	}
#endif

#endif
