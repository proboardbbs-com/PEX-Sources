/*
 * This file is part of LZSETUP (Configuration program for Listerz)
 *
 * Copyright (c) 1997 Branislav L. Slantchev (gargoyle)
 * A fine product of Silicon Creations, Inc.
 *
 * This file is released under the terms and conditions of the GNU
 * General Public License Version 2. The full text of the license is
 * supplied in the Copying.Doc file included with this archive. This
 * free software comes with absolutely no warranty, as outlined in the
 * licensing text. You are not allowed to remove this copyright notice.
 *
 * Contact: Branislav L. Slantchev at 73023.262@compuserve.com
*/
#include <stdlib.h>
#include <stdio.h>
#include <opcrt.h>
#include <opcolor.h>
#include <opmenu.h>
#include <opsedit.h>
#include <opdos.h>
#include <opentry.h>
#include <oppick.h>
#include <opmouse.h>
#include "../lzconfig.h"
#include "lzs_cmds.h"
#include "lzs_vers.h"
#include "file.h"

extern void       DisplayBanner();
extern boolean    MsgAsk(char*, char*, boolean*,ColorSet&,boolean=TRUE);
extern boolean    AskFileName(char *header, char *fileName, ColorSet&);
extern void       DropFile();
extern void       SystemPaths();
extern word       InitMenu(Menu &M, ColorSet &Colors);
extern void       ArchiverSetup(int archiver);
extern void       SelectColor(byte *attrib, char *header);
extern void       SelectSymbol(byte *symbol, char *header);
extern void       EditScreen(char *fileName, int nRects, zRect*[], char*[]);
extern boolean    LoadConfig();
extern void       SaveConfig();
extern void       proboard_emulator_init();
extern void       proboard_emulator_term();
extern byte far   LzsKeySet[];

// Color set used by entry screen
ColorSet lz_Colors =
{
  /* TextColor */        CyanOnBlack,     /* TextMono */         LtGrayOnBlack,
  /* CtrlColor */        YellowOnBlue,    /* CtrlMono */         WhiteOnBlack,
  /* FrameColor */       WhiteOnBlack,    /* FrameMono */        LtGrayOnBlack,
  /* HeaderColor */      YellowOnBlue,    /* HeaderMono */       BlackOnLtGray,
  /* ShadowColor */      BlackOnBlack,    /* ShadowMono */       WhiteOnBlack,
  /* HighlightColor */   WhiteOnRed,      /* HighlightMono */    BlackOnLtGray,
  /* PromptColor */      LtGrayOnBlack,   /* PromptMono */       LtGrayOnBlack,
  /* SelPromptColor */   BlueOnLtGray,    /* SelPromptMono */    LtGrayOnBlack,
  /* ProPromptColor */   BlackOnCyan,     /* ProPromptMono */    LtGrayOnBlack,
  /* FieldColor */       LtGrayOnBlack,   /* FieldMono */        LtGrayOnBlack,
  /* SelFieldColor */    BlueOnGreen,     /* SelFieldMono */     WhiteOnBlack,
  /* ProFieldColor */    LtGrayOnBlue,    /* ProFieldMono */     LtGrayOnBlack,
  /* ScrollBarColor */   CyanOnBlue,      /* ScrollBarMono */    LtGrayOnBlack,
  /* SliderColor */      CyanOnBlue,      /* SliderMono */       WhiteOnBlack,
  /* HotSpotColor */     BlackOnCyan,     /* HotSpotMono */      BlackOnLtGray,
  /* BlockColor */       YellowOnCyan,    /* BlockMono */        WhiteOnBlack,
  /* MarkerColor */      WhiteOnMagenta,  /* MarkerMono */       BlackOnLtGray,
  /* DelimColor */       BlueOnCyan,      /* DelimMono */        WhiteOnBlack,
  /* SelDelimColor */    BlueOnCyan,      /* SelDelimMono */     WhiteOnBlack,
  /* ProDelimColor */    BlueOnCyan,      /* ProDelimMono */     WhiteOnBlack,
  /* SelItemColor */     BlueOnLtGray,    /* SelItemMono */      BlackOnLtGray,
  /* ProItemColor */     DkGrayOnBlack,   /* ProItemMono */      LtGrayOnBlack,
  /* HighItemColor */    YellowOnBlack,   /* HighItemMono */     WhiteOnBlack,
  /* AltItemColor */     CyanOnBlack,     /* AltItemMono */      WhiteOnBlack,
  /* AltSelItemColor */  BlueOnLtGray,    /* AltSelItemMono */   BlackOnLtGray,
  /* FlexAHelpColor */   WhiteOnBlue,     /* FlexAHelpMono */    WhiteOnBlack,
  /* FlexBHelpColor */   WhiteOnBlue,     /* FlexBHelpMono */    WhiteOnBlack,
  /* FlexCHelpColor */   LtCyanOnBlue,    /* FlexCHelpMono */    BlackOnLtGray,
  /* UnselXrefColor */   YellowOnBlue,    /* UnselXrefMono */    LtBlueOnBlack,
  /* SelXrefColor */     WhiteOnMagenta,  /* SelXrefMono */      BlackOnLtGray,
  /* MouseColor */       WhiteOnRed,      /* MouseMono */        BlackOnLtGray
};

