/*
 File Library Navigator (ProBoard PEX and limited DOS EXE)
 Copyright (C) 1995 by Branislav L. Slantchev

 This file is part of the FileNav.

 FileNav is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2.

 FileNav is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with FileNav; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <conio.h>
#include <ctype.h>

#define INPUT_ALL       0
#define INPUT_UPFIRST   1
#define INPUT_UPALL     2
#define INPUT_DIGITS    3
#define INPUT_PWD      64   /* OR */
#define INPUT_NOFIELD 128   /* OR */
#define EOS           '\0'

	void
Input( char *buf, int len, int mode )
{
    int   ch;
    int   i;

	if( !(mode & INPUT_NOFIELD) ){
		textbackground( BLUE );
		for( ch = 0; ch <= len; ++ch ) putch( ' ' );
		for( ; ch > 0; --ch ) putch( '\b' );
	}

	for( i = 0;; ){
		ch = getch();

		if( '\r' == ch ){ buf[i] = EOS; return; }
		if( '\b' == ch ){
			if( 0 < i ){ --i; cprintf( "\b \b" ); }
			continue;
		}
		if( len == i ) continue;

		if( (mode & 0x0f) == INPUT_UPALL ) ch = toupper(ch);
		else if( ((mode & 0x0f) == INPUT_DIGITS) && !isdigit(ch) ) continue;
		else if( (mode & 0x0f) == INPUT_UPFIRST ){
			if( !i || isspace(buf[i - 1]) ) ch = toupper(ch);
		}

		if( mode & INPUT_PWD ) putch( '*' );
		else putch( ch );
		buf[i++] = ch;
	}
}

