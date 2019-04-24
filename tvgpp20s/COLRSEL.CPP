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
#include "colrsel.h"

const cpNoColor = 0x01;  // this is actually blue on black, but ' ' == no clr

/////////////////////////////////////////////////////////////////////////////
// The TColorChart class implementation
/////////////////////////////////////////////////////////////////////////////
TColorChart::TColorChart(const TPoint &origin, uchar aColor):
	TView( TRect(origin.x, origin.y, origin.x + 18, origin.y + 10) )
{
	setColor(aColor);
    options |= (ofPreProcess | ofSelectable | ofFirstClick);
}

void
TColorChart::setColor(uchar aColor)
{
	x_fore = aColor & 0x0f;
	y_back = (aColor >> 4) & 0x07;
	blink  = aColor & 0x80;
	drawView();
}

void
TColorChart::draw()
{
	TDrawBuffer buf;
	uchar       color = cpNoColor;

	// first and last lines are empty
	buf.moveChar(0, ' ', color, size.x);
	writeLine(0, 0, size.x, 1, buf);
	writeLine(0, size.y - 1, size.x, 1, buf);

	for( short i = 0; i < 8; ++i )
	{      // the lines (background colors)
		buf.moveChar(0, ' ', color, 1);  // first character is space
		for( short j = 0; j < 16; ++j )
		{ // the columns (foreground)
			buf.moveChar(j + 1, 'X', 0, 1);
			buf.putAttribute(j + 1, (i << 4) | j);
		}
		buf.moveChar(size.x - 1, ' ', color, 1);
		writeLine(0, i + 1, size.x, 1, buf);
	}
	selSet();
}

void
TColorChart::selSet()
{
	TDrawBuffer buf;
	uchar       color = x_fore | (y_back << 4) + blink;

	// draw the upper frame
	buf.moveStr(0, "зд©", 15);
	writeLine(x_fore, y_back, 3, 1, buf);

    // draw the colored 'X' boxed
    buf.moveChar(0, 'Ё', 15, 3);
    buf.moveChar(1, 'X', color, 1);
    if( !color ) buf.putAttribute(1, 0);
    writeLine(x_fore, y_back + 1, 3, 1, buf);

    // draw the lower frame
    buf.moveStr(0, "юды", 15);
    writeLine(x_fore, y_back + 2, 3, 1, buf);

	// broadcast that new color is selected
	uchar colorValue = (x_fore | (y_back << 4)) + blink;
	message(owner, evBroadcast, cmUpdateDemo, &colorValue);
}

void
TColorChart::selReset()
{
	TDrawBuffer buf;
	uchar       color = cpNoColor;

    // restore the line above the selection, see if it's the first
    // line in the window, in which case set to default attribute
	if( !y_back )
	{
		buf.moveStr(0, "   ", color);
	}
	else
	{
		if( !x_fore )
		{
			buf.moveChar(0, ' ', color, 1);
		}
		else
		{
			buf.moveChar(0, 'X', 0, 1);
			buf.putAttribute(0, ((y_back-1) << 4) | (x_fore - 1));
		}
		buf.moveChar(1, 'X', 0, 1);
		buf.putAttribute(1, ((y_back-1) << 4) | x_fore);
		if( x_fore == 15 )
		{
			buf.moveChar(2, ' ', color, 1);
		}
		else
		{
			buf.moveChar(2, 'X', 0, 1);
			buf.putAttribute(2, ((y_back-1) << 4) | (x_fore + 1));
		}
	}
	writeLine(x_fore, y_back, 3, 1, buf);

	// now restore the three characters on the line itself
	if( !x_fore )
	{
		buf.moveChar(0, ' ', color, 1);
	}
	else
	{
		buf.moveChar(0, 'X', 0, 1);
		buf.putAttribute(0, (y_back << 4) | (x_fore - 1));
	}
	buf.moveChar(1, 'X', 0, 1);
	buf.putAttribute(1, (y_back << 4) | x_fore);
	if( 15 == x_fore )
	{
		buf.moveChar(2, ' ', color, 1);
	}
	else
	{
		buf.moveChar(2, 'X', 0, 1);
		buf.putAttribute(2, (y_back << 4) | (x_fore + 1));
	}
	writeLine(x_fore, y_back + 1, 3, 1, buf);

	// now, restore the line below the selection
	if( 7 == y_back )
	{
		buf.moveStr(0, "   ", color);
	}
	else
	{
		if( !x_fore )
		{
			buf.moveChar(0, ' ', color, 1);
		}
		else
		{
			buf.moveChar(0, 'X', 0, 1);
			buf.putAttribute(0, ((y_back + 1) << 4) | (x_fore - 1));
		}
		buf.moveChar(1, 'X', 0, 1);
		buf.putAttribute(1, ((y_back + 1) << 4) | x_fore);
		if( 15 == x_fore )
		{
			buf.moveChar(2, ' ', color, 1);
		}
		else
		{
			buf.moveChar(2, 'X', 0, 1);
			buf.putAttribute(2, ((y_back + 1) << 4) | (x_fore + 1));
		}
	}
	writeLine(x_fore, y_back + 2, 3, 1, buf);
}

