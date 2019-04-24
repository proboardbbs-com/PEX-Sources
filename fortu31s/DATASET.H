/*
 * This file is part of Fortune Cookie v3.00.R1
 *
 * Copyright (c) 1995, 1997 by Branislav L. Slantchev
 * A fine product of Silicon Creations, Inc. (gargoyle)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the License which accompanies this
 * software. This library is distributed in the hope that it will
 * be useful, but without any warranty; without even the implied
 * warranty of merchantability or fitness for a particular purpose.
 *
 * You should have received a copy of the License along with this
 * library, in the file LICENSE.DOC; if not, write to the address
 * below to receive a copy via electronic mail.
 *
 * You can reach Branislav L. Slantchev (Silicon Creations, Inc.)
 * at bslantch@cs.angelo.edu. The file SUPPORT.DOC has the current
 * telephone numbers and the postal address for contacts.
*/
#ifndef INCLUDED_DATASET_H
#define INCLUDED_DATASET_H

#define FORTUNE_ID_STRING "Ÿ•çÅ–¤‰\b\b\b\b\b\b\b\032"
#define FORTUNE_ID_LENGTH sizeof(FORTUNE_ID_STRING)

struct index_header
{
	char id[FORTUNE_ID_LENGTH];  // text id
	long entries;   // number of entries
	long last_used; // last used entry
};

struct index_item
{
	long   offset;  // offset into the file
	size_t length;  // # of characters to read
};

#endif /* INCLUDED_DATASET_H */