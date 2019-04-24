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
#ifndef INCLUDED_TMFRAME_H
#define INCLUDED_TMFRAME_H

#define Uses_TDialog
#define Uses_TEvent
#define Uses_TKeys
#include <tv.h>

#include "typedef.h"
#include "tvdefs.h"

const short tfWidth  = 48;
const short tfHeight =  7;

class TFrameInterior: public TView
{
friend class TTimeFrame;
public:
	TFrameInterior(TRect &r);
    virtual void handleEvent(TEvent &event);
    virtual void draw();

protected:
	void  setCell( Boolean reset );
    void  setAll( Boolean off );
	uchar timeFrame[tfHeight][tfWidth];
    short col, row;
};

class TTimeFrame: public TDialog
{
public:
	TTimeFrame(const char *aTitle);

    void getTimeFrame(TimeFrame aTimeFrame);
    void setTimeFrame(TimeFrame aTimeFrame);

	void   getData(void*);
	void   setData(void*);
	TVINT  dataSize();

protected:
	TFrameInterior *tframe;
};

#endif /* INCLUDED_TMFRAME_H */
