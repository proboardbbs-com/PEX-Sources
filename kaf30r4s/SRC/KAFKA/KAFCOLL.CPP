/*
 * This file is part of the KaFKa Kompiler
 * Copyright (C) 1995 by Branislav L. Slantchev
 *
 * Distributed under the GNU General Public License
 * See the file 'copying' for more information about your license.
 *
*/
#include "kafcoll.h"
#include <string.h>

TKafData::TKafData(const KafEntryData &fileData, long aOffset):
    speed(fileData.speed),
    maxChars(fileData.maxChars),
    offset(aOffset)
{
	strcpy(name, fileData.name);
}
