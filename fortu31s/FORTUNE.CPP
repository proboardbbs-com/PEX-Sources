/*
 * This file is part of Fortune Cookie v3.01.R1
 *
 * Copyright (c) 1995, 1997 by Branislav L. Slantchev
 * A fine product of Silicon Creations, Inc. (gargoyle)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the License which accompanies this
 * software. This library is distributed in the hope that it will
 * be useful, but without any warranty; without even the implied
 * warranty of merchantability or fitness for a particular purpose.
 *
 * You should have received a copy of the License along with this
 * library, in the file LICENSE.DOC; if not, write to the address
 * below to receive a copy via electronic mail.
 *
 * You can reach Branislav L. Slantchev (Silicon Creations, Inc.)
 * at bslantch@cs.angelo.edu. The file SUPPORT.DOC has the current
 * telephone numbers and the postal address for contacts.
*/
#include "fortune.h"
#include "getopt.h"
#include "comdef.h"
#include "pblsdk.h"
#include "file.h"
#include "dataset.h"
#include "utils.h"
#include "useron.h"
#include "proboard.h"
#include "terminal.h"
#include "str.h"
#include "emulator.h"
#include "profile.h"

#ifndef PB_SDK
	#include <ctype.h>
	#include <stdlib.h>
	#include <string.h>
	#include <io.h>
	#include <fcntl.h>
	#include <share.h>
	#include <dos.h>
#endif

const char* fortune::s_AppName = "Ÿortune";
const char* fortune::s_DefaultIntroText = "...and now some words of wisdom:\n\n";

#define __VERSION__ "3.01.R1"
#define TAB_TEXT    "        "

fortune::fortune(int argc, char *argv[])
	:m_flags(wait_enter|clear_before|clear_after|only_once|fancy|show_intro)
	,m_delay(1000)
	,m_ihandle(-1)
	,m_datafile(0)
	,m_introtext(0)
{
	int          opt;
	long         offset;
	char         fileName[256], indexName[256];
	index_header header;

	// preliminary setups
	file_chext(fileName, argv[0], ".DAT");
	m_colors[upper_color] = 7;
	m_colors[lower_color] = 15;
	m_colors[digit_color] = 11;
	m_colors[punct_color] = 9;

	while( EOF != (opt = getopt(argc, argv, "nbamsludh!f:p:")) )
	{
		switch( tolower(opt) )
		{
			case 'n': m_flags &= ~wait_enter;   break;
			case 'b': m_flags &= ~clear_before; break;
			case 'a': m_flags &= ~clear_after;  break;
			case 'm': m_flags &= ~only_once;    break;
			case 's': m_flags &= ~fancy;        break;
			case 'i': m_flags &= ~show_intro;   break;
			case 'l': m_flags |= log_action;    break;
			case 'u': m_flags |= userdoes;      break;
			case 'd': m_flags |= doing;         break;
			case 'h': m_flags |= logoff;        break;
			case '!': m_flags |= no_bragging;   break;
			case 'f': strcpy(fileName, optarg); break;
			case 'p': m_delay = atoi(optarg);   break;
			default :
				Log(LOG_FRIEND, "%s: Invalid option - '%s'", s_AppName, argv[optind]);
				m_flags |= error;
				goto FunctionExit;
		}
	}

	// create path to index file
	file_chext(indexName, fileName, ".IDX");
	// get the info from the index
	m_ihandle = open(indexName, O_RDWR|O_BINARY|O_DENYNONE, SH_DENYNO);
	if( -1 == m_ihandle )
	{
		m_flags |= error;
		Log(LOG_FRIEND, "%s: Cannot read index '%s'", s_AppName, indexName);
		goto FunctionExit;
	}
	offset = lseek(m_ihandle, 0L, SEEK_END) - sizeof(index_header);
	lseek(m_ihandle, offset, SEEK_SET);
	read(m_ihandle, &header, sizeof(header));
	m_items = header.entries;
	m_origno = header.last_used;
	m_itemno = (header.last_used + 1L) % m_items;

	// now try to open the data file as well
	if( 0 == (m_datafile = fopen(fileName, "rb")) )
	{
		close(m_ihandle);
		m_ihandle = -1;
		m_flags |= error;
		Log(LOG_FRIEND, "%s: Cannot open the dataset '%s'", s_AppName, fileName);
		goto FunctionExit;
	}

	// finally load the colors from SILICON.INI (if any)
	LoadConfiguration();

FunctionExit:
}

void
fortune::~fortune()
{
	if( -1 != m_ihandle )
	{
		if( 0 != _dos_lock(m_ihandle, 0L, 1L) )
		{
			Log(LOG_FRIEND, "%s: Unable to lock index for update", s_AppName);
		}
		else
		{  // we don't update if already updated (check last_used == m_origno)
			index_header header;
			long         offset;

			offset = lseek(m_ihandle, 0L, SEEK_END) - sizeof(index_header);
			lseek(m_ihandle, offset, SEEK_SET);
			read(m_ihandle, &header, sizeof(header));
			if( header.last_used == m_origno ) header.last_used = m_itemno;
			lseek(m_ihandle, offset, SEEK_SET);
			write(m_ihandle, &header, sizeof(header));
			_dos_unlock(m_ihandle, 0L, 1L);
		}
		close(m_ihandle);
	}

	if( m_datafile  ) fclose(m_datafile);
	if( m_introtext ) delete[] m_introtext;
}

