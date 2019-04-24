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

#ifndef __LISTREC_H
#define __LISTREC_H
#include "pblib.h"
#include "proutil.h"
#include "toptagz.h"

#ifndef PB_SDK
	#include <string.h>
#endif

class TBaseRecord
{
public:
	TBaseRecord(short aNum, short aFlags, short aTextRows):
    	num(aNum), flags(aFlags), textRows(aTextRows), tagged(False){}

	short   num;			// record number
    short   flags;			// access flags (TUserFlags only)
    short   textRows;		// number of rows per description
    Boolean tagged;

    virtual void toggleTag(){}	// this is a NOP function, can be overriden

    virtual void draw() = 0;	// the MUST be provided in derived classes
    virtual void select() = 0;
    virtual void deselect() = 0;
};

class TTopFile: public TBaseRecord
{
public:
	TTopFile(short aNum, TTopTag &aFile);

    void draw();
    void select();
    void deselect();
    void toggleTag();

    TTopTag file;
};

#endif