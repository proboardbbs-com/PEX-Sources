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
#include "geometry.h"
#include "terminal.h"
#include <stdio.h>
#include <opwindow.h>
#include <opcrt.h>
#include <opcolor.h>

extern ColorSet   lz_Colors;
extern boolean    lz_Dirty;

#define ATTR_HIGH 0x1E
#define ATTR_NORM 0x19
#define BACK_HIGH 0x1F
#define BACK_NORM 0x01
#define CHAR_HIGH '²'
#define CHAR_NORM '°'

static char      *StatusHelp =
	"\x02    F2\x01 Save    \x02 Alt-X \x01Quit    \x02 \032 \x01Move  "
	"\x02   Shift-\032 \x01Resize    \x02 Tab \01Switch    ";
static FlexAttrs  FlexColors = { 0x74, 0x70, 0x74 };

/*
 * display the background ansi/avatar/ascii/pcboard/wildcat file
*/
static void
DisplayFile(char *fileName)
{
	zTerminal       terminal;
	ansi_interp     ansi;
	avatar_interp   avatar;
	pcboard_interp  pcboard;
	FILE           *fp;

	terminal.clrScr();

	if( 0 != (fp = fopen(fileName, "rb")) )
	{
		uchar ch;

		terminal.RegisterHandler(&avatar);
		terminal.RegisterHandler(&pcboard);
		terminal.RegisterHandler(&ansi);

		setvbuf(fp, NULL, 0x4000, _IOFBF);
		ch = fgetc(fp);
		while( !feof(fp) )
		{
			terminal.handle(ch);
			ch = fgetc(fp);
		}
		fclose(fp);

		terminal.flush();
	}
}

static void
SwitchCurrent(StackWindow &prev, StackWindow &next, char *text)
{
	prev.SetTextAttr(BACK_NORM, 0x07);
	prev.SetBackChar(CHAR_NORM);
	prev.Clear();
	next.Select();
	next.SetTextAttr(BACK_HIGH, 0x07);
	next.SetBackChar(CHAR_HIGH);
	next.Clear();
	next.wFastCenter(text, 1, ATTR_HIGH);
}

/*
 * edit several rectangles against a background file 'fileName'
*/
void
EditScreen(char *fileName, int nRects, zRect* r[], char *names[])
{
	StackWindow *win;
	byte         x1, x2, y1, y2;
	long         options = wClear|wResizeable|wUserContents;

	if( nRects > 1 && 0 != (win = new StackWindow [nRects]) )
	{
		int         i, curWin = 0;
		void far   *buf;

		SaveWindow(1, 1, ScreenWidth, ScreenHeight, TRUE, buf);

		for( i = 0; i < nRects; ++i )
		{
			x1 = r[i]->a.x; y1 = r[i]->a.y;
			x2 = r[i]->b.x; y2 = r[i]->b.y;

			win[i].InitCustom(x1, y1, x2, y2, lz_Colors, options);
			win[i].SetPosLimits(1, 1, ScreenWidth - 1, ScreenHeight - 1);
			win[i].SetSizeLimits(1, 1, ScreenWidth - 1, ScreenHeight - 1);
			win[i].SetCursor(cuHidden);
		}

		HiddenCursor();
		DisplayFile(fileName);
		for( i = 0; i < nRects; ++i )
		{
			win[i].Select();
			win[i].Draw();
			win[i].SetTextAttr(BACK_NORM, 0x07);
			win[i].SetBackChar(CHAR_NORM);
			win[i].Clear();
		}

		// select the first window
		win[curWin].Select();
		win[curWin].SetTextAttr(BACK_HIGH, 0x07);
		win[curWin].SetBackChar(CHAR_HIGH);
		win[curWin].Clear();
		win[curWin].wFastCenter(names[curWin], 1, ATTR_HIGH);

		// draw the help info on line 25
		FlexWrite(StatusHelp, ScreenHeight, 1, FlexColors);

		// process user commands and resize/move windows
		for( ;; )
		{
			int prevWin = curWin;

			switch( ReadKeyWord() )
			{
				case 0x2D00:
					goto DoneSelections;  // Alt+X

				case 0x0F09: // Tab
					curWin = (curWin + 1) % nRects;
					SwitchCurrent(win[prevWin], win[curWin], names[curWin]);
					break;

				case 0x0F00: // Shift+Tab
					curWin = (0 == curWin) ? nRects - 1 : curWin - 1;
					SwitchCurrent(win[prevWin], win[curWin], names[curWin]);
					break;

				case 0x3C00: // F2
					for( i = 0; i < nRects; ++i )
					{
						win[i].Coordinates(x1, y1, x2, y2);
						r[i]->a.x = x1; r[i]->a.y = y1;
						r[i]->b.x = x2; r[i]->b.y = y2;
						lz_Dirty = TRUE;
					}
					goto DoneSelections;

				case 0x4B00: // Shift+Left and Left
					if( KbdFlags() & 3 ) win[curWin].ResizeWindow(-1,0);
					else win[curWin].MoveWindow(-1, 0);
                    win[curWin].wFastCenter(names[curWin], 1, ATTR_HIGH);
					break;

				case 0x4D00: // Shift+Right and Right
					if( KbdFlags() & 3 ) win[curWin].ResizeWindow(+1,0);
					else win[curWin].MoveWindow(+1, 0);
					win[curWin].wFastCenter(names[curWin], 1, ATTR_HIGH);
					break;

				case 0x4800: // Shift+Up and Up
					if( KbdFlags() & 3 ) win[curWin].ResizeWindow(0,-1);
					else win[curWin].MoveWindow(0, -1);
					win[curWin].wFastCenter(names[curWin], 1, ATTR_HIGH);
					break;

				case 0x5000: // Shift+Down and Down
					if( KbdFlags() & 3 ) win[curWin].ResizeWindow(0,+1);
					else win[curWin].MoveWindow(0, +1);
					win[curWin].wFastCenter(names[curWin], 1, ATTR_HIGH);
					break;
			}
		}

	DoneSelections:
		// screen cleanup
		for( i = 0; i < nRects; ++i )
		{
			win[i].Select();
			win[i].Erase();
			win[i].Done();
		}
		delete[] win;

		RestoreWindow(1, 1, ScreenWidth, ScreenHeight, TRUE, buf);
	}
}
