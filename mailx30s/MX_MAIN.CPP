/*
 * This file is part of mail-x (EXE)
 *
 * Copyright (C) 1995-1997 by Branislav L. Slantchev
 * A Product of Silicon Creations, Inc.
 *
 * This file is distributed under the terms and conditions of the GNU
 * General Public License. For more information, refer to the file
 * Copying.Doc which is included in the archive.
*/
#include "mx_area.h"
#include "proboard.h"
#include "pbnames.h"
#include "keyboard.h"
#include "kbcodes.h"
#include "stdmac.h"
#include "cmdbar.h"
#include "profile.h"
#include "bitvect.h"
#include "terminal.h"
#include "utils.h"
#include "getopt.h"
#include "file.h"
#include "useron.h"

#ifndef PB_SDK
	#include <stdio.h>
	#include <stdlib.h>
	#include <ctype.h>
	#include "_pbinit.h"
#else
	#include "pblibc.h"
	#include "pblsdk.h"
	#define  proboard_emulator_init()
	#define  proboard_emulator_term()
#endif

const short
	cmQuit      = 1, cmSaveExit = 2, cmTagAll = 3, cmUntagAll = 4,
	cmToggleAll = 5, cmRedraw   = 6;

/* used by the ChangeTag function */
const int gc_StateSet    = 1;
const int gc_StateUnset  = 2;
const int gc_StateToggle = 3;

/* used for global operation option */
#define GLOBAL_NONE    0
#define GLOBAL_ON      1
#define GLOBAL_OFF     2
#define GLOBAL_FORCED  3

/* used for drop file type */
#define DROP_NOTHING   0
#define DROP_USERDOES  1
#define DROP_DOING     2

zDoubleList    *mx_AreaList       = 0;
zCommandBar    *mx_CommandBar     = 0;
zListViewer    *mx_ListBox        = 0;
zBitVector     *mx_ForceList      = 0;
zTerminal      *mx_Terminal       = 0;
pcboard_interp *mx_PCBoardHandler = 0;
ansi_interp    *mx_AnsiHandler    = 0;
avatar_interp  *mx_AvatarHandler  = 0;
uchar          *mx_UserArea       = 0;
zUserOnline    *mx_UserOnline     = 0;
zKeyboard       mx_Keyboard;

char            mx_DisplayFile[MAXPATH];
char            mx_Copyright[100];
char            mx_Registration[100];
char            CommandBarPalette[15];

char            mx_TagChar;
int             mx_TagColor;
int             mx_TagHilite;
int             mx_ListColor;
int             mx_ListHilite;

const char *Header[3] =
{
	" _     *                  *     _ ",
	"( `ú._( `ú._ .mail-x. _.ú' )_.ú' )",
	"þðþðþðþðþðþðþðþðþðþðþðþðþðþðþðþðþðþðþðþ"
		"ðþðþðþðþðþðþðþðþðþðþðþðþðþðþðþðþðþðþðþ"
};

void
SetGlobally(int global)
{
	switch( global )
	{
		case GLOBAL_ON:
			memset(mx_UserArea, 0xff, sizeof(combinedboards));
			break;
		case GLOBAL_OFF:
			memset(mx_UserArea, 0x00, sizeof(combinedboards));
			break;
		case GLOBAL_FORCED:
			for( int i = 0; i < 1000; ++i )
			{
				if( mx_ForceList->has(i) ) SetBit(mx_UserArea, i);
			}
			break;
	}
}

int
init_areas()
{
	FILE    *fp;
	MSGAREA  area;
	short    areano = 0;

	fp = pb_fopen(fnMESSAGES, "rb", PBDIR_SYS);
	if( fp && 0 != (mx_AreaList = new zDoubleList(True)) )
	{
		fread(&area, sizeof(area), 1, fp);
		while( !feof(fp) && areano < 1000 )
		{
			if( EOS != area.name[0] &&
				pb_access(area.readLevel, area.readFlags, area.readFlagsNot) )
			{
				mx_AreaList->link(new Area(areano, area.name));
			}
			areano++;
			fread(&area, sizeof(area), 1, fp);
		}
	}

	return (int)mx_AreaList;
}

