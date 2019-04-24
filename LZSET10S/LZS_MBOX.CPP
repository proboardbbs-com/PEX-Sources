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
#include <opcrt.h>
#include <opwindow.h>
#include <opinline.h>
#include <opsedit.h>
#include <string.h>
#include "str.h"

extern FrameArray lz_FrameType;

// display text in a messagebox. 'text' can contain '\r' characters which
// will split it in separate lines
void
MsgBox(char *header, char *text, ColorSet &colors)
{
	RawWindow   dlg;
	byte        left, right, top, bottom;
	size_t      width, nlines;
	long        options = wBordered | wClear | wUserContents;
	char       *p, buf[255], center[255];
	CursorType  cursor = ClassifyCursorType();

	// get the maximum width required
	strcpy(buf, text);
	nlines = 0;
	width = strlen(header) + 2;
	for( p = strtok(buf, "\r"); p; p = strtok(NULL, "\r") )
	{
		width = MaxWord(width, strlen(p));
		nlines++;
	}

	nlines += 2;  // leave 2 lines on top and bottom of message
	left    = (ScreenWidth - width) / 2;
	right   = left + width + 2;
	top     = (ScreenHeight - nlines) / 2;
	bottom  = top + nlines - 1;

	if( dlg.InitCustom(left, top, right, bottom, colors, options) )
	{
		dlg.wFrame.SetFrameType(lz_FrameType);
		dlg.wFrame.AddHeader(header, heTR);
		dlg.wFrame.AddShadow(shBR, shSeeThru);
		dlg.EnableExplosions(15);
		dlg.Draw();
		HiddenCursor();

		strcpy(buf, text);
		top = 2;
		for( p = strtok(buf, "\r"); p; p = strtok(NULL, "\r") )
		{
			dlg.wFastCenter(p, top++, colors.TextColor);
		}

		for( ;; )
		{
			char ch = Upcase( Lo(SimpEditCommands.cpGetKey()) );

			if( ' ' == ch || 27 == ch || 13 == ch ) break;
		}

		dlg.Erase();
		dlg.Done();
		SetCursorType(cursor);
	}
}
