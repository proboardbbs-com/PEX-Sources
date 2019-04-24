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
#ifndef INCLUDED_LZS_CMDS_H
#define INCLUDED_LZS_CMDS_H

#define LZS_KEYSMAX 83

//Menu item #defines
#define  miScreen1           1
#define  miMainScreen        2
#define  miProcScreen        3
#define  miOptions4          4
#define  miCharacter5        5
#define  miSelection6        6
#define  miListAreaName      7
#define  miListSelectBar     8
#define  miListTag           9
#define  miListTagHilite     10
#define  miCommand11         11
#define  miCbarActiveFg      12
#define  miCbarActiveBg      13
#define  miCbarHotkey        14
#define  miCbarHilite        15
#define  miCbarOffFg         16
#define  miCbarOffBg         17
#define  miProgress18        18
#define  miProcText          19
#define  miProcDigits        20
#define  miProcAreaName      21
#define  miProgress22        22
#define  miPbarFg            23
#define  miPbarBg            24
#define  miPbarTotalFg       25
#define  miPbarTotalBg       26
#define  miCompression       27
#define  miZIP               28
#define  miRAR               29
#define  miARJ               30
#define  miLHA               31
#define  miZOO               32
#define  miFileLocations     33
#define  miUseron            34
#define  miInformation       35
#define  miTest              36
#define  miShell             37
#define  miExit              38
#define  miPbarFgChar        39
#define  miPbarBgChar        40
#define  miPbarTotalFgChar   41
#define  miPbarTotalBgChar   42
#define  miTagSymbol         43

// entry field #defines (DROPFILE)
#define id_UserDoes     0
#define id_Doing        id_UserDoes + 1
#define id_UseronText   id_Doing    + 1

// help index #defines (DROPFILE)
#define hi_UserDoes     1
#define hi_Doing        hi_UserDoes   + 1
#define hi_UseronText   hi_Doing      + 1

// file dialog prompt
#define hi_FileName     hi_UseronText + 1

//Entry field #defines (PATHS)
#define id_System              0
#define id_AnsiMain            id_System       + 1
#define id_ProcAnsi            id_AnsiMain     + 1
#define id_GlobalHeader        id_ProcAnsi     + 1
#define id_GlobalFooter        id_GlobalHeader + 1
#define id_AreaHeader          id_GlobalFooter + 1
#define id_AreaFooter          id_AreaHeader   + 1

//Help index #defines
#define hi_System              1
#define hi_AnsiMain            hi_System       + 1
#define hi_ProcAnsi            hi_AnsiMain     + 1
#define hi_GlobalHeader        hi_ProcAnsi     + 1
#define hi_GlobalFooter        hi_GlobalHeader + 1
#define hi_AreaHeader          hi_GlobalFooter + 1
#define hi_AreaFooter          hi_AreaHeader   + 1

//Entry field #defines (ARCHSETUP)
#define id_ArchPath            0
#define id_Options             id_ArchPath + 1
#define id_Enabled             id_Options  + 1
#define id_Swap                id_Enabled  + 1

//Help index #defines
#define hi_ArchPath            1
#define hi_Options             hi_ArchPath + 1
#define hi_Enabled             hi_Options  + 1
#define hi_Swap                hi_Enabled  + 1

#endif /* INCLUDED_LZS_CMDS_H */
