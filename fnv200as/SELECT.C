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
#include "xkeys.h"

#if !defined( PB_SDK )
	#include <string.h>
	#include <conio.h>
#endif

/*
 * These are from the screen printing module
*/
EXTERN FMN_STRUCT *pscreen( FMN_STRUCT *from, int *pad, int group );
EXTERN void ptitle( int mode );
EXTERN void pline( FMN_STRUCT *fm, int line );
EXTERN void pmenubar( int which );
EXTERN void pmarker( FMN_STRUCT *from, FMN_STRUCT *to, int yFrom, int yTo );

/*
 * This will be set to != 0 if there are any tagged files
*/
extern int _numTagged;

/*
 * Save the last fromPtr in case we want to start the listing from it
 * also save the current pointer for the same situation.
*/
static FMN_STRUCT *__fromPtr = NULL;
static FMN_STRUCT *__curPtr  = NULL;
static        int  __curLine = Y_START + 1;

/*
 * Function prototypes. _getArea() is not local since it is called by
 * the screen printing function too. The getnode_t type is defined in
 * getnode.h, as is the getNext macro (used by the screen function too).
*/
FMN_STRUCT *_getArea( FMN_STRUCT *, int, getnode_t );
FMN_STRUCT *_getHeadTail( FMN_STRUCT *, int, getnode_t );

/*
 * Macros to return the previous or next area in the list. These provide
 * the interface for the select() function. Note that getNext is defined
 * in getnode.h since it is used by the screen printing function too. The
 * getnode_t type is also defined there. Note that we are using the
 * function versions of the dlist_* routines: __dlist_*()
*/
#define getPrev(from,grp) _getArea((from),(grp),__dlist_prev)
#define getLast(ls,grp)   _getHeadTail(__dlist_last(ls),(grp),__dlist_prev)

/*
 * This function returns the next/prev area that belongs to the group. If the
 * group is == -1, it is ignored and the area is simply the next/prev one
 * from the list. If there are no more areas, NULL is returned.
*/
	FMN_STRUCT*
_getArea( FMN_STRUCT *from, int group, getnode_t getNode )
{
	FMN_STRUCT *ptr = (FMN_STRUCT *)getNode(from);

	if( -1 == group ) return ptr;
	for( ; NULL != ptr; ptr = (FMN_STRUCT *)getNode(ptr) ){
		int i;

		if( ptr->flags & FMN_ALLGROUPS ) return ptr;
		for( i = 0; i < 4; ++i ) if( ptr->groups[i] == group )
			return ptr;
	}
	return NULL;
}

/*
 * This function returns the first/last area that belongs to a group. If
 * the group is == -1, it is ignored. If no such area, NULL is returned.
*/
	FMN_STRUCT*
_getHeadTail( FMN_STRUCT *from, int group, getnode_t getNode )
{
	int i;

	if( NULL == from ) return NULL;
	if( -1 == group || (from->flags & FMN_ALLGROUPS) ) return from;
	for( i = 0; i < 4; ++i ) if( from->groups[i] == group ) return from;
	return _getArea( from, group, getNode );
}

/*
 * The select() function handles the navigation in the list that it is
 * passed. It will return on ESCAPE, ENTER and any of the hotkeys defined
 * in the hotkey list. In all cases, the currently focused item is returned.
 *
 * If the parameter group is == -1, group designations are not checked,
 * otherwise, only areas belonging to the selected group will be showed and
 * allowed. Also, note that the hotkeys are case-sensitive! Note that the
 * 'R' and 'r' keys are reserved by this function: used for redrawing the
 * screen. The Space key is reserved too, for tagging in file mode.
 *
 * The arrow keys, Home, End, Esc, Enter and Space are processed here.
 *
 * This function handles the file area tagging. When an areas is tagged,
 * its flags are modified to prevent subsequent tagging and the color of
 * the name is changed. Then, only the current (tagged) line is updated.
*/
	int
