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
#include "gdlist.h"
#include "getnode.h"
#include "strlib.h"

#if !defined( PB_SDK )
	#include <conio.h>
#else
	#define gotoxy         GotoXY
	#define clreol         ClrEol
#endif

#define ENTER_TEXT    " [1;37mP[0;36mr[1;34me[0;34mss "\
	"([1;37mEnter[0;34m) "\
	"[1;37mt[0;36mo [1;37mc[0;36mo[1;34mn[0;34mtinue[0;36m..."

/* show 'more' or 'end' on the status line */
#define _STL_MORE 1
#define _STL_END  2
static int _statLine;

static const char *_menuDefs[] =
{
"[0;36mS[1;37melect [0;36ma [0;36mg[1;37mroup[0;36m:   "
	"[0;34m([0;36mArrows[0;34m) ([0;36mEnter[0;34m)"
	" ([0;36mEscape[0;34m) ùù"
	" ([0;36mH[1;37melp[0;34m) ([0;36mI[1;37mgnore[0;34m)[1;37m",
"[0;34m([0;36mArrows[0;34m) ([0;36mEnter[0;34m)"
	" ([0;36mEscape[0;34m) ([0;36mSpace[0;34m) ùù"
	" ([0;36mH[1;37melp[0;34m)"
	" ([0;36mG[1;37mroups[0;34m) ([0;36mS[1;37mearch[0;34m)"
	" ([0;36mE[1;37mdit[0;34m) ([0;36mT[1;37mransfer[0;34m)[1;37m",
"[0;36mS[1;37melect [0;36ms[1;37mearch [0;36mm[1;37mode[0;36m[0;36m:   "
	" [0;34m([0;36mEscape[0;34m) "
	" ([0;36mA[1;37mrea[0;34m)  ([0;36mF[1;37milename[0;34m) "
	" ([0;36mK[1;37meyword[0;34m)",
"[0;36mL[1;37mimit [0;36ms[1;37mearch [0;36ms[1;37mcope[0;36m:   "
	" [0;34m([0;36mEscape[0;34m) "
	" [0;34m([0;36mT[1;37magged[0;34m)  ([0;36mG[1;37mroup[0;34m) "
	" ([0;36mA[1;37mrea[0;34m)  ([0;36mI[1;37mgnore[0;34m)",
"[0;36mT[1;37mransfer [0;36mf[1;37miles[0;36m:   "
	" [0;34m([0;36mEscape[0;34m) "
	" [0;34m([0;36mU[1;37mpload[0;34m)  ([0;36mD[1;37mownload[0;34m)"
};

/*
 * prints program title
*/
	void
ptitle( int mode )
{
	ClearScr();
	puts( "[0;36mF[1;37mile [0;36mL[1;37mibrary [0;36mN[1;37mavigator"
		  " [0;36mv.[1;33m2[0;36m.[1;33m0               [1;37mC[0;36mo[1;34mp[0;34myright [0;36m([1;37mC[0;36m)"
		  " [1;33m1995 [1;37mS[0;36mi[1;34ml[0;34micon [1;37mC[0;36mr[1;34me[0;34mations" );
	printf( "[0;34mÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ"
			"ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ([1;33m%10.10s[0;34m)ÄÄ\n[1;37m",
			(mode == SL_FILAREAS) ? "File areas" : "File group" );
}


/*
 * print status line
*/
	void
pstatline( void )
{
	gotoxy( 1, 23 );
	printf( "[0;34mÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ"
			"ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ([1;33m%4.4s[0;34m)ÄÄ\n",
			(_STL_MORE == _statLine) ? "more" : "end." );
}

/*
 * print menubar (status line)
*/
	void
pmenubar( int which )
{
	gotoxy( 1, 24 );
	clreol();
	printf( "%s", _menuDefs[which]);
}


/*
 * displays a full screen with info from the list, returns last el. on scr
*/
	FMN_STRUCT*
pscreen( FMN_STRUCT *from, int *pad, int group )
{
	int lines = 20;
	FMN_STRUCT  *tmp = from;

	*pad = 0;
	_statLine = _STL_MORE;
	gotoxy( 1, Y_START + 1 );

	for( ; from && lines; --lines, from = getNext(from, group) ){
		puts( from->name );
		tmp = from;
	}

	if( lines ){
		*pad = lines; _statLine = _STL_END;
		for( ; lines; --lines )
			puts( "                                        "
				  "                                       " );
	}

	pstatline();
	return tmp;
}


/*
 * Updates the marker position, the cursor as at the beginning of from line
*/
	void
pmarker( FMN_STRUCT *from, FMN_STRUCT *to, int yFrom, int yTo )
{
	char buf[10];

	if( from ){
		gotoxy( 1, yFrom );
		strnecpy( buf, from->name, 9 );
		printf( buf );
	}
	gotoxy( 1, yTo );
	printf( "[1;44;33m[[1;44;37m%4d[1;44;33m][1;40;37m", to->num );
	gotoxy( 5, yTo );
}


/*
 * sets the tag colors and character
*/
	void
tagline( FMN_STRUCT *fm )
{
#if defined( PB_SDK )
	if( fm->flags & FMN_TAGGED ){
		fm->name[13] = _clrTagged;
		_numTagged++;
	}
	else{
		fm->name[13] = _clrName;
		_numTagged--;
	}
#else
	if( fm->flags & FMN_TAGGED ){
		fm->name[8] = '';
		_numTagged++;
	}
	else{
		fm->name[8] = 'ú';
		_numTagged--;
	}
#endif
}


/*
 * prints a single line (updates the color if tagged)
*/
	void
pline( FMN_STRUCT *fm, int line )
{
	tagline( fm );
	gotoxy( 1, line );
	printf( fm->name );
}

/*
 * displays text and waits for enter (on line 23)
*/
	void
waitEnter( char *text )
{
	gotoxy( 1, 24 );
	clreol();
	printf( "%s", text );
	printf( "%s", ENTER_TEXT );
#if defined( PB_SDK )
	printf( "\t" );
#else
	getch();
#endif
}
