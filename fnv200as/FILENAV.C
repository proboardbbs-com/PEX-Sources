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
#include "xkeys.h"
#include "inifile.h"
#include "getopt.h"
#include "dirmanip.h"
#include "userd.h"

#if !defined( PB_SDK )
	#define MEM_DEBUG 1
	#include "memdbg.h"
	#include <stdlib.h>
#if defined( __BORLANDC__ )
	#include <alloc.h>
#endif
	USER_REC _curUser, * const CurUser;
	USER_REC **curUserPtr = (USER_REC **)&CurUser;
#endif

char _systemPath[MAXPATH];                	/* ProBoard sytem directory */
char _helpFilePath[MAXPATH];

int _clrBracket, _clrDigit, _clrDotOne, _clrDotTwo, _clrName,
	 _clrTagged, _clrFlagOne, _clrFlagTwo, _clrFlagThree;

/* this will be != 0 if there are any tagged files */
int _numTagged = 0;

/*
 * External function definition to list files, search by keyword or name
*/

/* function number to use for this one  */
int  listFunc, keywordFunc, nameFunc, sendFunc, receiveFunc, editFunc;
/* should we append our data to line    */
bool listNoParam, keywordNoParam, nameNoParam, sendNoParam;
/* see if we have special data for line  */
char listDataStr[MAX_DATA], keywordDataStr[MAX_DATA], nameDataStr[MAX_DATA],
	 sendDataStr[MAX_DATA], receiveDataStr[MAX_DATA], editDataStr[MAX_DATA];

static void getSetup( char *path );

EXTERN void listFiles( dlist_t *list, FMN_STRUCT *focus );
EXTERN select( dlist_t *, FMN_STRUCT **, char*, int, int, bool);
EXTERN int  readGroups( dlist_t *list );
EXTERN int  readAreas( dlist_t *list );
EXTERN void selectSearch( dlist_t*, FMN_STRUCT *, int );
EXTERN void transferFiles( dlist_t*, FMN_STRUCT *, int );
EXTERN void editTags( void );
EXTERN void waitEnter( char *text );

	int
main( int argc, char *argv[] )
{
	dlist_t *groupList = NULL,                    /* list of all groups */
			*areaList = NULL;                      /* list of all areas */
	int group = -1, opt;                 /* current group, -1 == ignore */
	bool restart;                      /* for areas: restart from first */
	char cfgPath[MAXPATH], *cfgPathPtr = NULL;
	char areaKeys[20], groupKeys[20];
	bool groupsOnly = FALSE,
		 areasOnly = FALSE,
		 changeOnly = FALSE,
		 userDoes = FALSE;

#if !defined( PB_SDK )
	#if defined( __BORLANDC__ )
		long  core = coreleft();
	#endif
	mem_init();
	strcpy( _systemPath, getenv("PROBOARD") );
	*curUserPtr = &_curUser; CurUser->level = 32000; CurUser->aFlags = 0;
#else
	if( !CurUser->uFlags & UFLAG_ANSI ){
		printf( "The File Library Navigator requires an ANSI terminal.\n" );
		printf( "Please, enable your ANSI emulation and try again.\n" );
		printf( "Press (Enter) to continue..." );
		return 1;
	}
	CurUser->uFlags |= UFLAG_CLEAR;
	strcpy( _systemPath, SysPath );
#endif


	/* process command-line arguments */
	while( EOF != (opt = getopt(argc, argv, "f:F:aAgGcCn:N:uU")) ){
		switch( tolower(opt) ){
			case 'f': cfgPathPtr = optarg; break;
			case 'a': areasOnly = TRUE; break;
			case 'g': groupsOnly = TRUE; changeOnly = TRUE; break;
			case 'c': changeOnly = TRUE; break;
			case 'n': group = atoi( optarg ); break;
			case 'u': userDoes = TRUE; break;
		}
	}

	/* do some initialization */
	if( userDoes ) userd_creat( "Browsing in FMNav 2.00" );
	if( cfgPathPtr ) strcpy( cfgPath, cfgPathPtr );
	else chext( cfgPath, *argv, ".INI" );
	chext( _helpFilePath, *argv, ".HLP" );
	getSetup( cfgPath );
	_numTagged = 0;
	XKeysInit();

	/* allocate and build the lists, setup hotkey lists */
	if( !areasOnly  ){
		groupList = dlist_new();
		if( !groupList ) return 1;
		if( -1 == readGroups(groupList) ) goto _freeQuit;
		strcpy( areaKeys, "HhSsEeTtGg" );
	}
	else{
		strcpy( areaKeys, "HhSsEeTt" );
	}

	if( !groupsOnly ){
		areaList = dlist_new();
		if( !areaList ) return 1;
		if( -1 == readAreas(areaList) ) goto _freeQuit;
		strcpy( groupKeys, "HhIi" );
	}
	else{
		strcpy( groupKeys, "Hh" );
	}

	if( areasOnly ) goto _procAreas;

	/* process the file groups */
_procGroups:
	do{
		FMN_STRUCT *focus;
		switch( select(groupList, &focus, groupKeys, -1, SL_GROUPS, TRUE ) ){
			case cmQuit: case cmNoEntry: goto _freeQuit;
			case 'H': case 'h':
#if defined( PB_SDK )
				MenuFunction( MENU_VIEW_FILE_WAIT, _helpFilePath );
#else
				waitEnter( _helpFilePath );
#endif
			break;
			case 'I': case 'i':	group = -1;	goto _procAreas;
			case cmOk:
#if defined( PB_SDK )
				CurUser->fileGroup = focus->num;
#endif
				if( changeOnly ) goto _freeQuit;
				group = focus->num;
			goto _procAreas;
		}
	}while(1);

	/* process the file areas */
_procAreas:
	restart = TRUE;
	do{
		FMN_STRUCT *focus;

		switch( select(areaList, &focus, areaKeys,
				group, SL_FILAREAS, restart) ){
			case cmQuit: goto _freeQuit;

			case cmOk:
#if defined( PB_SDK )
				CurUser->fileArea = focus->num;
#endif
				if( changeOnly ) goto _freeQuit;
				listFiles( areaList, focus );
				restart = FALSE;
			break;

			/* this will not occur in area-only mode, keys ignored */
			case cmNoEntry: if( areasOnly ) goto _freeQuit;
			case 'G': case 'g': goto _procGroups;

			case 'H': case 'h':
#if defined( PB_SDK )
				MenuFunction( MENU_VIEW_FILE_WAIT, _helpFilePath );
#else
				waitEnter( _helpFilePath );
#endif
				restart = FALSE;
			break; /* display ANSI help file */
			case 'S': case 's':
				selectSearch( areaList, focus, group );
				restart = FALSE;
			break;
			case 'E': case 'e':
				ClearScr();
				editTags();
				restart = FALSE;
			break;
			case 'T': case 't':
				transferFiles( areaList, focus, group );
				restart = FALSE;
			break;
		}
	}while( 1 );

_freeQuit:
	/* free the memory for the lists */
	if( !areasOnly ) dlist_free( groupList, NULL );
	if( !groupsOnly ) dlist_free( areaList, NULL );

	XKeysDeinit();
	ClearScr();

#if !defined( PB_SDK )
	mem_term();
#if defined( __BORLANDC__ )
	printf( "\n\nCore_start: %08lx\nCore_close: %08lx\n", core, coreleft() );
#endif
#endif
	if( userDoes ) userd_unlink();
	return 0;
}


	void
