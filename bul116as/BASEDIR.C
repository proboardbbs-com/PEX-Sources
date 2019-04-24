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


#if !defined( __DRIMANIP_H )
	#include "dirmanip.h"
#endif

/*
** returns '\\'-terminated directory spec of path
*/
	char*
basedir( const char *path )
{
	static char buf[81];
	char *p;

	strcpy( buf, path );

	p = strrchr( buf, '\\' );
	if( !p ) p = strrchr( buf, '/' );
	if( !p ) strcpy( buf, ".\\" );
	else *(p+1) = EOS;

	return buf;
}