/*
 * change the tag for the area
*/
void
ChangeTag(void *pItem, void *arg)
{
	Area *item  = (Area *)pItem;
	int   state = *(int *)arg;

	// don't modify forced areas (these are set already)
	if( !mx_ForceList->has(item->m_index) )
	{
		if( gc_StateSet == state ) item->m_tagged = True;
		else if( gc_StateUnset == state ) item->m_tagged = False;
		else item->toggleTag();
	}
}

/*
 * synchronize the area list with list of forced areas
*/
void
SyncForced(void *pItem, void *arg)
{
	Area       *item   = (Area *)pItem;
	zBitVector *bitmap = (zBitVector *)arg;

	if( bitmap->has(item->m_index) ) item->m_tagged = True;
}

/* save status of the current area (called for all of them) */
void
SaveState(void *pItem, void*)
{
	Area *item = (Area *)pItem;

	if( item->m_tagged ) SetBit(mx_UserArea, item->m_index);
	else ClearBit(mx_UserArea, item->m_index);
}

/* get the current status of the area from user record */
void
RetrieveState(void *pItem, void*)
{
	Area *item = (Area *)pItem;

	if( TestBit(mx_UserArea, item->m_index) ) item->m_tagged = True;
	else item->m_tagged = False;
}

/* process command bar request */
int
ProcessCommand()
{
	int          retval = cmCancel;
	keyDownEvent event;

	mx_CommandBar->setState(True);

	for( ;; )
	{
		mx_Keyboard.getEvent(event);
		retval = mx_CommandBar->handle(event.keyCode);

		switch( retval )
		{
			case cmTagAll:
            	mx_AreaList->forEach(ChangeTag, (void *)&gc_StateSet);
				mx_ListBox->draw();
				break;
			case cmUntagAll:
				mx_AreaList->forEach(ChangeTag, (void *)&gc_StateUnset);
				mx_ListBox->draw();
				break;
			case cmToggleAll:
				mx_AreaList->forEach(ChangeTag, (void *)&gc_StateToggle);
				mx_ListBox->draw();
				break;
			case cmRedraw:
				clrscr();
				pb_showfile(mx_DisplayFile, NULL, mx_Terminal);
				mx_ListBox->draw();
				mx_CommandBar->draw();
				break;
			case cmSaveExit:
				mx_AreaList->forEach(SaveState, 0);
				/* fall-through to next case */
			case cmQuit    :
				/* fall-through to next case */
			case cmCancel  :
				goto Done;
		}
	}

Done:
	mx_CommandBar->setState(False);

	return retval;
}

