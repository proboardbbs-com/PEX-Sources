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

#include "filenav.h"

static const char *_dotStr =
"תתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתת";

/*
 * formats string to internal representation
*/
#pragma warn -aus
#pragma warn -use
	void
namefmt( char *dest, char *src, int n, word flags )
{
	char f1Clr, f2Clr = _clrFlagTwo;
	char f1Char, f2Char, *f3Str;

	if( flags & FMN_FREE ){	f1Clr = _clrFlagOne; f1Char = 'F'; }
	else{ f1Clr = _clrDotTwo; f1Char = 'ת'; }

	if( flags & FMN_NOTOPS ) f2Char = 'N';
	else if( flags & FMN_GROUP ){
		f2Clr = _clrDotTwo;
		f2Char = 'ת';
	}
	else f2Char = 'T';

	if( flags & FMN_CDROM ) f3Str = "CDR";
	else f3Str = "HDD";

#if defined( PB_SDK )
	/* magic string precision: 80 - num fmt, three dots, four dots */
	sprintf( dest, "%c[%c%4d%c]%cתתת%c%.64s%c%.*s%c%c%c%cת%c%s",
		_clrBracket, _clrDigit, n, _clrBracket,
		_clrDotOne,	_clrName, src, _clrDotTwo,
		64 - strlen(src), _dotStr, f1Clr, f1Char,
		f2Clr, f2Char, _clrFlagThree, f3Str );
#else
	sprintf( dest, "[%4d]תתת%.64s%.*s%c%cת%s",
		n, src, 64 - strlen(src), _dotStr, f1Char, f2Char, f3Str );
#endif
}
#pragma warn +aus
#pragma warn +use
