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

#include "listbox.h"
#include "kbcodes.h"
#include "listrec.h"

// these are constant
CPoint TListBox::origin(1, 3);
CPoint TListBox::size(80, 23);	// size.y - origin.y = numRows

TListBox::TListBox(TDoubleList *aList):
    list(aList),
	focus(*list, True),
    top(*list, True),
    bottom(*list, True),
    where(0, 0)
{
	getNextLimit(bottom, top);
}

void TListBox::draw()
{
    TListCursor cur = top;
	short x = origin.x + where.x;
    short y = origin.y + 0;
    short nrows = size.y - origin.y;

    while( 0 < nrows ){
    	GotoXY(x, y);
        TBaseRecord *elem = (TBaseRecord *)cur.get();
        elem->draw();
        nrows -= elem->textRows;
        y += elem->textRows;
        if( !cur.next() ) break;
	}
    if( 0 < nrows ){
    	SetColor(0);
		while( nrows-- ){
	        GotoXY(x, y++);
	        ClrEol();
        }
	}
    writeLiteBar(False);
}

void TListBox::handle(ushort &aKeyCode)
{
	switch( aKeyCode ){

    	case kbdLeft : case '4':
        	if( !top.atStart() ){
                TListCursor nt = top; nt.prev();
                getPrevLimit(top, nt);
                getNextLimit(bottom, top);
                focus = top;
                where.y = 0;
                draw();
			}
        break;

        case kbdRight: case '6':
        	if( !bottom.atEnd() ){
                TListCursor nb = bottom; nb.next();
                getNextLimit(bottom, nb);
                getPrevLimit(top, bottom);
                focus = top;
                where.y = 0;
                draw();
			}
		break;

        case kbdUp   : case '8':
        	writeLiteBar(True);
        	if( focus() == top() ){
				focus = bottom;
                where.y = rowsToSkip(&top, &bottom);
            }
            else{
            	focus--;
                where.y -= rowsToSkip(&focus, 0);
			}
		    writeLiteBar(False);
        break;

        case kbdDown : case '2':
        	writeLiteBar(True);
        	if( focus() == bottom() ){
				focus = top;
				where.y = 0;
			}
            else{
				focus++;
                where.y += rowsToSkip(0, &focus);
			}
		    writeLiteBar(False);
        break;

        case kbdHome : case '7':
            if( focus() != top() ){
            	writeLiteBar(True);
                focus = top;
                where.y = 0;
			    writeLiteBar(False);
			}
        break;

        case kbdEnd  : case '1':
        	if( focus() != bottom() ){
            	writeLiteBar(True);
                focus = bottom;
                where.y = rowsToSkip(&top, &bottom);
			    writeLiteBar(False);
			}
		break;

        default:
        	return;
	}
    aKeyCode = 0;
}

void TListBox::writeLiteBar(Boolean turnOff)
{
	TBaseRecord *p = (TBaseRecord *)(focus.get());
    GotoXY(origin.x+where.x, origin.y+where.y);
    if( True == turnOff ) p->deselect();
    else p->select();
}

void TListBox::getNextLimit(TListCursor &newLimit, TListCursor &whence)
{
	TListCursor cur = whence;
    short numRows = size.y - origin.y;

    numRows -= ((TBaseRecord *)cur.get())->textRows;
    while( 0 < numRows && !cur.atEnd() ){
    	cur++;
        numRows -= ((TBaseRecord *)cur.get())->textRows;
	}
    newLimit = cur;
}

void TListBox::getPrevLimit(TListCursor &newLimit, TListCursor &whence)
{
	TListCursor cur = whence;
    short numRows = size.y - origin.y;

    numRows -= ((TBaseRecord *)cur.get())->textRows;
    while( 0 < numRows && !cur.atStart() ){
    	cur--;
        numRows -= ((TBaseRecord *)cur.get())->textRows;
	}
    newLimit = cur;
}

/*
 * if any arg == 0, means get to next, i.e. from = 0, get from 'to'
 * to the previous elem; if to = 0, get from 'from' to next elem
*/
short TListBox::rowsToSkip(TListCursor *aFrom, TListCursor *aTo)
{
	if( 0 == aFrom && 0 == aTo ) return 0;
    if( 0 == aFrom && !aTo->atStart() ){
    	TListCursor to(*aTo);
    	to.prev();
		return ((TBaseRecord *)to.get())->textRows;
	}
    else if( 0 == aTo && !aFrom->atEnd() ){
    	TListCursor from(*aFrom);
    	from.next();
        return ((TBaseRecord *)from.get())->textRows;
	}
    else{
    	TListCursor from(*aFrom), to(*aTo);
	    short num = 0;
	    while( !from.atEnd() && from() != to() ){
	    	num += ((TBaseRecord *)from.get())->textRows;
	        from.next();
		}
	    return num;
	}
}

void TListBox::setFocus(void *item)
{
	TListCursor cur(*list, True);
    do{
    	if( cur.get() == item ) break;
	}while( cur.next() );

    if( item == cur.get() ){
    	getNextLimit(bottom, cur);
        getPrevLimit(top, bottom);
        focus = cur;
        where.y = rowsToSkip(&top, &focus);
	}
	draw();
}

void TListBox::drawFocus()
{
	where.y = rowsToSkip(&top, &focus);
    GotoXY(1, origin.y + where.y);
    ((TBaseRecord *)focus.get())->draw();
    writeLiteBar(False);
}

Boolean TListBox::lastItem() const
{
	return Boolean(bottom.atEnd());
}

Boolean TListBox::firstItem() const
{
	return Boolean(top.atStart());
}

void TListBox::newList(TDoubleList *aList)
{
    list = aList;
	focus = TListCursor(*aList, True),
    top = TListCursor(*aList, True),
    bottom = TListCursor(*aList, True),
    where = CPoint(0, 0);
	getNextLimit(bottom, top);
}