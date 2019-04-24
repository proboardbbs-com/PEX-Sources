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

#ifndef INCLUDED_OPINLINE_H
#define INCLUDED_OPINLINE_H
#include <opinline.h>
#endif

#ifndef INCLUDED_OPCRT_H
#define INCLUDED_OPCRT_H
#include <opcrt.h>
#endif

#ifndef INCLUDED_OPSEDIT_H
#define INCLUDED_OPSEDIT_H
#include <opsedit.h>
#endif

#ifndef INCLUDED_STRING_H
#define INCLUDED_STRING_H
#include <string.h>
#endif

#ifndef INCLUDED_STDIO_H
#define INCLUDED_STDIO_H
#include <stdio.h>
#endif

extern FrameArray lz_FrameType;

boolean
MsgAsk( char *aHeader, char *aText, boolean *escaped,
		ColorSet &colors, boolean Default = TRUE )
{
	byte        right, left, row;
	boolean     retval  = FALSE;
	size_t      width   = MaxWord(strlen(aHeader), strlen(aText)) + 8;
	long        options = wBordered+wClear+wUserContents+wCoversOnDemand;
	CursorType  cursor;
	char        text[255], header[255], response[4];
	StackWindow Popup;

	*escaped = TRUE;

	strcpy(response, "y/n");
	if( Default ) response[0] = 'Y';
	else response[2] = 'N';

	sprintf(header, " %s ", aHeader);
	sprintf(text, " %s (%s) ", aText, response);

	left  = (ScreenWidth - width) / 2 + 1;
	right = left + width - 1;
	row   = ScreenHeight / 2 - 1;

	if( Popup.InitCustom(left, row, right, row + 2, colors, options) )
	{
		boolean done = FALSE;

		cursor = ClassifyCursorType();
		Popup.wFrame.SetFrameType(lz_FrameType);
		Popup.wFrame.AddHeader(header, heTR);
		Popup.wFrame.AddShadow(shBR, shSeeThru);
		Popup.EnableExplosions(15);
		Popup.Draw();
		HiddenCursor();
		Popup.wFastWrite(text, 2, 1, colors.TextColor);
		*escaped = FALSE;

		for( ; !done; )
		{
			done = TRUE;

			switch( Upcase(Lo(SimpEditCommands.cpGetKey())) )
			{
				case 13 : retval = Default; break;
				case 27 : *escaped = TRUE; break;
				case 'Y': retval = TRUE; break;
				case 'N': retval = FALSE; break;
				default : done = FALSE;
			}
		}

		SetCursorType(cursor);
		Popup.Erase();
		Popup.Done();
	}

FunctionExit:
	return retval;
}
