/*
 * This file is part of TopTagz
 *
 * Copyright (c) 1996 by Branislav L. Slantchev
 * A Product of Silicon Creations, Inc.
 *
 * This source is distributed under the terms and conditions of the
 * GNU General Public License. A copy of the license is included with
 * this distrbiution (see the file 'Copying.Doc').
 *
 * Contact: 73023.262@compuserve.com
*/

#pragma warn -ofp
#include "effects.h"
#include "toptagz.h"

int _delayFade = 125;
int _delayFlash = 12;
byte _palette[MAXCOLORS] =
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
		SetColor( _palette[i] );
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
	SetColor( _palette[0] );
	GotoXY( x, y );
	printf( s );

	/* just skip first three characters... no big deal */
	for( i = 3; i < len; ++i ){
		int sx;

		sx = x + i;
		GotoXY( sx, y );
		SetColor( _palette[1] );
		PutChar( s[i] );
		delay( _delayFlash );
		GotoXY( sx - 1, y);
		SetColor( _palette[2] );
		PutChar( s[i - 1] );
		delay( _delayFlash );
		GotoXY( sx - 2, y );
		SetColor( _palette[3] );
		PutChar( s[i - 2] );
		delay( _delayFlash );
		GotoXY( sx - 3, y );
		SetColor( _palette[4] );
		PutChar( s[i - 3]);
		delay( _delayFlash );
	}

	/* set last three characters to old color */
	SetColor( _palette[0] );
	GotoXY( x + len - 3, y );
	printf( &s[len - 3] );
}
