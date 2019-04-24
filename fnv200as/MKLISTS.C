/*
 File Library Navigator (ProBoard PEX and limited DOS EXE)
 Copyright (C) 1995 by Branislav L. Slantchev

 This file is part of the FileNav.

 FileNav is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2.

 FileNav is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with FileNav; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "filenav.h"
#include "gdlist.h"
#include "isaccess.h"

EXTERN void namefmt(char *src, char *dest, int n, word flags );

/*
 * read file or message groups
*/
	int
readGroups( dlist_t *list )
{
	FILEGROUP grp;
	FMN_STRUCT fmn;
	FILE *fp;
	char path[MAXPATH];
	int n;

	sprintf( path, "%s\\FGROUPS.PB", _systemPath );
	if( NULL == (fp = fopen(path, "rb")) ) return -1;

	for( n = 1;; ++n){
		fread( &grp, 1, sizeof(FILEGROUP), fp );
		if( feof(fp) ) break;

		if( EOS == grp.name[0] ) continue;

		fmn.num = n;
		fmn.flags = FMN_GROUP | FMN_ALLGROUPS;
		if( !isAccess(grp.level, grp.flags, grp.flagsNot) )
			continue;
#if 0
			fmn.flags |= FMN_NOACCESS;
#endif

		namefmt( fmn.name, grp.name, n, fmn.flags );

		if( -1 == dlist_link( list, &fmn, sizeof(FMN_STRUCT), GDL_END ) ){
			fclose(fp);
			return -1;
		}
	}

	fclose(fp);
	return n;
}


/*
 * read file or message areas
*/
	int
readAreas( dlist_t *list )
{
	FILEAREA fa;
	char path[MAXPATH];
	FMN_STRUCT fmn;
	FILE *fp;
	int n;

	sprintf( path, "%s\\FILECFG.PRO", _systemPath );
	if( NULL == (fp = fopen(path, "rb")) ) return -1;

	for( n = 1; ; n++ ){
		fread( &fa, 1, sizeof(FILEAREA), fp );
		if( feof(fp) ) break;

		fmn.num = n;
		fmn.flags = 0;

		if( EOS == fa.name[0] ) continue;
		if( !isAccess(fa.level, fa.flags, fa.flagsNot) )
			continue;
#if 0
			fmn.flags |= FMN_NOACCESS;
#endif
		if( fa.free ) fmn.flags |= FMN_FREE;
		if( fa.allGroups ) fmn.flags |= FMN_ALLGROUPS;
		if( fa.cdrom ) fmn.flags |= FMN_CDROM;
		if( fa.notops ) fmn.flags |= FMN_NOTOPS;
		memcpy( fmn.groups, fa.groups, sizeof(fa.groups) );
		namefmt( fmn.name, fa.name, fmn.num, fmn.flags );

		if( -1 == dlist_link(list, &fmn, sizeof(FMN_STRUCT), GDL_END ) ){
			n = -1;
			break;
		}
	}

	fclose(fp);
	return n;
}