LZCONFIG          lz_Config;
char              lz_ConfigPath[80];
FrameArray        lz_FrameType = "ÖÓ·½ÄÄºº";
boolean           lz_Dirty;
CommandProcessor  lz_Commands;
RawWindow         StatusLine;
word              Status;
Menu              Menu;

// display copyright text header
void
DisplayCopyright()
{
	ClearWindow(1, 1, ScreenWidth, 1, ' ', BlackOnCyan);
	FastCenter(__COPYRIGHT__, 1, BlackOnCyan);
}

// initialize window for the help line
word
InitHelpLine(RawWindow &aStatusLine)
{
	byte Row = 25;
	if( !aStatusLine.InitCustom(1,Row,ScreenWidth,Row,lz_Colors,wClear) )
		return aStatusLine.InitStatus;
	aStatusLine.SetTextAttr(BlackOnLtGray, LtGrayOnBlack);
	return 0;
}

// report errors
void
ErrorHandler(byte ModuleCode, word &ErrCode, char *Msg)
{
	RingBell();
}

void
UpdateHelpLine(MenuItemNodePtr CurrentItem, MenuPtr)
{
	char s[255];

	if( CurrentItem == NULL ) s[0] = '\0';
	else
	{
		CurrentItem->HelpString(&s[1]);
		s[0] = ' ';
	}
	Pad(s, StatusLine.Width(), s);
	StatusLine.wFastText(s, 1, 1);
}

