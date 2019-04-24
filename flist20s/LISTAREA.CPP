/*
 * This file is part of FLS/Fast! Listing Generator
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
#include "listarea.h"
#include "utils.h"
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <dir.h>

#define SECS_DAY  20864

	int
listarea( int num, const FILEAREA *fa, FILE *fp_list )
{
	FILE  *fp_fbbs;
	char   buf1[255], buf2[255], timebuf[9], *ptr, *descr;
	char   fname[15], date[9];
	long   size, bytes_area = 0L;
	struct stat statbuf;
	time_t today_time;
	int    in_listing = 0;

	/* open the listing file */
	if( !(fp_fbbs = fopen( fa->listpath, "r" )) )
		fail( "%s: couldn't access listfile for area '%d'",	progname, num );

	area_kbytes = 0;
	area_files  = 0;

	today_time = time(NULL);

	for( ;; )
	{
		if( NULL == fgets( buf1, BUFSIZ, fp_fbbs ) ) break;

		/* skip empty lines */
		for( ptr = buf1; iswhite(*ptr) && '\0' != *ptr; ptr++ )
			;
		if( '\0' == *ptr || '\b' == *ptr ) continue;

		/* see other restrictions */
		if( exclude_lines && strchr( "!;", buf1[0] ) ) continue;

		/* description line, see if we want single line description */
		if( strchr( " \t!;+", *ptr ) )
		{
			if( !single_line && in_listing )
			{
				for( ptr++; iswhite(*ptr) && '\0' != *ptr; ++ptr )
					;
				if( '\0' == *ptr ) continue;
				sprintf( buf2, "                               %s", ptr );
				fputs( buf2, fp_list );
				if( '\n' != buf2[strlen(buf2) - 1] )
					fputc( '\n', fp_list );
			}
			continue;
		}

		/* cdrom areas are copied verbatim, date and size are here */
		/* no check for days_old is done on cdrom areas            */
		if( fa->copyLocal )
		{
			sscanf( buf1, "%s %ld %s %*s %[\x20-\xff]", fname, &size, date, buf2 );
			sprintf( buf1, "%-12s %8ld %s %s", fname, size, date, buf2 );
			bytes_area += size;
			strcpy( buf2, buf1 );
		}
		/* normal area, must get file size and date */
		else
		{
			if( !(ptr = strtok( buf1, " \t" )) ) continue;

			/* get description, skipping file counters */
			if( !(descr = strtok( NULL, "]" )) )
				descr = "description missing";
			else descr = strtok( NULL, "\0" );

			while( iswhite(*descr) && '\0' != *descr ) descr++;

			strcpy( buf2, fa->filepath );
			if( '\\' != buf2[strlen(buf2) - 1] ) strcat( buf2, "\\" );
			strcat( buf2, ptr );
			if( -1 == stat( buf2, &statbuf ) ) continue;

			/* see if file is newer */
			if( days_old && (days_old < (difftime(today_time, statbuf.st_ctime) / SECS_DAY )) )
				continue;

			strftime( timebuf, 9, "%m/%d/%y", localtime(&statbuf.st_ctime) );
			sprintf( buf2, "%-12s %8ld %s %s", ptr, statbuf.st_size,
				timebuf, descr  );
			bytes_area += statbuf.st_size;
		}

		fputs( buf2, fp_list );
		if( '\n' != buf2[strlen(buf2) - 1] ) fputc( '\n', fp_list );
		in_listing = 1;
		area_files++;
	}

	fclose( fp_fbbs );
	area_kbytes = (unsigned int)(bytes_area / 1024);
	return area_files;
}