/*
 * load application defaults from SILICON.INI file and setup lists
*/
int
init_application(int argc, char *argv[])
{
	char * const Section = "mail-x";
	char         fileName[MAXPATH];
	char *       pForced;
	zProfile     profile;
	int          retval = 0, opt;
	zRect        bounds;
	int          Global = GLOBAL_NONE;
	int          Dropfile = DROP_NOTHING;

	mx_UserArea = CurUser->mailCheckBoards;
	opterr = 0;

	/* -c : combined areas, -g [on|off|forced] */
	while( EOF != (opt = getopt(argc, argv, "cCg:G:d:D:")))
	{
		switch( tolower(opt) )
		{
			case 'c':
				mx_UserArea = CurUser->combinedBoards;
				break;
			case 'g':
				if( !stricmp(optarg, "on") ) Global = GLOBAL_ON;
				else if( !stricmp(optarg, "off") ) Global = GLOBAL_OFF;
				else if( !stricmp(optarg, "forced") ) Global = GLOBAL_FORCED;
				else
				{
					Log(LOG_FRIEND, "mail-x: unknown global mode '%s'", optarg);
					exit(EXIT_FAILURE);
				}
				break;
			case 'd':
				if( !stricmp(optarg, "userdoes") ) Dropfile = DROP_USERDOES;
				else if( !stricmp(optarg, "doing") ) Dropfile = DROP_DOING;
				break;
			default :
				Log(LOG_FRIEND, "mail-x: unknown option '%s'", optarg);
				exit(EXIT_FAILURE);
		}
	}

	file_chext(fileName, argv[0], ".ini");
	profile.Assign(fileName);
	profile.Open();
	/* get the list of areas to be forced */
	mx_ForceList = new zBitVector(1000);
	pForced = strtok(profile.ReadString(Section, "Force", ""), ", ");
	while( pForced )
	{
		mx_ForceList->set(atoi(pForced) - 1);
		pForced = strtok(NULL, ", ");
	}

	/* are we operating in global mode (if yes, perform operation and stop) */
	if( Global != GLOBAL_NONE )
	{
		SetGlobally(Global);
		delete mx_ForceList;
		proboard_emulator_term();
		exit(EXIT_SUCCESS);
	}

	/* this is the interactive mode here, no emulation, run proboard */
	if( !(CurUser->uFlags & UFLAG_ANSI)
		&& !(CurUser->uFlags & UFLAG_AVATAR)
		&& !(CurUser->uFlags & UFLAG_AVTPLUS) )
	{
		Log(LOG_FRIEND, "mail-x: caller '%s' has an ascii terminal!", CurUser->name);
		delete mx_ForceList;

#ifdef PB_SDK // run proboard's internal selectors
		if( mx_UserArea == CurUser->mailCheckBoards )
			MenuFunction(MENU_COMBINED_SELECT, "/M");
		else MenuFunction(MENU_COMBINED_SELECT, "");
#endif

		proboard_emulator_term();
		exit(EXIT_FAILURE);
	}

	/* setup the areas and the list box */
	if( init_areas() )
	{
		/* get the name of the display file to show (ANS/ASC/PCB/WC) */
		strcpy(mx_DisplayFile, profile.ReadString(Section, "DisplayFile",
									pb_getpath(Section, PBDIR_TEXT)));
		/* get the position of the command bar rectangle */
		bounds.a.x = profile.ReadShort(Section, "bar.LeftCol", 10);
		bounds.b.x = profile.ReadShort(Section, "bar.RightCol", 70);
		bounds.a.y = profile.ReadShort(Section, "bar.Row", 20);
		bounds.b.y = bounds.a.y + 1;
		memset(CommandBarPalette, 0x00, sizeof(CommandBarPalette));
		CommandBarPalette[0] = profile.ReadShort(Section, "bar.On", 0x04);
		CommandBarPalette[5] = profile.ReadShort(Section, "bar.BgOn", 0x00);
		CommandBarPalette[1] = profile.ReadShort(Section, "bar.Hotkey", 0x0c);
		CommandBarPalette[3] = profile.ReadShort(Section, "bar.Hilite", 0x0f);
		CommandBarPalette[7] = profile.ReadShort(Section, "bar.Off", 0x08);
		CommandBarPalette[10] = profile.ReadShort(Section, "bar.BgOff", 0x00);
		CommandBarPalette[6] = CommandBarPalette[11] = ' ';
		CommandBarPalette[8] = CommandBarPalette[7];
		/* setup the command bar rectangle items and colors */
		zCommandItem *items =
			   new zCommandItem("~q~uit", 'Q', cmQuit)
			+ *new zCommandItem("~s~ave & exit", 'S', cmSaveExit)
			+ *new zCommandItem("~t~ag all", 'T', cmTagAll)
			+ *new zCommandItem("~u~ntag all", 'U', cmUntagAll)
			+ *new zCommandItem("t~o~ggle tags", 'O', cmToggleAll)
			+ *new zCommandItem("~r~edraw", 'R', cmRedraw);
		mx_CommandBar = new zCommandBar(bounds, items);
		mx_CommandBar->options &= ~zCommandBar::focus;
		mx_CommandBar->options &= ~zCommandBar::delim;
		mx_CommandBar->setPalette(CommandBarPalette);
		/* get the position of the area list box z*/
		bounds.a.x = profile.ReadShort(Section, "List.LeftCol", 23);
		bounds.b.x = profile.ReadShort(Section, "List.RightCol", 59);
		bounds.a.y = profile.ReadShort(Section, "List.UpperRow", 7);
		bounds.b.y = profile.ReadShort(Section, "List.LowerRow", 17);
		mx_ListBox = new zListViewer(bounds, mx_AreaList);
		/* synchronize the list with the forced list */
		mx_AreaList->forEach(RetrieveState, 0);
		mx_AreaList->forEach(SyncForced, mx_ForceList);
		/* read last configuration options */
		mx_TagChar     = profile.ReadString(Section, "list.TagChar", "û")[0];
		mx_TagColor    = profile.ReadShort(Section, "list.TagColor", 9);
		mx_TagHilite   = profile.ReadShort(Section, "list.TagHilite", 31);
		mx_ListColor   = profile.ReadShort(Section, "list.Color", 9);
		mx_ListHilite  = profile.ReadShort(Section, "list.Hilite", 31);
		/* setup the terminal interpreters */
		mx_Terminal       = new zTerminal;
		mx_PCBoardHandler = new pcboard_interp;
		mx_AvatarHandler  = new avatar_interp;
		mx_AnsiHandler    = new ansi_interp;
		mx_Terminal->RegisterHandler(mx_PCBoardHandler);
		mx_Terminal->RegisterHandler(mx_AvatarHandler);
		mx_Terminal->RegisterHandler(mx_AnsiHandler);
		/* display the background file */
		clrscr();
		pb_showfile(mx_DisplayFile, NULL, mx_Terminal);
		mx_ListBox->draw();
		mx_CommandBar->draw();
		/* tell the program that we are ok */
		retval = 1;
	}
	profile.Close();

	sprintf(mx_Registration, "registered to %s", Config->bbsname);
	strlwr(mx_Registration);
	strcpy(mx_Copyright, "coded by branislav l. slantchev (gargoyle)");

	if( Dropfile != DROP_NOTHING )
	{
		mx_UserOnline = new zUserOnline(Boolean(Dropfile == DROP_USERDOES),
							Boolean(Dropfile == DROP_DOING));
		mx_UserOnline->Create(NodeNumber, "in mail-x v3.0 (the best!)");
	}

	return retval;
}

