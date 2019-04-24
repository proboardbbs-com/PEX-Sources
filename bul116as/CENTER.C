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

#pragma warn -ofp
#include "center.h"

/*
** Centers a line of text in buf using width len. buf must be >= len
** len should be <= 80
*/
	char*
center( buf, text, len )
	char *buf, *text;
	 int  len;
{
	int slen, pad;
	static char *_empty = "                                        ";

	/* setup the buffer */
	memset( buf, 0, len );
	if( len > 80 ) len = 80;

	slen = strlen( text );
	if( slen > len ){
		slen = len;
		text[slen] = '\0';
	}

	pad = (len - slen) / 2;
	sprintf( buf, "%.*s%s%.*s", pad, _empty, text, pad, _empty );
	return buf;
}


/* #define TEST_CENTER */
#if defined( TEST_CENTER )
	int
main( void )
{
	char buf[100];
	char *text = "This should be centered!";

	printf( "\nCentered at 80 chars:\n" );
	center( buf, text, 80 );
	puts( buf );
	printf( "\nCentered at 40 chars:\n" );
	center( buf, text, 40 );
	puts( buf );
	printf( "\nCentered at 20 chars:\n" );
	center( buf, text, 20 );
	puts( buf );
	printf( "\nCentered at 100 chars:\n" );
	center( buf, text, 100 );
	puts( buf );
	return 0;
}
#endif
