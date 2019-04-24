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
#include <time.h>
#include <stdio.h>

/*
 * save the last caller info into InterMail's history
*/
void
write_caller( USER_REC *pUser, const char *path )
{
	LASTCALL   Caller;
	FILE      *fp;
	struct tm  tmUser;

	if( 0 != (fp = fopen(path, "r+b")) )
	{
		fread(&Caller, sizeof(Caller), 1, fp);
		c2pStr(Caller.system_name, pUser->name);
		c2pStr(Caller.location, pUser->city);
		Caller.zone  = (ushort)pUser->timeUsed;
		Caller.net   = (ushort)pUser->totalTimeUsed;
		Caller.node  = (ushort)pUser->timesCalled;
		Caller.point = 0; /* not used for display */
		tmUser.tm_mday = pUser->lastDate[0];
		tmUser.tm_mon = pUser->lastDate[1] - 1;
		tmUser.tm_year = pUser->lastDate[2];
		tmUser.tm_hour = pUser->lastTime[0];
		tmUser.tm_min = pUser->lastTime[1];
		tmUser.tm_sec = pUser->lastTime[2];
		Caller.time = mktime(&tmUser) - (timezone - tmUser.tm_isdst * 3600);
		fseek(fp, 0L, SEEK_SET);
		file_write(fp, &Caller);
		fclose(fp);
	}
	else fail("   ! Unable to open last caller file '%s'\n", path);
}
