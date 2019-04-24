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
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "bucket.h"
#include "file.h"
#include "dataset.h"

void
main(int /*argc*/, char *argv[])
{
	FILE                *ifp;
	char                 buf[1024], fileName[256];
	index_header         header;
	index_item           item;
	zBucket<index_item> *index;

	fprintf(stderr, "Cook v1.00.R1 ú "
					"Copyright (c) 1997 Branislav L. Slantchev "
					"%s\n\n", __DATE__);

	if( !(ifp = fopen(argv[1], "r")) )
		fail("usage: cook <fortune datafile>\n\nunable to open '%s'\n", argv[1]);
	file_chext(fileName, argv[1], ".idx");
	fclose(fopen(fileName, "wb"));
	if( !(index = new zBucket<index_item>(fileName, 1, True)) || !index )
	{
		fclose(ifp);
		fail("unable to create the index file '%s'\n", fileName);
	}

	strcpy(header.id, FORTUNE_ID_STRING);
	header.entries = 0L;
	header.last_used = 0L;
	item.offset = 0L;

	setvbuf(ifp, NULL, _IOFBF, 0x8000);
	fprintf(stderr, "Working...");

	fgets(buf, sizeof(buf), ifp);
	while( !feof(ifp) )
	{
		if( '%' == buf[0] && '\n' == buf[1] )
		{
			long offset = ftell(ifp);

			item.length = (size_t)(offset - item.offset - (strlen(buf) + 1));
			(*index)[header.entries++] = item;
			item.offset = offset;
		}
		fgets(buf, sizeof(buf), ifp);
	}

	// rewrite the header of the index
	delete index;
	fclose(ifp);

	if( !(ifp = fopen(fileName, "r+b")) )
	{
		file_remove(fileName);
		fail("unable to update the index '%s'\n", fileName);
	}
	fseek(ifp, 0L, SEEK_END);
	fwrite(&header, sizeof(header), 1, ifp);
	fclose(ifp);

	fprintf(stderr, "done (%ld cookies)\n", header.entries);
}