void
fortune::run()
{
	zUserOnline useron(Boolean(m_flags&userdoes), Boolean(m_flags&doing));

	useron.Create(NodeNumber, "Munching cookies");
	if( m_flags & clear_before ) ClearScreen(False);
	if( m_flags & log_action ) Log(LOG_FRIEND, "%s: %s is wolfing cookies", s_AppName, CurUser->name);

	if( (m_flags & show_intro) && m_introtext )
	{
		ChangeColor(intro_color);
		for( char *p = m_introtext; EOS != *p; ++p )
		{
			if( '\t' == *p ) cprintf(TAB_TEXT);
			else putch(*p);
		}
	}

	DisplayFortune();
	if( m_flags & only_once )
	{
		if( m_flags & wait_enter ) pb_askenter();
		else delay(m_delay);
	}
	else
	{
		zTerminal       terminal;
		ansi_interp     ansi;
		proboard_interp proboard;

		terminal.RegisterHandler(&ansi);
		terminal.RegisterHandler(&proboard);
		for( ; !(m_flags & error) && pb_askmore(&terminal); )
		{
			if( m_flags & clear_before ) ClearScreen(False);
			DisplayFortune();
		}
	}

	useron.Remove(NodeNumber);
	if( m_flags & clear_after ) ClearScreen(Boolean(m_flags & fancy));
	if( !(m_flags & no_bragging) ) ShowCopyright();
}

void
fortune::DisplayFortune()
{
	index_item  item;
	char       *buf;

	lseek(m_ihandle, m_itemno * sizeof(index_item), SEEK_SET);
	read(m_ihandle, &item, sizeof(item));
	fseek(m_datafile, item.offset, SEEK_SET);
	if( 0 != (buf = new char [item.length + 1]) )
	{
		fread(buf, sizeof(char), item.length, m_datafile);
		buf[item.length] = EOS;
		for( char *p = buf; EOS != *p; ++p )
		{
			if( '\t' == *p )
			{
				cprintf(TAB_TEXT);
			}
			else
			{
				if( isdigit(*p) ) ChangeColor(digit_color);
				else if( ispunct(*p) ) ChangeColor(punct_color);
				else if( isupper(*p) ) ChangeColor(upper_color);
				else ChangeColor(lower_color);
				putch(*p);
			}
		}
		cprintf("\r\n");
		delete[] buf;
	}
	m_itemno = (m_itemno + 1L) % m_items;
}

void
fortune::ClearScreen(Boolean isFancy)
{
	if( isFancy )
	{
		textattr(7);
		for( int i = CurUser->screenLength; i > 0; --i)
		{
			gotoxy(1, i);
			clreol();
			delay(80);
		}
	}
	else clrscr();
}

void
fortune::ShowCopyright()
{
	char  msg[256];
	char *msg2 = "returning to the board...";
	char *ch   = "þ";

	sprintf(msg, "Fortune Cookie made by gargoyle for %s", Config->bbsname);
	if( m_flags & fancy )
	{
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
	else cprintf("%c %s\r\n%c %s", ch, msg2, ch, msg);
}

void
fortune::LoadConfiguration()
{
	char     buf[256];
	zProfile ini;
	char     *section = "Fortune";

	ini.Assign(pb_getpath("SILICON.INI", PBDIR_SYS));
	if( ini.Open() )
	{
		ini.ReadString(section, "LowerCase", buf, "@X0F");
		m_colors[lower_color] = hexColor(toupper(buf[2]), toupper(buf[3]));
		ini.ReadString(section, "UpperCase", buf, "@X07");
		m_colors[upper_color] = hexColor(toupper(buf[2]), toupper(buf[3]));
		ini.ReadString(section, "Numbers", buf, "@X0B");
		m_colors[digit_color] = hexColor(toupper(buf[2]), toupper(buf[3]));
		ini.ReadString(section, "Punctuation", buf, "@X09");
		m_colors[punct_color] = hexColor(toupper(buf[2]), toupper(buf[3]));
		ini.ReadString(section, "IntroColor", buf, "@X03");
		m_colors[intro_color] = hexColor(toupper(buf[2]), toupper(buf[3]));
		ini.ReadString(section, "IntroText", buf, (char*)s_DefaultIntroText);
		ParseText(buf);
		ini.Close();
	}
}

// this text string can use the @<>@ proboard macros and '\n' (newline)
void
fortune::ParseText(char *s)
{
	char buf[1024];

	strsrep(s, "\\n", "\n");
#ifdef PB_SDK
	ParseStringVars(s, buf, sizeof(buf) - 1);
#else
	strcpy(buf, s);
#endif
	m_introtext = newStr(buf);
}

////////////////////////////////////////////////////////////////////////////
// main()
////////////////////////////////////////////////////////////////////////////
void
main(int argc, char *argv[])
{
	proboard_emulator_init();

	fortune *application = new fortune(argc, argv);
	Boolean mustHangup = False;

	if( application )
	{
		if( application->ok() )
		{
			application->run();
			mustHangup = application->mustHangup();
		}
		delete application;
	}

	proboard_emulator_term();
	if( mustHangup ) HangUp();
}
