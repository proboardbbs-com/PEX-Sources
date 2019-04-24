/*
 * This file is part of Listerz v2.0 (VE)
 *
 * Copyright (C) 1995-1997 by Branislav L. Slantchev
 * A Product of Silicon Creations, Inc.
 *
 * This file is distributed under the terms and conditions of the GNU
 * General Public License. For more information, refer to the file
 * Copying.Doc which is included in the archive.
*/
#ifndef INCLUDED_LSZ_AREA_H
#define INCLUDED_LSZ_AREA_H

#include "listview.h"
#include "pbstruct.h"

class Area: public zListRecord
{
public:
	Area(short index);
	void draw(const zPoint &origin, short maxlen);
	void select(const zPoint &origin, short maxlen, Boolean off);

	short m_index;
};

class Group: public zListRecord
{
public:
	Group(short num, const GROUP_PB &group);
	void draw(const zPoint &origin, short maxlen);
	void select(const zPoint &origin, short maxlen, Boolean);

	short m_num;  // NOT index, but a group number

private:
	char  m_name[80];
};

#endif /* INCLUDED_LSZ_AREA_H */
