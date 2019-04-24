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
#ifndef INCLUDED_OPENTRY_H
#define INCLUDED_OPENTRY_H
#include <opentry.h>
#endif

#define hiFileName 0x2100
extern FrameArray lz_FrameType;

boolean
AskFileName(char *header, char *fileName, ColorSet &colors)
{
	boolean     retval = FALSE;
	EntryScreen dialog;
	long        options = wBordered+wClear+wUserContents;
	char        pic[255];
	byte        width = 50;
	byte        row = ScreenHeight / 2;
	byte        col = (ScreenWidth - width) / 2;

	if( dialog.InitCustom(col, row, col+width, row + 2, colors, options) )
	{
		dialog.wFrame.SetFrameType(lz_FrameType);
		dialog.EnableExplosions(15);
		dialog.wFrame.AddShadow(shBR, shSeeThru);
		dialog.wFrame.AddHeader(header, heTR);
		dialog.esFieldOptionsOn(efCursorToEnd+efClearFirstChar+
			efForceUpper+sefNoFieldMovement);
		dialog.esFieldOptionsOff(efAutoAdvance+efInsertPushes+
			efShowReadChar+efDefaultAccepted);
		dialog.SetWrapMode(ExitAtBot);

		CharStr('!', 79, pic);
		dialog.AddStringField("", 2, 3, pic, 2, 3, 47, hiFileName, fileName);

		if( !dialog.RawError() )
		{
			dialog.Draw();
			dialog.Process();
			dialog.Erase();
			dialog.Done();
			retval = TRUE;
		}
	}
	return retval;
}
