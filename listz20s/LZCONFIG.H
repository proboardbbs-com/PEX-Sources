/*
 * This file is part of Listerz v2.0 (VE)
 *
 * Copyright (C) 1995-1997 by Branislav L. Slantchev
 * A Product of Silicon Creations, Inc.
 *
 * This file is distributed under the terms and conditions of the GNU
 * General Public License. For more information, refer to the file
 * Copying.Doc which is included in the archive.
*/
#ifndef INCLUDED_LZCONFIG_H
#define INCLUDED_LZCONFIG_H
#include "geometry.h"

#define ARCHIVER_ZIP 1
#define ARCHIVER_RAR 2
#define ARCHIVER_ARJ 3
#define ARCHIVER_ZOO 4
#define ARCHIVER_LHA 5

typedef struct
{
	zRect bounds;   // bounds of the progress bar
	uchar fgColor;  // foreground color attribute
	uchar bgColor;  // background color attribute
	uchar fgChar;   // foreground character
	uchar bgChar;   // background character
} PROGBAR;

typedef struct
{
	char    path[80];    //..............................path to the archiver
	char    options[36]; //..........................options for the archiver
	Boolean enabled;     //.........................is the archiver available
	Boolean swap;        //...........................should we swap when run
} ARCHIVER;

typedef struct
{
	// SYSTEM PATH AND FILES
	char     sys_Path[80];       //................system directory for files
	char     sys_MainFile[80];   //........file to display as main background
	char     sys_ProcFile[80];   //..........file to display while processing
	// MAIN SELECTION SCREEN
	zRect    lbox_Bounds;        //............................listbox window
	uchar    lbox_TagChar;       //..............character to use for tagging
	uchar    lbox_TagColor;      //...............color for the tag character
	uchar    lbox_TagHilite;     //.......color for the hilited tag character
	uchar    lbox_Color;         //...........text for area names in list box
	uchar    lbox_Hilite;        //.......................selection bar color
	// COMMAND BAR CONFIGURATION
	zRect    cbar_Bounds;        //....................bounds for command bar
	uchar    cbar_Color;         //...........................color for items
	uchar    cbar_Background;    //......................color for background
	uchar    cbar_HotkeyColor;   //.....................color for the hotkeys
	uchar    cbar_HiliteColor;   //..................color for the hilite bar
	uchar    cbar_OffColor;      //.............color for items when disabled
	uchar    cbar_OffBackground; //........color for background when disabled
	// PROGRESS SCREEN
	zRect    proc_Bounds;        //...........................progress window
	uchar    proc_Color;         //...color for area names in progress window
	// CURRENT AREA PROGRESS LOCATIONS AND COLORS
	zRect    area_Bounds;        //...............rectangle for the area name
	uchar    area_Color;         //...................color for the area name
	zRect    area_Percent;       //.....percentage for current area (4 chars)
	PROGBAR  area_Progbar;       //......progress bar config for current area
	// STATISTICS LOCATIONS AND COLORS
	zRect    total_Percent;      //......................percentage for total
	PROGBAR  total_Progbar;      //.........progress bar config for all areas
	zRect    total_Areas;        //....................total areas (15 chars)
	zRect    total_Files;        //...............................total files
	zRect    total_Size;         //................................total size
	zRect    total_ListSize;     //.....................size of compiled list
	uchar    total_DigitColor;   //........color for the digits in the totals
	// DROP FILE ONLINE SUPPORT
	Boolean  drop_UserDoes;      //......................create UserDoes file
	Boolean  drop_Doing;         //.........................create Doing file
	char     drop_Text[40];      //.........text to place in the drop file(s)
	// ARCHIVER CONFIGURATIONS
	ARCHIVER zip;
	ARCHIVER rar;
	ARCHIVER arj;
	ARCHIVER lha;
	ARCHIVER zoo;
	// HEADER AND FOOTERS
	char     text_HeaderAll[80]; //......header file for the complete listing
	char     text_FooterAll[80]; //......footer file for the complete listing
	char     text_Header[80];    //...........header for the individual areas
	char     text_Footer[80];    //...........footer for the individual areas
	// SPACE FOR EXPANSION
	char     extra[726];         //.......space reserved for future expansion
} LZCONFIG;

extern LZCONFIG lz_Config;

#endif /* INCLUDED_LZCONFIG_H */
