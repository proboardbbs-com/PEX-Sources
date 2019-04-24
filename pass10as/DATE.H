/*
 PB-GNU PASSWD ProBoard PEX Version
 Copyright (C) 1995 by Branislav L. Slantchev

 This file is part of the PB-GNU PASSWD.

 PB-GNU PASSWD is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2.

 PB-GNU PASSWD is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with PB-GNU PASSWD; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#if !defined( __DATE_H )
#define __DATE_H


#include <pb_sdk.h>

/* #define DEBUG */

bool isLeapYear( word year );
word dayOfYear( word day, word mon, word year );
word daysDiff( word day, word mon, word year );

#endif
