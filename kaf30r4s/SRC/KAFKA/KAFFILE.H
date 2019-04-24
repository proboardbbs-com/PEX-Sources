/*
 * This file is part of the KaFKa Kompiler
 * Copyright (C) 1995 by Branislav L. Slantchev
 *
 * Distributed under the GNU General Public License
 * See the file 'copying' for more information about your license.
 *
*/
#ifndef __KAFFILE_H
#define __KAFFILE_H

// record structures for the .KDT and .KiX files
typedef unsigned short ushort;

// The header for the data files (.KDT)
struct KafDataFile{
	char  name[80];     // name of the file, ASCIIZ
    char  idString[10]; // 10 bytes id string (not ASCIIZ)
    short numRecs;      // number of records stored
    char  __rsrvd[58];  // reserved space

    KafDataFile();      // constructor
};

// The header for the index files (.KiX)
struct KafIdxFile{
	char  name[80];
    char  idString[10];
    short numRecs;
    char  __rsrvd[58];

    KafIdxFile();
};

// The local header in the data file (.KDT)
struct KafEntryData{
	char   name[26];   // name of the sequence
    ushort dataSize;   // length of the record (excluding the header)
    short  numLines;   // number of lines to animate
    ushort speed;      // default animation speed (0 == static)
    short  maxChars;   // maximum line length (0 == ignore)
    char   __rsrvd[6];
};

extern const char kafDataID[10];
extern const char kafIdxID[10];

#endif
