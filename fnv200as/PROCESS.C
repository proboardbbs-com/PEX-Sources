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
#include "strlib.h"

#if !defined( PB_SDK )
	#include <conio.h>
#else
	#define gotoxy GotoXY
	#define clreol ClrEol
#endif

/* text strings */
#define PROC_TEXT     "[0;36mP[1;37mrocess [0;36mn[1;37mext "\
	"[0;36ma[1;37mrea[0;36m?"
#define NOTAGS_TEXT   "[0;36mS[1;37morry[0;36m, y[1;37mou "\
	"[0;36md[1;37mon[0;36m'[1;37mt [0;36mh[1;37mave [0;36ma[1;37mny "\
	"[0;36mt[1;37magged [0;36ma[1;37mreas[0;36m."
#define NOGROUP_TEXT  "[0;36mS[1;37morry[0;36m, y[1;37mou "\
	"[0;36mh[1;37maven[0;36m'[1;37mt [0;36ms[1;37melected [0;36ma "\
	"g[1;37mroup[0;36m."
#define NOSRCH_TEXT   "[1;37mS[1;31mearch [1;37ms[1;31mtring "\
	"[1;33mnot [1;37mf[1;31mound."

/* holds the data string */
static char _dataParam[2048];

/* external support functions */
EXTERN int  YesNo( char *text );
EXTERN void waitEnter( char *text );
EXTERN void pmenubar( int which );
EXTERN void tagline( FMN_STRUCT * );

#if !defined( PB_SDK )
	EXTERN void Input( char *, int, int );
#endif

/* local functions */
static FMN_STRUCT *getTagged( FMN_STRUCT * );
static void buildDataStr( dlist_t*, FMN_STRUCT *, int, int );
static void buildListData( char *, int );
static int  selectMode( int );
static void filenameSearch( dlist_t *, FMN_STRUCT *, int, int);
static void keywordSearch( dlist_t *, FMN_STRUCT *, int, int);
static void areaSearch( dlist_t*, int );

/* the only publics */
void listFiles( dlist_t*, FMN_STRUCT *);
void selectSearch( dlist_t*, FMN_STRUCT *, int );
void transferFiles( dlist_t *, FMN_STRUCT *, int );
void editTags( void );

/*
 * gets first tagged area, we know that there's at least one
*/
	static FMN_STRUCT*
getTagged( FMN_STRUCT *cur )
{
	FMN_STRUCT *nptr = cur;

	for( ; nptr; nptr = dlist_next(nptr) )
		if( nptr->flags & FMN_TAGGED ) return nptr;

	return NULL;
}

/*
 * builds the necessary data string for the search functions
 * mode is SRCH_AREA, SRCH_GROUP, SRCH_TAGGED, SRCH_ALL
*/
	void
buildDataStr( dlist_t *list, FMN_STRUCT *focus, int group, int mode )
{
	FMN_STRUCT *nptr;

	if( SRCH_AREA == mode ){
		sprintf( &_dataParam[strlen(_dataParam)], "%d", focus->num );
	}
	else if( SRCH_ALL == mode ){
		strcat( _dataParam, "*" );
	}
	else if( SRCH_GROUP == mode ){
		for( nptr = getFirst(list, group); nptr; nptr = getNext(nptr,group) ){
			char buf[10];
			sprintf( buf, "+%d ", nptr->num );
			strcat( _dataParam, buf );
		}
	}
	else /* if( SRCH_TAGGED == mode ) */{
		for( nptr = dlist_first(list); nptr; nptr = dlist_next(nptr) ){
			if( nptr->flags & FMN_TAGGED ){
				char buf[10];
				sprintf( buf, "+%d ", nptr->num );
				strcat( _dataParam, buf );
			}
		}
	}
}


/*
 * builds the necessary data string for the listing function
*/
	void
buildListData( char *buf, int num )
{
	buf[0] = EOS;
	if( listDataStr && EOS != listDataStr[0] )
		sprintf( buf, "%s ", listDataStr );
	if( !listNoParam ) sprintf( &buf[strlen(buf)], "%d", num );
}


/*
 * list files in focused area or in the tagged areas
*/
	void
listFiles( dlist_t *list, FMN_STRUCT *focus )
{
	FMN_STRUCT *nptr;
	char buf[256];

	if( _numTagged ) nptr = getTagged( dlist_first(list) );
	else nptr = focus;

	buildListData( buf, nptr->num );

#if defined( PB_SDK )
	MenuFunction( listFunc, buf );
#else
	ClearScr();
	printf( "processing fun: %d  with data '%s'", listFunc, buf );
	getch();
#endif
	if( !_numTagged ) return;

	do{
		if( NULL == (nptr = getTagged(dlist_next(nptr))) ) break;
		ClearScr();
		printf( "[0;36mN[1;37mext [0;36ma[1;37mrea[0;36m:\n%s\n", nptr->name );
		if( !YesNo( PROC_TEXT ) ) break;
		buildListData( buf, nptr->num );
#if defined( PB_SDK )
		MenuFunction( listFunc, buf );
#else
		ClearScr();
		printf( "processing fun: %d  with data '%s'", listFunc, buf );
		getch();
#endif
	}while( 1 );
}


/*
 * Does the keyword search in current area, in tagged areas or in group
 * mode is SRCH_AREA, SRCH_GROUP, SRCH_TAGGED, SRCH_ALL
*/
	void
keywordSearch( dlist_t *list, FMN_STRUCT *focus, int group, int mode )
{

	if( EOS != keywordDataStr[0] ) sprintf( _dataParam, "%s ", keywordDataStr );
	else _dataParam[0] = EOS;
	if( !keywordNoParam ) buildDataStr( list, focus, group, mode );

#if defined( PB_SDK )
	MenuFunction( keywordFunc, _dataParam );
#else
	ClearScr();
	printf( "keyword: fn %d, data: '%s'", keywordFunc, _dataParam );
	getch();
#endif
}


