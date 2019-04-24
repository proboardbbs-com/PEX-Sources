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
	#include "dirmanip.h"
#endif

/*
 * Changes the file extension of src to ext, returns dest
 * If ext is NULL, remove the existing extension, ext must have the period!
*/
	char*
chext( char *dest, const char *src, const char *ext )
{
	char *p = strrchr( strcpy( dest, src ), '.' );

	/* no period, simply append extension if needed */
	if( !p ){
		if( ext ) strcat( dest, ext );
		return dest;
	}

	/* see if we have a period in a subdirectory specification */
	if( strlen(p) > 4 || strchr(p, '\\') || strchr(p, '/') ){
		if( ext ) strcat( dest, ext );
		return dest;
	}

	/* see if we need to add, change or remove the extension */
	if( ext ) strcpy( p, ext );
	else *p = EOS;

	return dest;
}