getSetup( char *path )
{
	/* read configuration for the keyword search */
	IniReadInt( path, "Keyword", "Function", &keywordFunc, 35 );
	IniReadBool( path, "Keyword", "NoParam", &keywordNoParam, FALSE );
	IniReadStr( path, "Keyword", "Data", keywordDataStr, "\0" );

	/* read configuration for the filename search */
	IniReadInt( path, "Filename", "Function", &nameFunc, 36 );
	IniReadBool( path, "Filename", "NoParam", &nameNoParam, FALSE );
	IniReadStr( path, "Filename", "Data", nameDataStr, "\0" );

	/* read configuration for the file list function */
	IniReadInt( path, "List", "Function", &listFunc, 31 );
	IniReadBool( path, "List", "NoParam", &listNoParam, FALSE );
	IniReadStr( path, "List", "Data", listDataStr, "\0" );

	/* read the configuration for the send function */
	IniReadInt( path, "Send", "Function", &sendFunc, 32 );
	IniReadBool( path, "Send", "NoParam", &sendNoParam, FALSE );
	IniReadStr( path, "Send", "Data", sendDataStr, "\0" );

	/* read the configuration for the send function */
	IniReadInt( path, "Receive", "Function", &receiveFunc, 33 );
	IniReadStr( path, "Receive", "Data", receiveDataStr, "\0" );

	/* read the configuration for the edit tagged files function */
	IniReadInt( path, "EditTags", "Function", &editFunc, 68 );
	IniReadStr( path, "EditTags", "Data", editDataStr, "\0" );

	/* retrieve the color definitions */
	IniReadInt( path, "Colors", "Bracket", &_clrBracket, 0x04 );
	IniReadInt( path, "Colors", "Digits", &_clrDigit, 0x03 );
	IniReadInt( path, "Colors", "LeadDot", &_clrDotOne, 0x02 );
	IniReadInt( path, "Colors", "FillDot", &_clrDotTwo, 0x04 );
	IniReadInt( path, "Colors", "NameText", &_clrName, 0x07 );
	IniReadInt( path, "Colors", "TaggedText", &_clrTagged, 0x02 );
	IniReadInt( path, "Colors", "FlagOne", &_clrFlagOne, 0x02 );
	IniReadInt( path, "Colors", "FlagTwo", &_clrFlagTwo, 0x05 );
	IniReadInt( path, "Colors", "FlagThree", &_clrFlagThree, 0x01 );
}

