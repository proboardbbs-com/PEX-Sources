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
#include "flagsel.h"

#ifndef INCLUDED_CTYPE_H
#define INCLUDED_CTYPE_H
#include <ctype.h>
#endif

#ifndef INCLUDED_STRING_H
#define INCLUDED_STRING_H
#include <string.h>
#endif

static const ushort cmFlagSelected = 0x4701;

////////////////////////////////////////////////////////////////////////////
// The interior TFlagView class implementation
////////////////////////////////////////////////////////////////////////////
TFlagView::TFlagView(const TPoint &origin, ulong aFlags, ulong aFlagsRev):
	TView( TRect(origin.x, origin.y, origin.x + 34, origin.y + 1) ),
    flags(aFlags),
    flagsRev(aFlagsRev)
{
	options |= ofSelectable | ofFirstClick;

	// make sure we don't have bits that are set in both flags
	for( short i = 0; i < 32; ++i )
	{
		ulong bitMask = 1L << i;

		// if bit is set in both flags, unset it in both flags
		if( (flags & bitMask) && (flagsRev & bitMask) )
		{
			flags &= ~bitMask;
			flagsRev &= ~bitMask;
		}
	}
}

// Bit-mapped set: A..Z1..6 (from 31-0 bit)
void TFlagView::draw()
{
	TDrawBuffer  buf;
	char        *s = " תתתתתתתתתתתתתתתתתתתתתתתתתתתתתתתת ";
	ushort       color = 0x1e;   // bright yellow on black
	ushort       flagColor = 0x1f, flagRevColor = 0x2f;

	buf.moveChar(0, ' ', color, size.x);
	buf.moveStr(0, s, color);
	for( short i = 0; i < 32; ++i )
	{
		char   aChar = (i < 6) ? '6' - i : 'Z' - (i - 6);

		if( flags & (1L << i) )
		{ // this flag is on
			buf.moveChar(32-i, aChar, flagColor, 1);
		}
		else if( flagsRev & (1L << i) )
		{ // this flag is reversed
			buf.moveChar(32-i, aChar, flagRevColor, 1);
		}
	}
	writeLine(0, 0, size.x, size.y, buf);
}

void TFlagView::handleEvent(TEvent &event)
{
	TView::handleEvent(event);

	if( (state & sfSelected) == 0 ) return;

	if( evKeyboard == event.what )
	{
		switch( event.keyDown.keyCode )
		{
			case kbEnter:
				message(owner, evBroadcast, cmFlagSelected, this);
				break;
			case kbAltS:
				flags = 0xFFFFFFFFL;
				flagsRev = 0L;
				break;
			case kbAltC:
				flags = 0L;
				flagsRev = 0L;
				break;
			case kbAltR:
				flags = 0L;
				flagsRev = 0xFFFFFFFFL;
				break;
			default:
				char aChar = toupper(event.keyDown.charScan.charCode);
				if( ' ' > aChar ) return;	// a tab?
				long bitMask;

				if( 'A' <= aChar && 'Z' >= aChar )
					bitMask = 1L << (6 + ('Z' - aChar));
				else if('1' <= aChar && '6' >= aChar)
					bitMask = 1L << ('6' - aChar);
				else
					return;

				if( flags & bitMask )
				{ // already set, set it to reverse
					flags &= ~bitMask;
					flagsRev |= bitMask;
				}
				else if( flagsRev & bitMask ) flagsRev &= ~bitMask;
				else flags |= bitMask;
		}
		drawView();
		clearEvent(event);
	}
	else if( evMouseDown == event.what )
	{
		TPoint mouse = makeLocal(event.mouse.where);
		if( mouse.x && mouse.x <= 32 )
		{
			ulong bitMask = 1L << (32 - mouse.x);
			if( flags & bitMask )
			{
				flags &= ~bitMask;
				flagsRev |= bitMask;
			}
			else if( flagsRev & bitMask ) flagsRev &= ~bitMask;
			else flags |= bitMask;
			drawView();
		}
		clearEvent(event);
	}
}

////////////////////////////////////////////////////////////////////////////
// The main dialog TFlagSelect class implementation
////////////////////////////////////////////////////////////////////////////
TFlagSelect::TFlagSelect(ulong aFlags, ulong aFlagsRev, const char *aTitle):
	TDialog(TRect(0,0,42,7), aTitle),
	TWindowInit(TFlagSelect::initFrame)
{
	options |= ofCentered;
	TRect bounds = getExtent();
	bounds.grow(-4, -3);
	interior = new TFlagView(bounds.a, aFlags, aFlagsRev);
	interior->options |= (ofCenterX | ofFramed);
	insert(interior);
	interior->select();
}

void TFlagSelect::handleEvent(TEvent &event)
{
	TDialog::handleEvent(event);

	if( evBroadcast == event.what )
	{
		if( cmFlagSelected == event.message.command )
		{
			endModal(cmClose);
		}
	}
}

void TFlagSelect::getData(void *rec)
{
	TFlagData *p = (TFlagData *)rec;
	p->flags = interior->flags;
	p->flagsNot = interior->flagsRev;
}

ushort TFlagSelect::dataSize()
{
	return sizeof(TFlagData);
}

void TFlagSelect::setData(void *rec)
{
	TFlagData *p = (TFlagData *)rec;
	interior->flags = p->flags;
	interior->flagsRev = p->flagsNot;
}
