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

#ifndef __TLISTBOX_H
#define __TLISTBOX_H

#include "listrec.h"
#include "class/dlist.h"
#include "class/tobjs.h"

class TListBox
{
public:
	TListBox(TDoubleList *aList);

    void  handle(ushort &aKeyCode);	// handle command (direction keys only)
    void  draw();					// redraw a full-screen of the listbox
    void  setFocus(void *item);		// set the focus to item from list
    void  drawFocus();				// redisplays the focused item
    void  newList(TDoubleList *aList);
    void *focused(){ return focus.get(); }	// get focused item
    Boolean lastItem() const;		// if last item is displayed
    Boolean firstItem() const;		// if first item is displayed

protected:
	void  writeLiteBar(Boolean turnOff);
    void  getNextLimit(TListCursor &newLimit, TListCursor &whence);
    void  getPrevLimit(TListCursor &newLimit, TListCursor &whence);
    short rowsToSkip(TListCursor *from, TListCursor *to);

	TDoubleList *list;		// yeah, this is the list
    TListCursor  focus;		// the iterator for the list (focused item)
    TListCursor  top;		// the current top item on the list
    TListCursor  bottom;	// the current bottom item on the list
    CPoint		 where;		// the selection bar coordinates

    static CPoint origin;	// the origin point on the screen
    static CPoint size;		// the size of the rectangle
};

#endif