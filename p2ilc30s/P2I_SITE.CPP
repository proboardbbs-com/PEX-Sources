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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pblsdk.h"
#include "file.h"
#include "utils.h"

void
write_site( const char *site, const char *swpath )
{
	SITEINFO   Site;
	FILE      *fp;
	const int  nMaxSites = 256;
	const int  nNameLen = sizeof(Site.systemname);
	const int  nLocLen = sizeof(Site.systemloc);
	char       buf[nMaxSites];
	char      *sites[nMaxSites];
	int        nSites = 0;

	// load the switch info (well, try at least)
	randomize();
	if( 0 != (fp = fopen(swpath, "r")) )
	{
		fgets(buf, sizeof(buf), fp);
		while( !feof(fp) && nSites < nMaxSites )
		{
			buf[strlen(buf) - 1] = EOS;
			if( EOS != buf[0] && ';' != buf[0] )
			{
				if( 0 == (sites[nSites++] = strdup(buf)) )
				{
					nSites--;
					break;
				}
			}
			fgets(buf, sizeof(buf), fp);
		}
		fclose(fp);
	}
	else fail("   ! Unable to open switch file '%s'\n", swpath);

	if( 0 != (fp = fopen(site, "r+b")) )
	{
		file_read(fp, &Site);
		for( ;; )
		{
			// get an even offset into the array of site info
			// make sure we're within the array limits too!
			int nCurSite = (random(nSites - 2) + 1) & 0xfffe;
			// check for a new system name (assume location ok)
			if( 0 != strcmp(Site.systemname, sites[nCurSite]) )
			{
				strncpy(Site.systemname, sites[nCurSite], nNameLen);
				Site.systemname[nNameLen - 1] = EOS;
				strncpy(Site.systemloc, sites[nCurSite+1], nLocLen);
				Site.systemloc[nLocLen - 1] = EOS;
				break;
			}
		}
		printf("   $ Site       : %s\n   $ Location   : %s\n",
					Site.systemname, Site.systemloc);
		rewind(fp);
		file_write(fp, &Site);
		fclose(fp);
	}
	else fail("   ! Unable to update site info '%s'\n", site);

	// cleanup
	for( int i = 0; i < nSites; ++i ) free(sites[i]);
}
