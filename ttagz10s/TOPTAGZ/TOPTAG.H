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

#ifndef __TOPTAG_H
#define __TOPTAG_H

#include "stdtype.h"

struct TTopTag
{
	ushort num;			// number of downloads
    short  area;		// area file is in
    char   name[13];	// file name
    char   desc[50];	// description
};

#endif