/*
 * does the filename search, similar to keyword
*/
	void
filenameSearch( dlist_t *list, FMN_STRUCT *focus, int group, int mode )
{
	if( EOS != nameDataStr[0] )	sprintf( _dataParam, "%s ", nameDataStr );
	else _dataParam[0] = EOS;
	if( !nameNoParam ) buildDataStr( list, focus, group, mode );

#if defined( PB_SDK )
	MenuFunction( nameFunc, _dataParam );
#else
	ClearScr();
	printf( "filename: fn %d, data: '%s'", nameFunc, _dataParam );
	getch();
#endif
}


/*
 * searches for text in the area names
*/
	void
areaSearch( dlist_t *list, int group )
{
	char text[128];
	FMN_STRUCT *nptr = getFirst( list, group );

	gotoxy( 1, 24 ); clreol();
	printf( "[0;36mE[1;37mnter [0;36ms[1;37mearch [0;36ms[1;37mtring[0;36m: " );
	Input( text, 50, INPUT_ALL | INPUT_NOFIELD );
	if( EOS == text[0] ) return;

	for( ; nptr; nptr = getNext(nptr, group) ){
		if( !stristr(nptr->name, text ) ) continue;
		gotoxy( 1, 24 ); clreol();
#if defined( PB_SDK )
		printf( "[1;37m%-30.30súú[0;34m([1;37mEscape[0;34m) "
			"([1;37mEnter[0;34m) [0;36mfor next [0;34m"
			"([1;37mSpace[0;34m) [0;36mto toggle tag[1;37m",
			&(nptr->name[14]) );
#else
		printf( "[1;37m%-30.30súú[0;34m([1;37mEscape[0;34m) "
			"([1;37mEnter[0;34m) [0;36mfor next [0;34m"
			"([1;37mSpace[0;34m) [0;36mto toggle tag[1;37m",
			&(nptr->name[9]) );
#endif
		do{
			bool dummy;
			switch( XGetKey(&dummy) ){
				case ' ':
					if( nptr->flags & FMN_TAGGED ){
						nptr->flags &= ~FMN_TAGGED;
						tagline( nptr );
					}
					else{
						nptr->flags |= FMN_TAGGED;
						tagline( nptr );
					}
				case KEY_RET: goto _getNextArea;
				case KEY_ESC: return;
			}
		}while( 1 );
	_getNextArea:
	}
	gotoxy( 1, 24 );
	waitEnter( NOSRCH_TEXT );
}


/*
 * Select limit of search scope
*/
	static int
selectMode( int group )
{
	pmenubar( SL_FILWHERE );
	do{
		bool dummy;
		switch( XGetKey(&dummy) ){

			case KEY_ESC: return -1;

			case 'T': case 't':
				if( !_numTagged ){
					waitEnter( NOTAGS_TEXT );
					pmenubar( SL_FILWHERE );
				}
				else return SRCH_TAGGED;
			break;

			case 'G': case 'g':
				if( -1 == group ){
					waitEnter( NOGROUP_TEXT );
					pmenubar( SL_FILWHERE );
				}
				else return SRCH_GROUP;
			break;

			case 'A': case 'a': return SRCH_AREA;
			case 'I': case 'i': return SRCH_ALL;
		}
	}while( 1 );
}


/*
 * selects and executes a search
*/
	void
selectSearch( dlist_t *list, FMN_STRUCT *focus, int group )
{
	bool dummy;
	int  mode;

	pmenubar( SL_FILSEARCH );
	do{
		switch( XGetKey(&dummy) ){
			case KEY_ESC: return;
			case 'A': case 'a': /* search for area text */
				areaSearch( list, group );
			return;
			case 'F': case 'f': /* filename search */
				mode = selectMode( group );
				if( -1 != mode )
					filenameSearch( list, focus, group, mode );
			return;
			case 'K': case 'k': /* keyword search */
				mode = selectMode( group );
				if( -1 != mode )
					keywordSearch( list, focus, group, mode );
			return;
		}
	}while( 1 );
}


/*
 * transfer files
*/
	void
transferFiles( dlist_t *list, FMN_STRUCT *focus, int group )
{
	int  mode;
	bool dummy;

	pmenubar( SL_TRANSFER );
	do{
		switch( XGetKey(&dummy) ){
			case KEY_ESC: return;
			case 'D': case 'd': /* download files */
				mode = selectMode( group );
				if( -1 != mode ){
					if( EOS != sendDataStr[0] )
						sprintf( _dataParam, "%s ", nameDataStr );
					else _dataParam[0] = EOS;
					if( !sendNoParam )
						buildDataStr( list, focus, group, mode );
#if defined( PB_SDK )
					MenuFunction( sendFunc, _dataParam );
#else
					ClearScr();
					printf( "download: %d, '%s'", sendFunc, _dataParam );
					getch();
#endif
				}
			return;
			case 'U': case 'u': /* upload files */
#if defined( PB_SDK )
				MenuFunction( receiveFunc, receiveDataStr );
#else
				ClearScr();
				printf( "upload: %d, '%s'", receiveFunc, receiveDataStr );
				getch();
#endif
			return;
		}
	}while( 1 );
}


/*
 * edits the tag lists (areas and files)
*/
	void
editTags( void )
{
#if defined( PB_SDK )
	MenuFunction( editFunc, editDataStr );
#else
	ClearScr();
	printf( "edit file tags: %d, '%s'", editFunc, editDataStr );
	getch();
#endif
}
