/*
 * This file is part of the KaFKa Kompiler
 * Copyright (C) 1995 by Branislav L. Slantchev
 *
 * Distributed under the GNU General Public License
 * See the file 'copying' for more information about your license.
 *
*/
#ifndef __KAFCOLL_H
#define __KAFCOLL_H

#define Uses_TCollection
#include <tv.h>

#include "kaffile.h"

struct TKafData
{
	char   name[26];   // name of the record
    ushort speed;      // default animation speed (0 == static)
    short  maxChars;   // maximum line length (0 == ignore)
    long   offset;     // offset in the data file (stored in .KiX)

    TKafData(const KafEntryData &fileData, long aOffset);
};

class TKafCollection: public TCollection
{
public:
	TKafCollection(ccIndex aLimit, ccIndex aDelta): TCollection(aLimit, aDelta){}
    TKafData *at(ccIndex index){ return (TKafData *)TCollection::at(index); }

private: // we don't need those, but have to have them
	virtual void writeItem(void *item, opstream& os){ return; }
	virtual void *readItem(ipstream& is){ return 0; }
};

#endif