select( dlist_t *list, FMN_STRUCT **focus,
		char *keys, int group, int mode, bool restart )
{
	int pad,                 /* number of blank lines for padding, if any */
		curLine;                                /* current line displayed */
	FMN_STRUCT *curPtr,                     /* the currently focused item */
			   *fromPtr, *toPtr;         /* start and end of current page */

	/* init the screen */
	ptitle( mode );
	pad = 0;
	if( !restart && __fromPtr ){
		fromPtr = __fromPtr;
		curPtr  = __curPtr;
		curLine = __curLine;
	}
	else{
		fromPtr = getFirst( list, group);
		curPtr = fromPtr;
		curLine = Y_START + 1;
	}
	toPtr = pscreen( fromPtr, &pad, group );

	/* is this is NULL, no entries matched the group criteria, say so */
	if( NULL == fromPtr ){
		/* the screen was cleared by the pscreen() call, so print msg */
		printf( "[1;31mThere are no entries for this selection. " );
		printf( "%s",
			"[0;36mP[1;37mress [0;34m([1;37mEnter[0;34m) [0;36mt[1;37mo "
			"[0;36mr[1;37meturn[0;36m..." );
#if !defined( PB_SDK )
		while( !kbhit() ) ; getch();
#else
		printf( "\t" );
#endif
		return cmNoEntry;
	}

	pmenubar( mode );
	pmarker( NULL, curPtr, 0, curLine );

	/* this is the loop, process all incoming keys here */
	do{
		FMN_STRUCT *tmpPtr;
		int  tmpLine;
		bool dummy;
		int cmd = XGetKey( &dummy );

		__fromPtr = fromPtr;
		__curPtr  = curPtr;
		__curLine = curLine;

		switch( cmd ){

			case KEY_RT:
				tmpPtr = getNext(toPtr, group);
				if( tmpPtr ){
					toPtr = pscreen( tmpPtr, &pad, group );
					curPtr = fromPtr = tmpPtr; curLine = Y_START + 1;
					pmarker( NULL, curPtr, 0, curLine );
				}
			break;

			case KEY_LT:
				tmpPtr = getPrev(fromPtr, group);
				if( tmpPtr ){
					int i;

					for( i = 19; NULL != tmpPtr && i; --i )
						tmpPtr = getPrev(tmpPtr, group);
					if( !tmpPtr ) tmpPtr = getFirst(list, group);
					toPtr = pscreen( tmpPtr, &pad, group );
					curPtr = fromPtr = tmpPtr; curLine = Y_START + 1;
					pmarker( NULL, curPtr, 0, curLine );
				}
			break;

			/* move the focus within the current page, with wrap-around */
			case KEY_UP:
				tmpPtr = curPtr; tmpLine = curLine;
				if( curPtr != fromPtr ){
					curPtr = getPrev(curPtr, group);
					curLine--;
				}
				else{
					curPtr = toPtr;
					curLine = Y_START + 20 - pad;
				}
				pmarker( tmpPtr, curPtr, tmpLine, curLine );
			break;

			/* tag current area (only in file area mode) */
			/* note the fall-through to the down arrow processing */
			/* this will advance the marker to the next line      */
			case ' ':
				if( SL_FILAREAS != mode ) break;        /* only for areas */
				if( curPtr->flags & FMN_TAGGED ){      /* untag if tagged */
					curPtr->flags &= ~FMN_TAGGED;
					pline( curPtr, curLine );
				}
				else{
					curPtr->flags |= FMN_TAGGED;        /* tag otherwise */
					pline( curPtr, curLine ); /* pline updates _numTagged*/
				}

			case KEY_DN:
				tmpPtr = curPtr; tmpLine = curLine;
				if( curPtr != toPtr ){
					curPtr = getNext(curPtr, group);
					curLine++;
				}
				else{
					curPtr = fromPtr;
					curLine = Y_START + 1;
				}
				pmarker( tmpPtr, curPtr, tmpLine, curLine );
			break;

			/* go directly to first or last items on the current page */
			case KEY_HOME:
				pmarker( curPtr, fromPtr, curLine, Y_START + 1 );
				curLine = Y_START + 1; curPtr = fromPtr;
			break;

			case KEY_END:
				tmpPtr = toPtr;
				pmarker( curPtr, tmpPtr, curLine, Y_START + 20 - pad );
				curLine = Y_START + 20 - pad; curPtr = tmpPtr;
			break;

			/* reserved key for redrawing the screen */
			case 'R': case 'r':
				ptitle( mode );
				pscreen( fromPtr, &pad, group );
				pmenubar( mode );
				pmarker( NULL, curPtr, 0, curLine );
			break;

			/* these are the abort and accept commands */
			case KEY_ESC: return cmQuit;

			case KEY_RET: *focus = curPtr; return cmOk;

			/* see if the key is on the hotlist and return if so */
			default:
			{
				char *sel = strchr( keys, cmd );
				*focus = curPtr;
				if( NULL != sel ) return *sel;
			}
		}
	}while( 1 );
}


/*
 * Prints text and wait for Yes or No response, returns 1 on Yes, 0 on No
 * defaults to Yes
*/
	int
YesNo( char *text )
{
	bool dummy;

	printf( "%s [0;34m([1;44;37mYes[0;34m) ([0;36mNo[0;34m)", text );
	for( ;; ){
		switch( XGetKey( &dummy ) ){
			case 'N': case 'n': return 0;
			case 'Y': case 'y':
			case KEY_RET      : return 1;
		}
	}
}
