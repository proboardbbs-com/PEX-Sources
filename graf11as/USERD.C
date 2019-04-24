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

#include "userd.h"

static char _userd_path[MAXPATH] = "\0";

/*
 * Creates a USERDOES-compatible drop file.
 * On success, returns 0, otherwise: -1
*/
	int
userd_creat( const char *fmt, ... )
{
	va_list arg;
	int uHnd;
	char buf[BUFSIZ];

	va_start( arg, fmt );
	vsprintf( buf, fmt, arg );
	va_end( arg );

	/* line should not be longer than 22 characters */
	buf[USERDOES_LEN] = '\0';

#if defined( PB_SDK )
	sprintf( _userd_path, "%s/USERDOES.%d", SysPath, NodeNumber );
#else
	strcpy( _userd_path, "USERDOES.1" );
#endif

	if( -1 == (uHnd = creat( _userd_path, S_IWRITE )) ){
		/* errno = EACCES; */
        _userd_path[0] = '\0';
		return -1;
	}

	if( -1 == write( uHnd, buf, USERDOES_LEN ) ){
		/* errno = EACCES; */
		_userd_path[0] = '\0';
		close( uHnd );
		return -1;
	}

	close( uHnd );
	return 0;
}

/*
 * Removes the dropfile created by userd_creat
*/
	int
userd_unlink( void )
{
	if( '\0' == _userd_path[0] ) return 0;

	if( -1 == unlink( _userd_path ) ){
		/* errno = ENOENT; */
		return -1;
	}

	return 0;
}


#if defined( DEBUG_USERDOES )
#pragma argsused
#pragma warn -ofp

	void
main( argc, argv )
	 int  argc;
	char *argv[];
{
	if( -1 == userd_creat( "%s", *argv ) ){
		printf( "\nUnable to create drop file!" );
	}
	else{
		printf( "\nDrop file was created successfully!" );
		/* uncomment the following to remove the dropfile */
		/* userd_unlink(); */
	}
	getchar();

}
#endif
