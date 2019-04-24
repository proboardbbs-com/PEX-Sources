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
#ifndef INCLUDED_FLAGSEL_H
#define INCLUDED_FLAGSEL_H

#define Uses_TDialog
#define Uses_TEvent
#define Uses_TDrawBuffer
#define Uses_TKeys
#include <tv.h>
#include "typedef.h"
#include "tvdefs.h"

struct TFlagData
{
	ulong flags;
    ulong flagsNot;
};

class TFlagView: public TView
{
friend class TFlagSelect;

public:
	TFlagView(const TPoint &origin, ulong aFlags, ulong aFlagsRev);
    virtual void   handleEvent(TEvent &event);
    virtual void   draw();
    virtual TVINT  dataSize(){ return TVINT(sizeof(long) * 2); }

    ulong flags;
    ulong flagsRev;
};

class TFlagSelect: public TDialog
{
public:
	TFlagSelect(ulong aFlags = 0, ulong aFlagsRev = 0, const char *aTitle = "Flags");
    virtual void handleEvent(TEvent &event);

    long getFlags() const { return interior->flags; }
    long getRevFlags() const { return interior->flagsRev; }

    void   getData(void *);
    void   setData(void *);
    TVINT  dataSize();

protected:
	TFlagView *interior;
};

#endif /* INCLUDED_FLAGSEL_H */
