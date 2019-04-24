/*
 * This file is part of TopTagz
 *
 * Copyright (c) 1996 by Branislav L. Slantchev
 * A Product of Silicon Creations, Inc.
 *
 * This source is distributed under the terms and conditions of the
 * GNU General Public License. A copy of the license is included with
 * this distrbiution (see the file 'Copying.Doc').
 *
 * Contact: 73023.262@compuserve.com
*/

#include "gadgets.h"
#include "toptagz.h"
#include "class/lang.h"

extern TLanguage *lang;
extern short statusPrompt;

#ifndef PB_SDK
	#include <stdio.h>
	#include <string.h>
#endif

static void _drawSep(const char *aText, short y, uchar coSep, uchar coText)
{
	char   buf[79];

	SetColor(coSep);
    GotoXY(1, y);
    memset(buf, 'Ä', sizeof(buf)); buf[sizeof(buf) - 1] = EOS;
    if( 0 == aText ) printf(buf);
    else{
	    size_t slen = strlen(aText);
        if( 70 < slen ) slen = 70;	// arbitrary choice here
        short ofs = sizeof(buf) - slen  - 4; // for the parenthesis
        sprintf(&buf[ofs], "(");
        printf(buf);
        SetColor(coText);
        ((char *)aText)[slen] = EOS; printf(aText);
        SetColor(coSep);
        printf(")ÄÄ");
	}
}

void drawTopSep(const char *aText)
{
	_drawSep(aText, 2, 1, 5);
}

void drawBotSep(const char *aText)
{
	_drawSep(aText, 23, 1, 5);
}

void drawTitle()
{
	GotoXY(1, 1);
    SetColor(9); printf("Navigator/Pro v3.0r1 ");
    SetColor(5); printf("[toptagz module] ");
    SetColor(8); printf("Copyright (c) 1996 Silicon Creations, Inc");
}

void drawStatus()
{
	GotoXY(1, 24);
    if( statusPrompt && lang ){
    	lang->display(statusPrompt);
	}
    else{
		SetColor(1); PutChar('('); SetColor(9); printf("esc"); SetColor(1);
	    printf(") done  ("); SetColor(9); printf("space"); SetColor(1);
	    printf(") toggle  ("); SetColor(9); PutChar('s'); SetColor(1);
	    printf(")set  ("); SetColor(9); PutChar('r'); SetColor(1);
	    printf(")eset  ("); SetColor(9); printf("enter"); SetColor(1);
	    printf(") get  ("); SetColor(9); PutChar('g'); SetColor(1);
	    printf(")et tagged  ("); SetColor(9); PutChar('d'); SetColor(1);
		printf(")raw");
    }
}