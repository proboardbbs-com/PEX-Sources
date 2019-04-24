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
#include "taglist.h"
#include "bitvect.h"

#ifndef INCLUDED_STRING_H
#define INCLUDED_STRING_H
#include <string.h>
#endif

#ifndef INCLUDED_STDIO_H
#define INCLUDED_STDIO_H
#include <stdio.h>
#endif

#ifndef INCLUDED_DOS_H
#define INCLUDED_DOS_H
#include <dos.h>
#endif

////////////////////////////////////////////////////////////////////////////
// The TTagListBox class
////////////////////////////////////////////////////////////////////////////
TTagListBox::TTagListBox(const TRect &r, TScrollBar *sb, ushort maxSize):
	TListBox(r, 1, sb)
{
	bitmap = new zBitVector(maxSize);
	bitmap->clearAll();
	rangeStart = 0;
	tagChar = 'X';
}

void
TTagListBox::getText(char *dest, short item, short maxLen)
{
	static char buf[256];

	sprintf(buf, "[ ] %s", ((TTagEntry *)list()->at(item))->getText());
	if( bitmap->has(item) ) buf[1] = tagChar;
	strncpy(dest, buf, maxLen);
	dest[maxLen] = EOS;
}

void
TTagListBox::handleEvent(TEvent &event)
{
	ushort far *shiftState = (ushort *)MK_FP(0x40, 0x17);
	short prevItem, newItem, skipCount = 0;
	short mouseAutosToSkip = 4;

    // intercept the right button
	if( (evMouseDown & event.what) && (event.mouse.buttons & mbRightButton) )
	{
    	prevItem = focused;
        TPoint mouse = makeLocal(event.mouse.where);
        newItem = mouse.y + topItem;
		do
		{
			if( newItem != prevItem )
			{
				void *temp = list()->at(prevItem);
				// adjust the tags
				Boolean tagState = bitmap->has(prevItem);
				if( bitmap->has(newItem) ) bitmap->set(prevItem);
				else bitmap->reset(prevItem);
				if( tagState ) bitmap->set(newItem);
				else bitmap->reset(newItem);
				// swap the items in the collection
				list()->atPut(prevItem, list()->at(newItem));
				list()->atPut(newItem, temp);
				focusItemNum( newItem );
			}
			prevItem = newItem;
			mouse = makeLocal(event.mouse.where);
			if( mouseInView(event.mouse.where) )
			{
				newItem = mouse.y + topItem;
			}
			else
			{
				if( event.what == evMouseAuto ) skipCount++;
				if( skipCount == mouseAutosToSkip )
				{
					skipCount = 0;
					if( mouse.y < 0  && focused > 0 )
						newItem = focused - 1;
					else if( mouse.y >= size.y && focused < (range - 1) )
						newItem = focused + 1;
				}
			}
		}while( mouseEvent(event, evMouseMove | evMouseAuto) );
		focusItemNum(newItem);
		clearEvent(event);
		return;
	}

	// intercept the mouse clicks
	if( evMouseDown & event.what )
	{
		if( event.mouse.doubleClick )
		{ // double click
			TListBox::handleEvent(event);
			message(owner, evBroadcast, cmTagFocused, this);
		}
		else if( kbCtrlShift & *shiftState )
		{ // control + click
			TListBox::handleEvent(event);
			ToggleTag(focused);
		}
		else if( (kbLeftShift | kbRightShift) & *shiftState )
		{ // shift
			TListBox::handleEvent(event);
			RemoveAllTags();
			short firstRange = min(rangeStart, focused);
			short lastRange = max(rangeStart, focused);
			for( short i = firstRange; i <= lastRange; ++i ) SetTag(i);
		}
		else TListBox::handleEvent(event);

		drawView();
		clearEvent(event);
		return;
	}

	// handle the space here
	if( evKeyboard == event.what && ' ' == event.keyDown.charScan.charCode )
	{
		ToggleTag(focused);
		event.keyDown.keyCode = kbDown;
	}

	TListBox::handleEvent(event);

	if( evKeyboard == event.what )
	{
		switch( event.keyDown.keyCode )
		{
			case kbEnter: ToggleTag(focused); break;
			case kbF5   : SetAllTags();       break;
			case kbF6   : RemoveAllTags();    break;
			case kbF7   : ToggleAllTags();    break;
			default     : return;
		}
		clearEvent(event);
		drawView();
	}
}

