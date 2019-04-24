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
#ifndef INCLUDED_TAGLIST_H
#define INCLUDED_TAGLIST_H

#define Uses_TListBox
#define Uses_TEvent
#define Uses_TKeys
#define Uses_TCollection
#define Uses_TScrollBar
#define Uses_TGroup
#include <tv.h>
#include "tvdefs.h"

class far zBitVector;

////////////////////////////////////////////////////////////////////////////
// this is an 'empty' class that needs to be the base for the items you will
// be storing in the collection. this is done so that the list box can call
// the getText() routine to get the displayable part of your items
////////////////////////////////////////////////////////////////////////////
class TTagEntry: public TObject
{
public:
	virtual char *getText() = 0;
};

////////////////////////////////////////////////////////////////////////////
class TTagListBox: public TListBox
{
public:
	TTagListBox(const TRect &bounds, TScrollBar *aScrollBar, ushort maxSize);

	virtual void handleEvent(TEvent &event);
	virtual void getText(char *aDest, short item, short maxLen);

	void SetTagChar(char aChar){ tagChar = aChar; }
	void DeleteTag(short item);  // needs to be called when item is removed
	void InsertTag(short item);  // needs to be called when item is added
	void ToggleTag(short item);
	void SetTag(short item);
	void SetAllTags();
	void RemoveAllTags();
	void ToggleAllTags();
	Boolean IsTagged(short item);

protected:
	virtual void draw();

	short       rangeStart;
	char        tagChar;
	zBitVector *bitmap;
};

#endif /* INCLUDED_TAGLIST_H */
