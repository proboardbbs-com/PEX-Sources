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

#pragma warn -ofp
#include "effects.h"

int _delayFade = 125;
int _delayFlash = 12;
byte _palette[MAXPALETTE] =
{
	DARKGRAY | (BLACK << 4),
	LIGHTGRAY | (BLACK << 4),
	LIGHTWHITE | (BLACK << 4),
	LIGHTGRAY | (BLACK << 4),
	DARKGRAY | (BLACK << 4)
};

/*
** Fades string in and out.
*/
	void
fade( s, x, y )
	char *s;
	 int  x, y;
{
	int i;

	for( i = 0; i < 5; i++ ){
		GotoXY( x, y );
		SetFullColor( _palette[i] );
		printf( s );
		delay( _delayFade );
	}
}


/*
** Flashes through string.
*/
	void
flash( s, x, y )
	char *s;
	 int  x, y;
{
	int len, i;

	len = strlen( s );
	SetFullColor( _palette[0] );
	GotoXY( x, y );
	printf( s );

	/* just skip first three characters... no big deal */
	for( i = 3; i < len; ++i ){
		int sx;

		sx = x + i;
		GotoXY( sx, y );
		SetFullColor( _palette[1] );
		putch( s[i] );
		delay( _delayFlash );
		GotoXY( sx - 1, y);
		SetFullColor( _palette[2] );
		putch( s[i - 1] );
		delay( _delayFlash );
		GotoXY( sx - 2, y );
		SetFullColor( _palette[3] );
		putch( s[i - 2] );
		delay( _delayFlash );
		GotoXY( sx - 3, y );
		SetFullColor( _palette[4] );
		putch( s[i - 3]);
		delay( _delayFlash );
	}

	/* set last three characters to old color */
	SetFullColor( _palette[0] );
	GotoXY( x + len - 3, y );
	printf( &s[len - 3] );
}
