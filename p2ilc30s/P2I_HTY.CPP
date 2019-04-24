/*
 * This file is part of P2iLC (EXE)
 *
 * Copyright (C) 1995-1997 by Branislav L. Slantchev
 * A Product of Silicon Creations, Inc.
 *
 * This file is distributed under the terms and conditions of the GNU
 * General Public License. For more information, refer to the file
 * Copying.Doc which is included in the archive.
 *
*/
#include "p2ilc.h"
#include "pblsdk.h"
#include "proboard.h"
#include "file.h"
#include "utils.h"
#include <stdio.h>

/*
 * write a record to InterMail's inbound history
*/
void
write_history( USER_REC *pUser, const char *path )
{
	MAILHIST  History;
	FILE     *fp;

	// now open the history file
	if( 0 != (fp = fopen(path, "r+b")) ){
		c2pStr(History.system_name, pUser->name);
		c2pStr(History.city, pUser->city);
		History.day        = pUser->lastDate[0];
		History.month      = pUser->lastDate[1];
		History.year       = pUser->lastDate[2] + 1900;
		History.hour       = pUser->lastTime[0];
		History.min        = pUser->lastTime[1];
		History.sec        = pUser->lastTime[2];
		History.sent_bytes = (long)pUser->kbToday * 1024L;
		History.rcvd_bytes = (long)pUser->kbUploaded * 1024L;
		History.cost       = (ushort)pUser->timeUsed;
		History.net        = (ushort)pUser->numDownloads;
		History.node       = (ushort)pUser->numUploads;
		History.zone       = 0; /* not used for display */
		History.point      = 0; /* not used for display */
		fseek(fp, 0L, SEEK_END);
		file_write(fp, &History);
		fclose(fp);
	}
	else fail("   ! Unable to open inbound history '%s'\n", path);
}