void
TTagListBox::ToggleTag(short item)
{
	if( range )
	{
		if( bitmap->has(item) ) bitmap->reset(item);
		else bitmap->set(item);
		rangeStart = item;
	}
}

void
TTagListBox::SetTag(short item)
{
	if( range ) bitmap->set(item);
}

void
TTagListBox::SetAllTags()
{
	for( short i = 0; i < range; ++i ) bitmap->set(i);
}

void
TTagListBox::RemoveAllTags()
{
	for( short i = 0; i < range; ++i ) bitmap->reset(i);
}

void
TTagListBox::ToggleAllTags()
{
	for( short i = 0; i < range; ++i )
	{
		if( bitmap->has(i) ) bitmap->reset(i);
		else bitmap->set(i);
	}
}

Boolean
TTagListBox::IsTagged(short item)
{
	return bitmap->has(item);
}

void
TTagListBox::DeleteTag(short item)
{
	for( int i = item; i < list()->getCount() - 1; ++i )
	{
		if( bitmap->has(i + 1) ) bitmap->set(i);
		else bitmap->reset(i);
	}
}

void
TTagListBox::InsertTag(short item)
{
	for( int i = list()->getCount(); i > item; ++i )
	{
		if( bitmap->has(i - 1) ) bitmap->set(i);
		else bitmap->reset(i);
	}
}

///////////////////////////////////////////////////////////////////////////
// the source for the draw() routine is taken from the original draw()
// for TListViewer. i have modified it to highlight the tagged items too.
///////////////////////////////////////////////////////////////////////////
void
TTagListBox::draw()
{
	short i, j, item;
	ushort normalColor, selectedColor, focusedColor, color;
	short colWidth, curCol, indent;
	TDrawBuffer b;
	uchar scOff;

	if( (state&(sfSelected | sfActive)) == (sfSelected | sfActive) )
	{
		normalColor = getColor(1);
		focusedColor = getColor(3);
		selectedColor = getColor(4);
	}
	else
	{
		normalColor = getColor(2);
		selectedColor = getColor(4);
	}

	if( hScrollBar != 0 ) indent = hScrollBar->value;
	else indent = 0;

	colWidth = size.x / numCols + 1;
	for( i = 0; i < size.y; i++ )
	{
		for( j = 0; j < numCols; j++ )
		{
			item =  j * size.y + i + topItem;
			curCol = j * colWidth;
			if( (state & (sfSelected|sfActive)) == (sfSelected|sfActive) &&
				focused == item && range > 0 )
			{
				color = focusedColor;
				setCursor( curCol + 1, i );
				scOff = 0;
			}
			else if( item < range && (isSelected(item) || IsTagged(item)) )
			{
				color = selectedColor;
				scOff = 2;
			}
			else
			{
				color = normalColor;
				scOff = 4;
			}

			b.moveChar( curCol, ' ', color, colWidth );
			if( item < range )
			{
				char text[256];
				getText( text, item, colWidth + indent );
				char buf[256];
				memmove( buf, text+indent, colWidth );
				buf[colWidth] = EOS;
				b.moveStr( curCol+1, buf, color );
				if( showMarkers )
				{
					b.putChar( curCol, specialChars[scOff] );
					b.putChar( curCol+colWidth-2, specialChars[scOff+1] );
				}
			}
			else if( i == 0 && j == 0 )
				b.moveStr( curCol+1, "<empty>", getColor(1) );

			b.moveChar( curCol+colWidth-1, 179, getColor(5), 1 );
		}
		writeLine( 0, i, size.x, 1, b );
	}
}
