/*
 * This file is part of the KaFKa Kompiler
 * Copyright (C) 1995 by Branislav L. Slantchev
 *
 * Distributed under the GNU General Public License
 * See the file 'copying' for more information about your license.
 *
*/
#include "kaffile.h"
#include <string.h>

const char kafDataID[10] = "SCi-DaT";
const char kafIdxID[10]  = "SCi-iDX";

KafDataFile::KafDataFile()
{
	strcpy(name, "KaF DaTa FiL�.\r\nCoPyRiGHT (C) 1995 By BRaNiSLaV L. SLaNTCH�V (SCi).\n\x1a");
    memcpy(idString, kafDataID, sizeof(kafDataID));
    numRecs = 0;
}

KafIdxFile::KafIdxFile()
{
	strcpy(name, "KaF iND�X FiL�.\r\nCoPyRiGHT (C) 1995 By BRaNiSLaV L. SLaNTCH�V (SCi).\n\x1a");
    memcpy(idString, kafIdxID, sizeof(kafIdxID));
    numRecs = 0;
}

