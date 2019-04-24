/*
 * This file is part of the TurboVision Gadgets++ v2.0 Library
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
#include "tmframe.h"

#ifndef INCLUDED_STDIO_H
#define INCLUDED_STDIO_H
#include <stdio.h>
#endif

static const char *dayNames[7] = {"Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"};
static const char *header =
"’ÕÕ—00—01—02—03—04—05—06—07—08—09—10—11—12—13—14—15—16—17—18—19—20—21—22—23∏";
static const char *footer =
"‘ÕÕœ00œ01œ02œ03œ04œ05œ06œ07œ08œ09œ10œ11œ12œ13œ14œ15œ16œ17œ18œ19œ20œ21œ22œ23æ";

////////////////////////////////////////////////////////////////////////////
// the interior of the timeframe dialog
////////////////////////////////////////////////////////////////////////////
TFrameInterior::TFrameInterior(TRect &r):
	TView(r),
    col(0),
    row(0)
{
	setAll(False);
    setCell(False);
}

void
TFrameInterior::setAll(Boolean off)
{
	short ch = 1;
	if( off ) ch = 0;
	for( short i = 0; i < tfHeight; ++i )
		for( short j = 0; j < tfWidth; ++j )
			timeFrame[i][j] = ch;
}

void
TFrameInterior::draw()
{
	TDrawBuffer b;
	char        buf[256];

	// write the upper double frame
	b.moveStr(0, header, 15);
	writeLine(0, 0, size.x, 1, b);	// write the header
	for( short i = 0; i < tfHeight; ++i )
	{
		short ofs = 3;
		sprintf(buf, "≥%s", dayNames[i]);		// lead in (day of week)
		b.moveStr(0, buf, 15);
		for( short j = 0; j < tfWidth; ++j, ++ofs )
		{
			char  ch  = '˛';
			if( 0 == (j % 2) ) b.moveChar(ofs++, '≥', 15, 1);
			if( 0 == timeFrame[i][j] ) ch = '˙';
			b.moveChar(ofs, ch, 14, 1);	// first half hour
		}
		b.moveChar(75, '≥', 15, 1);
		writeLine(0, i+1, size.x, 1, b);
	}
	b.moveStr(0, footer, 15);	// write the footer (repeat if necessary)
	writeLine(0, tfHeight + 1, size.x, size.y, b);
	setCell(False);	// set the selection cursor
}

// this also adjusts the mapping for the horizontal!!!
void
TFrameInterior::setCell( Boolean reset )	// set the selection cursor
{
	TDrawBuffer b;
	char ch = '˛';
	short color = 0x30;
	short numToSkip = col / 2;
	short realCol = col + numToSkip + 4;

	if( 0 == timeFrame[row][col] ) ch = '˙';
	if( True == reset ) color = 0x0e;
	b.moveChar(0, ch, color, 1);
	writeLine(realCol, 1+row, 1, 1, b);
}

void
TFrameInterior::handleEvent(TEvent &event)
{
	TView::handleEvent(event);
	if( evMouseDown == event.what )
	{
		TPoint where = makeLocal(event.mouse.where);
		if( (where.x > 3) && (0 != (where.x % 3)) &&
			(0 < where.y) && (where.y <= tfHeight) )
		{
			short tcol = 2 * where.x - 8;	// see setCell() for formula
			short adjust = (tcol % 3) ? 1 : 0;
			tcol /= 3;
			if( tcol < tfWidth )
			{
				setCell(True);
				col = tcol + adjust;
				row = where.y - 1;
				timeFrame[row][col] = !timeFrame[row][col];
				setCell(False);
			}
		}
		clearEvent(event);
	}
	else if( evKeyboard == event.what )
	{
		if( ' ' == event.keyDown.charScan.charCode )
		{
			timeFrame[row][col] = !timeFrame[row][col];
			event.keyDown.keyCode = kbRight;
		}
		setCell(True);
		switch( event.keyDown.keyCode )
		{
			case kbLeft    : if( col ) col--; else col = tfWidth - 1; break;
			case kbRight   : col = (col + 1) % tfWidth; break;
			case kbUp      : if( row ) row--; else row = tfHeight - 1; break;
			case kbDown    : row = (row + 1) % tfHeight; break;
			case kbHome    : col = 0; break;
			case kbEnd     : col = tfWidth - 1; break;
			case kbPgUp    : row = 0; break;
			case kbPgDn    : row = tfHeight - 1; break;
			case kbCtrlHome: row = col = 0; break;
			case kbCtrlEnd : row = tfHeight - 1; col = 0; break;
			case kbCtrlPgUp: row = 0; col = tfWidth - 1; break;
			case kbCtrlPgDn: row = tfHeight - 1; col = tfWidth - 1; break;
			case kbAltE    : // enable all
				setAll(False);
				drawView();
				break;
			case kbAltD    : // disble all
				setAll(True);
				drawView();
				break;
			case kbEnter   :
				event.what = evCommand;
				event.message.command = cmOK;
				putEvent(event);
				break;
			case kbEsc:
				event.what = evCommand;
				event.message.command = cmCancel;
				putEvent(event);
				break;
			default:
				setCell(False);
				return;
		}
		setCell(False);
		clearEvent(event);
	}
}


////////////////////////////////////////////////////////////////////////////
// this is the dialog that holds the interior
////////////////////////////////////////////////////////////////////////////
TTimeFrame::TTimeFrame(const char *aTitle):
	TDialog(TRect(0,0,78,11), aTitle),
    TWindowInit(TTimeFrame::initFrame)
{
	options |= ofCentered;
	TRect r = getExtent();
    r.grow(-1, -1);
	tframe = new TFrameInterior(r);
    insert(tframe);
    tframe->select();
}

void
TTimeFrame::getTimeFrame(TimeFrame tf)
{
	for( short i = 0; i < tfHeight; ++i )
	{
		memset(tf[i], 0, 6);
		for( short j = 0; j < tfWidth; ++j )
		{
			if( 0 != tframe->timeFrame[i][j] )
				tf[i][j>>3] |= (0x80 >> (j & 7));
		}
	}
}

void
TTimeFrame::setTimeFrame(TimeFrame tf)
{
	tframe->setAll(True);
	for( short i = 0; i < tfHeight; ++i )
	{
		for( short j = 0; j < tfWidth; ++j )
		{
			if( tf[i][j>>3] & (0x80 >> (j & 7)) )
				tframe->timeFrame[i][j] = 1;
		}
	}
}

void
TTimeFrame::getData(void *rec)
{
	getTimeFrame(*(TimeFrame *)rec);
}

void
TTimeFrame::setData(void *rec)
{
	setTimeFrame(*(TimeFrame *)rec);
}

ushort
TTimeFrame::dataSize()
{
	return sizeof(TimeFrame);
}
