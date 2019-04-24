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
#ifndef INCLUDED_COLRSEL_H
#define INCLUDED_COLRSEL_H

/*
 * Implements a color selection chart, like the one in ProBoard's ProCFG
*/
#define Uses_TRect
#define Uses_TView
#define Uses_TEvent
#define Uses_TDialog
#define Uses_TDrawBuffer
#define Uses_TKeys
#define Uses_TFrame
#include <tv.h>
#include "tvdefs.h"

////////////////////////////////////////////////////////////////////////////
// A color chart class that will be displayed in a window
////////////////////////////////////////////////////////////////////////////
class TColorChart: public TView
{
public:
	TColorChart(const TPoint &origin, uchar aColor = 0);

	virtual void  handleEvent(TEvent &event);
	virtual void  draw();
	virtual void  setColor(uchar aColor);
	virtual uchar getColor() const;

protected:
	void   selSet();    // sets the selction box
    void   selReset();  // resets the selection box

    // to translate from color values to coordinates, add 1 to the values
    // the reason for this is: we have one empty line on top and one
    // empty column to the left (significant). We also have an empty line
    // on the bottom and an empty column to the right, but these do not
    // affect out color-to-coordinate calculations
    uchar  x_fore, y_back;
    uchar  blink;
};

/////////////////////////////////////////////////////////////////////////////
// A class to display a simple framed text in a selected color
/////////////////////////////////////////////////////////////////////////////
class TColorDemo: public TView
{
public:
	TColorDemo(const TRect &bounds, uchar aColor, const char *aText);
    ~TColorDemo();
    void setColor(uchar aColor)
    	{ color = aColor; drawView(); }
    void setText(const char *aText)
    	{ delete text; text = newStr(aText); drawView(); }

    virtual void draw();
    virtual void handleEvent(TEvent&);

protected:
	uchar   color;
    char   *text;
};

////////////////////////////////////////////////////////////////////////////
// A class the lets you select colors, complete with a demo and a chart
////////////////////////////////////////////////////////////////////////////
class TColorSelect: public TDialog
{
public:
	TColorSelect(uchar aColor = 0, const char *aTitle = "colors");
    virtual void handleEvent(TEvent &event);

    uchar getColorCode() const { return color; }

private:
	TColorDemo  *demoLine;
	TColorChart *chart;
	uchar color;
};

#endif /* INCLUDED_COLRSEL_H */

