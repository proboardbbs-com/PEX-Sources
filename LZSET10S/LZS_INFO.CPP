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
#ifndef INCLUDED_OPWINDOW_H
#define INCLUDED_OPWINDOW_H
#include <opwindow.h>
#endif

#ifndef INCLUDED_OPCRT_H
#define INCLUDED_OPCRT_H
#include <opcrt.h>
#endif

#ifndef INCLUDED_OPCOLOR_H
#define INCLUDED_OPCOLOR_H
#include <opcolor.h>
#endif

#ifndef INCLUDED_STDIO_H
#define INCLUDED_STDIO_H
#include <stdio.h>
#endif

#include "lzs_vers.h"

extern FrameArray lz_FrameType;
extern ColorSet   lz_Colors;

// display information banner
void
DisplayBanner()
{
	RawWindow  info;
	long       options = wBordered+wClear+wUserContents+wCoversOnDemand;
	word       left = 6, right = 74, top = 5, bottom = 20;
	word       width = right - left + 1;
	char      *text[7] =
	{
		"                                                    ",
		"Copyright (c) 1997 Branislav L. Slantchev (gargoyle)",
		"A fine freeware product of Silicon Creations, Inc.",
		"Developed using Borland C++ 3.1 and Object Professional v1.10",
		"This release version compiled on " __DATE__ " at " __TIME__,
		"Made in the USA by illegal immigrants!",
		"THE BLOODY BULGARIAN STRIKES AGAIN!",
	};

	sprintf(text[0], "lzSetup Version %s", __VER__);
	if( info.InitCustom(left, top, right, bottom, lz_Colors, options) )
	{
		CursorType cursor = ClassifyCursorType();
		SetBlink(FALSE);
		info.wFrame.SetFrameType(lz_FrameType);
		info.wFrame.AddHeader(" Information ", heTR);
		info.EnableExplosions(20);
		info.wFrame.AddShadow(shBR, shSeeThru);
		info.Draw();
		info.wFastCenter(text[0], 2, YellowOnBlack);
		info.wFastCenter(text[1], 4, WhiteOnBlack);
		info.wFastCenter(text[2], 5, CyanOnBlack);
		info.wFastFill(width, ' ', 7, 1, 0xFF);
		info.wFastFill(width, ' ', 8, 1, BlackOnGreen);
		info.wFastCenter(text[6], 8, BlackOnGreen);
		info.wFastFill(width, ' ', 9, 1, BlackOnRed);
		info.wFastCenter(text[3], 12, CyanOnBlack);
		info.wFastCenter(text[4], 13, CyanOnBlack);
		info.wFastCenter(text[5], 15, WhiteOnBlack);
		HiddenCursor();
		ReadKey();
		info.Erase();
		info.Done();
		SetBlink(TRUE);
		SetCursorType(cursor);
	}
}