/*
 * free memory and shutdown the proboard emulator (if necessary)
*/
void
term_application()
{
	if( mx_ListBox        ) delete mx_ListBox;
	if( mx_AreaList       ) delete mx_AreaList;
	if( mx_CommandBar     ) delete mx_CommandBar;
	if( mx_ForceList      ) delete mx_ForceList;
	if( mx_PCBoardHandler ) delete mx_PCBoardHandler;
	if( mx_AvatarHandler  ) delete mx_AvatarHandler;
	if( mx_AnsiHandler    ) delete mx_AnsiHandler;
	if( mx_Terminal       ) delete mx_Terminal;

	if( mx_UserOnline )
	{
		mx_UserOnline->Remove(NodeNumber);
		delete mx_UserOnline;
	}

	/* show brag string only if speed is enough */
	if( BaudRate == 0L || BaudRate >= 9600L )
	{
		clrscr();
		textattr(0x0f);
		gotoxy((80 - strlen(Header[0])) / 2, 10);
		cprintf(Header[0]);
		gotoxy((80 - strlen(Header[1])) / 2, 11);
		cprintf(Header[1]);
		textattr(0x03);
		gotoxy((80 - strlen(Header[2])) / 2, 12);
		cprintf(Header[2]);
		gotoxy((80 - strlen(Header[2])) / 2, 15);
		cprintf(Header[2]);
		effect_fade(mx_Copyright, (80 - strlen(mx_Copyright)) / 2, 13);
		effect_flash(mx_Registration, (80 - strlen(mx_Registration))/2, 14);
	}
}

void
main(int argc, char *argv[])
{
	proboard_emulator_init();

	if( init_application(argc, argv) )
	{
		for( ;; )
		{
			keyDownEvent event;

			mx_Keyboard.getEvent(event);
			switch( event.keyCode )
			{
				case kbEsc:
					int cmd = ProcessCommand();
					if( cmQuit == cmd || cmSaveExit == cmd ) goto Done;
					break;

				case ' '    :
				case kbEnter:
					if( !mx_ForceList->has(((Area *)mx_ListBox->focused())->m_index) )
					{
						mx_ListBox->focused()->toggleTag();
						mx_ListBox->drawFocus();
					}
					event.keyCode = kbDown;
					/* fall-through to default */
				default:
					mx_ListBox->handle(event.keyCode);
			}
		}
	}
Done:
	term_application();
	proboard_emulator_term();
}
