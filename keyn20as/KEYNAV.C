/*
 PB-GNU KEYNAV ProBoard PEX Version
 Copyright (C) 1995 by Branislav L. Slantchev

 This file is part of PB-GNU KEYNAV.

 PB-GNU KEYNAV is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2.

 PB-GNU KEYNAV is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with PB-GNU KEYNAV; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

 This source code is for the Borland C++ compilers only!
*/

#include "pb_sdk.h"
#include "getopt.h"

#define VERSION      "2.00"
#define GRP_IGNORE   -1
#define EOF          -1

typedef enum { FILE_AREA, MSG_AREA } types;
typedef enum { NEXT, PREV } direction;

static bool setArea( types, direction, int );
static bool inGroup( int, byte[], bool );
static bool isAccess( word, long, long );


	int
main( int argc, char *argv[] )
{
	int       curGroup = GRP_IGNORE;
	direction dirType = NEXT;
	types     areaType = FILE_AREA;
    int       opt;

	/* check for minumin arguments */
	if( 1 == argc ){
		printf( "Usage: %s -<f|m> -d<+|-> [-g num]\t", strlwr(*argv) );
		exit();
	}

	/* process command-line options */
	while( EOF != (opt = getopt(argc, argv, "fFmMd:D:g:G:") ) ){
		switch( opt ){
			case 'f': case 'F':	areaType = FILE_AREA; break;
			case 'm': case 'M':	areaType = MSG_AREA; break;
			case 'g': case 'G':	curGroup = atoi( optarg ); break;
			case 'd': case 'D':
				dirType = ('+' == *optarg) ? NEXT : PREV;
			break;
		}
	}

	setArea( areaType, dirType, curGroup );
	return 0;
}


/*
 * Gets area. if curGroup = 0, ignores group.
*/
	bool
setArea( types areaType, direction dirType, int curGroup )
{
	int    incVal = (NEXT == dirType) ? 1 : -1;
	word   *ptrArea, *ptrGroup, totalAreas, curArea, oldArea;
	word   level;
	byte  *groups;
	dword  flags, flagsNot;
	bool   allGroups;

	if( FILE_AREA == areaType ){
		totalAreas = NumFileAreas();
		curArea = CurUser->fileArea;
		ptrArea = &CurUser->fileArea;
		ptrGroup = &CurUser->fileGroup;
	}
	else{
		totalAreas = NumMsgAreas();
		curArea = CurUser->msgArea;
		ptrArea = &CurUser->msgArea;
		ptrGroup = &CurUser->msgGroup;
	}

	oldArea = curArea;
	for( curArea += incVal; curArea != oldArea; curArea += incVal ){

		/* wrap around to beginning */
		if( curArea > totalAreas ){
			curArea = 0;
			continue;
		}
		/* wrap around to end */
		if( 0 == curArea ){
			curArea = totalAreas + 1;
			continue;
		}

		if( FILE_AREA == areaType ){
			FILEAREA fa;
			if( -1 == ReadFileArea(curArea, &fa) ) continue;
			groups = fa.groups; level = fa.level; allGroups = fa.allGroups;
			flags = fa.flags; flagsNot = fa.flagsNot;
		}
		else{
			MSGAREA ma;
			if( FALSE == ReadMsgArea(curArea, &ma) ) continue;
			groups = ma.groups;	allGroups = ma.allGroups;
			level = ma.readLevel;
			flags = ma.readFlags; flagsNot = ma.readFlagsNot;
		}

		/* check if area belongs to group (if requested) */
		if( GRP_IGNORE != curGroup ){
			if( !inGroup(curGroup, groups, allGroups) ) continue;
		}

		/* no access to area, get next */
		if( !isAccess(level, flags, flagsNot) ) continue;

		/* found area, change to it, return TRUE, update group */
		*ptrArea = curArea;
		if( GRP_IGNORE == curGroup ) *ptrGroup = groups[0];
		return TRUE;
	}

	return FALSE;
}


/*
 * Checks if curGroup belongs to groups[]
*/
   bool
inGroup( int curGroup, byte groups[], bool allGroups )
{
	int i;

	if( TRUE == allGroups ) return TRUE;
	for( i = 0; i < 4; ++i )
		if( curGroup == (int)groups[i] ) return TRUE;
	return FALSE;
}


/*
 * Checks access of CurUser to level, flags, flagsNot
*/
   bool
isAccess( word level, long flags, long flagsNot )
{
	if( FALSE == CheckAccess( level, flags ) ) return FALSE;
	if( CurUser->aFlags & flagsNot ) return FALSE;
	return TRUE;
}
