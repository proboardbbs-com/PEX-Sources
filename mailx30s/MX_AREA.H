/*
 * This file is part of mail-x (EXE)
 *
 * Copyright (C) 1995-1997 by Branislav L. Slantchev
 * A Product of Silicon Creations, Inc.
 *
 * This file is distributed under the terms and conditions of the GNU
 * General Public License. For more information, refer to the file
 * Copying.Doc which is included in the archive.
*/
#ifndef INCLUDED_AREA_H
#define INCLUDED_AREA_H

#include "listview.h"

class Area: public zListRecord
{
public:
	Area(short index, const char *name);
	void draw(const zPoint &origin, short maxlen);
	void select(const zPoint &origin, short maxlen, Boolean off);

	short m_index;
	char  m_name[80];
};

#endif /* INCLUDED_AREA_H */