void
TColorChart::handleEvent(TEvent &event)
{
	TView::handleEvent(event);

	if( False == getState(sfSelected) ) return;

	if( evMouseDown == event.what )
	{
		do
		{
			if( mouseInView(event.mouse.where) )
			{
				TPoint spot = makeLocal(event.mouse.where);
				selReset();
				x_fore = spot.x ? spot.x - 1 : 0;
				y_back = spot.y ? spot.y - 1 : 0;
				if( 15 < x_fore ) x_fore = 15;
				if(  7 < y_back ) y_back = 7;
				selSet();
			}
			if( event.mouse.doubleClick )
			{
				uchar color = ((y_back << 4) | x_fore) + blink;
				message(owner, evBroadcast, cmSelectColor, &color);
				break;
			}
		}while( mouseEvent(event, evMouseMove) );
		clearEvent(event);
	}
	else if( evKeyboard == event.what )
	{
		switch( event.keyDown.keyCode )
		{
			case kbHome : selReset(); x_fore = 0; break;
			case kbEnd  : selReset(); x_fore = 15; break;
			case kbUp   : selReset(); if( y_back ) y_back--; else y_back = 7; break;
			case kbDown : selReset(); y_back = (y_back + 1) % 8; break;
			case kbLeft : selReset(); if( x_fore ) x_fore--; else x_fore = 15; break;
			case kbRight: selReset(); x_fore = (x_fore + 1) % 16; break;
			case kbAltB : if( !blink ) blink = 128; else blink = 0; break;
			case kbPgUp : selReset(); y_back = 0; break;
			case kbPgDn : selReset(); y_back = 7; break;
			default: return;
		}
		selSet();
		clearEvent(event);
	}
}

uchar
TColorChart::getColor() const
{
	return ((y_back << 4) | x_fore) + blink;
}

/////////////////////////////////////////////////////////////////////////////
// The TColorDemo class implementation
/////////////////////////////////////////////////////////////////////////////
TColorDemo::TColorDemo(const TRect &bounds, uchar aColor, const char *aText):
	TView(bounds)
{
	options &= ~ofSelectable;
	eventMask = evBroadcast;
	color = aColor;
	text = newStr(aText);
}

TColorDemo::~TColorDemo()
{
	delete text;
}

void
TColorDemo::draw()
{
	TDrawBuffer buf;
	if( 0 == color ) buf.moveChar(0, ' ', 0x80, size.x);
	else buf.moveChar(0, ' ', color, size.x);
	buf.moveStr(0, text, color);
	writeLine(0, 0, size.x, size.y, buf);
}

void
TColorDemo::handleEvent(TEvent &event)
{
	if( evBroadcast == event.what && cmUpdateDemo == event.message.command )
	{
		setColor(*(uchar *)event.message.infoPtr);
	}
	TView::handleEvent(event);
}

/////////////////////////////////////////////////////////////////////////////
// The TColorSelect class implementation
/////////////////////////////////////////////////////////////////////////////
TColorSelect::TColorSelect(uchar aColor, const char *aTitle):
	TDialog(TRect(0, 0, 26, 17), aTitle),
	TWindowInit(TColorSelect::initFrame)
{
	options |= ofCentered;

	TRect bounds = getExtent();
	bounds.grow(-4, -2);
	bounds.b.y -= 3;
	chart = new TColorChart(bounds.a, aColor);
	chart->options |= ofFramed;
	insert(chart);
	demoLine = new TColorDemo(TRect(4,14,22,15), aColor, " this is a sample ");
	demoLine->options |= (ofFramed | ofCenterX);
	insert(demoLine);
	chart->select();
}

void
TColorSelect::handleEvent(TEvent &event)
{
	if( evKeyboard == event.what )
	{
		switch( event.keyDown.keyCode )
		{
			case kbEnter:
				color = chart->getColor();
				event.what = evCommand;
				event.message.command = cmOK;
				break;

			case kbEsc:
				event.what = evCommand;
				event.message.command = cmCancel;
		}
	}
	TDialog::handleEvent(event);
}

