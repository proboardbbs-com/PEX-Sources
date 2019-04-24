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

#include "listrec.h"
#include "toptagz.h"
#include "strlib.h"
#ifndef PB_SDK
	#include <stdio.h>
#endif

// 79 - 14 (bar) - 4 (lead-in) - 8 (# dls) = 53
#define MAX_TEXTSZ 53

extern ttConfig cfg;

///////////////////////////////////////////////////////////////////////////
// the top file object drawing routines
///////////////////////////////////////////////////////////////////////////
TTopFile::TTopFile(short aNum, TTopTag &aFile):
	TBaseRecord(aNum, uaNormal, 1)
{
	memcpy(&file, &aFile, sizeof(file));
    strrtrm(file.desc);
}

// displays at the current cursor position, which WILL change after the call
void TTopFile::draw()
{
	char  line[MAX_TEXTSZ + 1];
    short slen = strlen(file.desc);

    memset(line, cfg.fillChar, sizeof(line));
	line[sizeof(line) - 1] = EOS;
    deselect();

    SetColor(cfg.fillColor); PutChar(cfg.fillChar);
    if( tagged ){
    	SetColor(cfg.tagColor);
        printf("%c%c", cfg.tagChar, cfg.tagChar);
        SetColor(cfg.fillColor);
	}
    else printf("%c%c", cfg.fillChar, cfg.fillChar);
    PutChar(cfg.fillChar);

    if( slen > MAX_TEXTSZ ) slen = MAX_TEXTSZ;
    memcpy(line, file.desc, slen); line[slen] = EOS;
	SetColor(cfg.descColor); printf(line);
    if( slen < MAX_TEXTSZ ){
	    SetColor(cfg.fillColor);
		printf(&line[slen+1]);
    }
    SetColor(cfg.delimColor); PutChar('(');
	SetColor(cfg.numColor); printf("%3u", file.num);
    SetColor(cfg.dlsColor); printf(" dls");
	SetColor(cfg.delimColor); PutChar(')');
}

void TTopFile::select()
{
	SetColor(cfg.highColor);
	printf("[%-12s]", file.name);
}

void TTopFile::deselect()
{
	SetColor(cfg.delimColor); PutChar('[');
    SetColor(cfg.normColor); printf("%-12s", file.name);
    SetColor(cfg.delimColor); PutChar(']');
}

void TTopFile::toggleTag()
{
	tagged = Boolean(!tagged);
}