/////////////////////////////////////////////////////////////////////////////
// duh!
int
main()
{
	DetectMultitasking = TRUE;

	OPMenuInit();
    OPWindowInit();
	OPSEditInit();
	OPEntryInit();
	OPPickInit();
	OPMouseInit();

	if( MouseInstalled ) MouseGotoXY(1,1);

	proboard_emulator_init();

	Status = InitMenu(Menu, lz_Colors);
	if( Status != 0 )
	{
		cprintf("Error initializing menu: %u\r\n", Status);
		return 0;
	}

	//set up user hooks
	Menu.SetErrorFunc(ErrorHandler);
	lz_Commands.Init((CmdTablePtr)LzsKeySet, LZS_KEYSMAX);

	Status = InitHelpLine(StatusLine);
	if( Status != 0 )
	{
		cprintf("Error initializing help line: %d\r\n", Status);
		exit(1);
	}

	Menu.SetCurrentItemFunc(UpdateHelpLine);

	TextChar = '°';
	ClrScr();
	DisplayCopyright();
	StatusLine.Draw();
	Menu.Draw();

	lz_Dirty = FALSE;
	LoadConfig();  // may set Dirty to TRUE!!! (sets lz_ConfigPath)

MenuLoop:
	for( ;; )
	{
		int    command;
		zRect *rects[10];
		char  *headers[10];

		Menu.Process();
		command = Menu.GetLastCommand();

		if( ccQuit == command ) goto ExitMenuLoop;
		else if( ccSelect != command ) continue;

		switch( Menu.MenuChoice() )
		{
				case miMainScreen:
					rects[0] = &lz_Config.lbox_Bounds;
					rects[1] = &lz_Config.cbar_Bounds;
					headers[0] = " Name Display Area ";
					headers[1] = " Command Bar Area ";
					EditScreen(lz_Config.sys_MainFile, 2, rects, headers);
					break;

				case miProcScreen:
					rects[0] = &lz_Config.proc_Bounds;
					rects[1] = &lz_Config.area_Bounds;
					rects[2] = &lz_Config.area_Percent;
					rects[3] = &lz_Config.area_Progbar.bounds;
					rects[4] = &lz_Config.total_Percent;
					rects[5] = &lz_Config.total_Progbar.bounds;
					rects[6] = &lz_Config.total_Areas;
					rects[7] = &lz_Config.total_Files;
					rects[8] = &lz_Config.total_Size;
					rects[9] = &lz_Config.total_ListSize;
					headers[0] = " Name Display Area ";
					headers[1] = " Current Area Name ";
					headers[2] = "% Current Area";
					headers[3] = " Current Progress Bar ";
					headers[4] = "% All Areas";
					headers[5] = " Total Progress Bar ";
					headers[6] = " Total Areas ";
					headers[7] = " Total Files ";
					headers[8] = " Total Size ";
					headers[9] = " Listing Size ";
					EditScreen(lz_Config.sys_ProcFile, 10, rects, headers);
					break;

				case miTest:
					break;

				case miPbarFgChar:
					SelectSymbol(&lz_Config.area_Progbar.fgChar, " Fore Symbol ");
					break;
				case miPbarBgChar:
					SelectSymbol(&lz_Config.area_Progbar.bgChar, " Back Symbol ");
					break;
				case miPbarTotalFgChar:
					SelectSymbol(&lz_Config.total_Progbar.fgChar, " Fore Symbol ");
					break;
				case miPbarTotalBgChar:
					SelectSymbol(&lz_Config.total_Progbar.bgChar, " Back Symbol ");
					break;
				case miTagSymbol:
					SelectSymbol(&lz_Config.lbox_TagChar, " Tag Symbol ");
					break;
				case miListAreaName:
					SelectColor(&lz_Config.lbox_Color, " List Box ");
					break;
				case miListSelectBar:
					SelectColor(&lz_Config.lbox_Hilite, " Hilite Bar ");
					break;
				case miListTag:
					SelectColor(&lz_Config.lbox_TagColor, " Tag Symbol ");
					break;
				case miListTagHilite:
					SelectColor(&lz_Config.lbox_TagHilite, " Tag Hilite ");
					break;
				case miCbarActiveFg:
					SelectColor(&lz_Config.cbar_Color, " Active Text " );
					break;
				case miCbarActiveBg:
					SelectColor(&lz_Config.cbar_Background, " Active Fill " );
					break;
				case miCbarHotkey:
					SelectColor(&lz_Config.cbar_HotkeyColor, " Hotkeys ");
					break;
				case miCbarHilite:
					SelectColor(&lz_Config.cbar_HiliteColor, " Hilite Text ");
					break;
				case miCbarOffFg:
					SelectColor(&lz_Config.cbar_OffColor, " Inactive Text ");
					break;
				case miCbarOffBg:
					SelectColor(&lz_Config.cbar_OffBackground, " Inactive Fill ");
					break;
				case miProcText:
					SelectColor(&lz_Config.proc_Color, " List Text ");
					break;
				case miProcDigits:
					SelectColor(&lz_Config.total_DigitColor, " Digits ");
					break;
				case miProcAreaName:
					SelectColor(&lz_Config.area_Color, " Hilite Text ");
					break;
				case miPbarFg:
					SelectColor(&lz_Config.area_Progbar.fgColor, " Current Fg ");
					break;
				case miPbarBg:
					SelectColor(&lz_Config.area_Progbar.bgColor, " Current Bg ");
					break;
				case miPbarTotalFg:
					SelectColor(&lz_Config.total_Progbar.fgColor, " Total Fg ");
					break;
				case miPbarTotalBg:
					SelectColor(&lz_Config.total_Progbar.bgColor, " Total Bg ");
					break;
				case miZIP          : ArchiverSetup(ARCHIVER_ZIP); break;
				case miRAR          : ArchiverSetup(ARCHIVER_RAR); break;
				case miARJ          : ArchiverSetup(ARCHIVER_ARJ); break;
				case miLHA          : ArchiverSetup(ARCHIVER_LHA); break;
				case miZOO          : ArchiverSetup(ARCHIVER_ZOO); break;
				case miFileLocations: SystemPaths();   break;
				case miUseron       : DropFile();      break;
				case miInformation  : DisplayBanner(); break;
				case miExit         : goto ExitMenuLoop;
		}
	}

ExitMenuLoop:
	if( lz_Dirty )
	{
		boolean escaped, rv;

		rv = MsgAsk("Warning", "Configuration modified. Save now?", &escaped, lz_Colors);
		if( escaped ) goto MenuLoop;

		if( rv )
		{
			AskFileName(" Config File ", lz_ConfigPath, lz_Colors);
			SaveConfig();  // uses lz_ConfigPath
		}
	}

	Menu.Erase();
	Menu.Done();
	StatusLine.Erase();
	StatusLine.Done();
	lz_Commands.Done();

	ClrScr();
	cprintf("lzSetup %s ù The Listerz Configuration Utility\n\r",__VERSION__);
	cprintf("    þ Copyright (c) 1997 Branislav L. Slantchev\n\r");
	cprintf("    þ A Fine Product of Silicon Creations, Inc.\n\r");
	cprintf("    þ Release version compiled on %s at %s\n\r", __DATE__, __TIME__);

	proboard_emulator_term();

	return 0;
}
