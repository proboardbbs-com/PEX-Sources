/*
 File Library Navigator (ProBoard PEX and limited DOS EXE)
 Copyright (C) 1995 by Branislav L. Slantchev

 This file is part of the FileNav.

 FileNav is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2.

 FileNav is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with FileNav; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#if !defined( __GETNODE_H )
#define __GETNODE_H

#include "filenav.h"

/* short-hand for the dlist_next/prev functions */
typedef void *(getnode_t)(void *);

#define getNext(from,group)  _getArea( (from), (group), __dlist_next )
#define getFirst(ls,grp)  _getHeadTail(__dlist_first(ls),(grp),__dlist_next)

EXTERN FMN_STRUCT *_getArea( FMN_STRUCT *, int, getnode_t );
EXTERN FMN_STRUCT *_getHeadTail( FMN_STRUCT *, int, getnode_t );

#endif
