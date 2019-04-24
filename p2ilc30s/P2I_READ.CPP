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
#include "pbuser.h"
#include "pblsdk.h"
#include "file.h"
#include "proboard.h"
#include "utils.h"
#include "pbnames.h"
#include <stdio.h>

/*
 * read the last record from the ProBoard binary log and load user record
*/
void
read_user( int node, USER_REC *pUser )
{
	FILE     *fp;
	BINLOG    Binlog;
	zProUser  User;
	Boolean   found = False;
	long      recno;

	if( 0 != (fp = pb_fopen(fnBINLOG, "rb", PBDIR_SYS)) )
	{
		/* get the last caller info from the ProBoard's binlog */
		fseek(fp, 0, SEEK_END);
		fseek(fp, -(long)sizeof(Binlog), SEEK_CUR);
		do{
			file_read(fp, &Binlog);
			if( node == Binlog.node )
			{
				found = True;
				break;
			}
			fseek(fp, -2L * sizeof(Binlog), SEEK_CUR);
		} while( 0L != ftell(fp) );
		fclose(fp);

		if( !found )
		{
			fail("   ! No entry for caller on node %d\n", node);
		}

		/* load the user record for this caller */
		recno = User.Find(Binlog.name);

		if( -1L != recno )
		{
			User.Load(recno);
			*pUser = User.m_rec;
			fprintf(stdout, "   $ Last caller: %s (%s)\n",
							pUser->name, pUser->alias);
		}
		else
		{
			fail("   ! User '%s' not found in USERS.BBS\n", Binlog.name);
		}
	}
	else fail("   ! Unable to read the binary log.\n");
}
