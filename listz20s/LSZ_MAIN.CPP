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
#include "lsz_appl.h"
#include "utils.h"

#ifndef PB_SDK
	#include "_pbinit.h"
	#include <conio.h>
	#include <dos.h>
#else
	#include "proboard.h"
	#include "pblsdk.h"
#endif

Application *application;

static void
ClearScreen()
{
	textattr(7);
	for( int i = CurUser->screenLength; i > 0; --i)
	{
		gotoxy(1, i);
		clreol();
		delay(80);
	}
}

int
main(int, char *argv[])
{
#ifndef PB_SDK
	proboard_emulator_init();
#else
	// check if user has graphics emulation ON (ANSi/AVT)
	if( !pb_isgfx() )
	{
		Log(LOG_FRIEND, "%s: %s does not have graphics capability.",
			Application::s_AppName, CurUser->name);
		return 1;
	}
#endif

	setCursorType(CURSOR_HIDDEN);
	if( 0 != (application = new Application(argv[0])) )
	{
		application->run();
		delete application;
	}
	setCursorType(CURSOR_NORMAL);

	// brag screen local only or high bauds
	if( 0L == BaudRate || BaudRate >= 9600L )
	{
		char  msg[255];
		char *msg2 = "returning to the board...";
		char *ch   = "þ";

		ClearScreen();
		sprintf(msg, "Listerz made by gargoyle for %s", Config->bbsname);

		effect_fade(ch, 1, 2);
		textattr(3); gotoxy(1, 2); putch(ch[0]);
		effect_fade(msg, 3, 2);
		textattr(3); gotoxy(3, 2); cprintf(msg);
		effect_fade(ch, 1, 3);
		textattr(15); gotoxy(1, 3); putch(ch[0]);
		effect_fade(msg2, 3, 3);
		textattr(15); gotoxy(3, 3); cprintf(msg2);
		delay(1000);
	}

#ifndef PB_SDK
	proboard_emulator_term();
#endif

	return 0;
}
