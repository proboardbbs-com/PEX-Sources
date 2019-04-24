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
#ifndef INCLUDED_FORTUNE_H
#define INCLUDED_FORTUNE_H
#include "typedef.h"

#ifndef PB_SDK
	#include <stdio.h>
	#include <conio.h>
#else
	#include "pblibc.h"
	#include "pblsdk.h"
#endif

class fortune
{
public:
	fortune(int argc, char *argv[]);
	~fortune();

	Boolean ok() const;
	void    run();
	Boolean mustHangup() const;

private:
	enum bit_flags
	{
		wait_enter   = 0x0001,  // wait for enter after display
		clear_before = 0x0002,  // clear screen before display
		clear_after  = 0x0004,  // clear screen after display
		only_once    = 0x0008,  // show one fortune only
		log_action   = 0x0010,  // log action to PROBOARD.LOG
		userdoes     = 0x0020,  // create userdoes drop file
		doing        = 0x0040,  // create doing drop file
		logoff       = 0x0080,  // logoff when done
		fancy        = 0x0100,  // fancy clear screen, etc
		no_bragging  = 0x0200,  // don't display copyrights, etc.
		show_intro   = 0x0400,  // show leading text before quote
		error        = 0x1000,  // error initializing the fortune cookie
	};

	enum color_types
	{
		upper_color = 0,
		lower_color = 1,
		digit_color = 2,
		punct_color = 3,
		intro_color = 4,
	};

	FILE  *m_datafile;  // data file pointer (tied to the index)
	int    m_ihandle;   // handle for the index file
	long   m_itemno;    // item number in the index
	long   m_origno;    // original item number in header
	long   m_items;     // total number of items in the table
	int    m_flags;     // bitmapped flags (see bit_flags)
	int    m_delay;     // delay when no enter is requested
	uchar  m_colors[5]; // color attributes
	uchar  m_curcolor;  // current color attribute
	char  *m_introtext; // introduction text to show before cookie

	static const char *s_AppName;
	static const char *s_DefaultIntroText;

	void ClearScreen(Boolean fancy);
	void ShowCopyright();
	void DisplayFortune();
	void ChangeColor(color_types color);
	void LoadConfiguration();
	void ParseText(char *text);
};

inline
Boolean
fortune::ok() const
{
	return Boolean(!(m_flags & error));
}

inline
Boolean
fortune::mustHangup() const
{
	return Boolean( m_flags & logoff );
}

inline
void
fortune::ChangeColor(color_types which)
{
	if( m_curcolor != m_colors[which] )
	{
		m_curcolor = m_colors[which];
		textattr(m_curcolor);
	}
}


#endif /* INCLUDED_FORTUNE_H */