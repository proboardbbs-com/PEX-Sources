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


#pragma warn -ofp

#if !defined( __DIRMANIP_H )
	#include "dirmanip.h"
#endif

char *_genUnique( char *path );

/*
** creates a temp filename, returns name doesn't create file
*/
	char*
tempname( void )
{
	static char path[81];
	char *ptr;

	ptr = getenv( "TEMP" );
	if( !ptr ) ptr = getenv( "TMP" );
	if( !ptr ) strcpy( path, ".\\" );
	else strcat( strcpy(path, ptr), "\\" );

	return _genUnique( path );
}

/*
** creates a temp filename in specified '\\'-terminated directory.
*/
	char*
tmpname( path )
	char *path;
{
	return _genUnique( path );
}


/*
** generates a unique name, appends to path argument (must be '\\' term.)
*/
	char*
_genUnique( path )
	char *path;
{
	char *ends;
	int   retry;

	ends = &path[strlen(path)];

	for( retry = 0; retry < 200; retry++ ){
		*ends = '\0';
		sprintf( ends, "%lX", (unsigned long)time(NULL) + retry );
		if( -1 == access( path, 0 ) ){
			FILE *fp = fopen( path, "w+" );
			if( !fp ) return NULL;
			fclose( fp );
			unlink( path );
			return path;
		}
	}
	return NULL;
}
