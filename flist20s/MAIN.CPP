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
#pragma warn -ofp
#include <stdio.h>
#include <stdlib.h>
#include <dir.h>
#include <string.h>
#include <time.h>
#include "pbstruct.h"
#include "getopt.h"
#include "listarea.h"
#include "templ.h"
#include "file.h"
#include "utils.h"

#define __VERSION__  "v2.00.R1"

const char version[] =
	"FLS/Fast! " __VERSION__
	" Copyright (c) 1995,1997 Branislav L. Slantchev, "
	__DATE__ "\n";

const char more_version[] =
	"This version was compiled on " __DATE__ " at " __TIME__ "\n";

const char copying[] =
	"Copyright (c) 1995, 1997 by Branislav L. Slantchev\n"
	"This is a product of Silicon Creations, Inc.\n"
	"\n"
	"This program is free software; you can redistribute it and/or modify\n"
	"it under the terms of the GNU General Public License as published by\n"
	"the Free Software Foundation; either version 2 of the License or\n"
	"(at your opinion) any later version.\n"
	"\n"
	"This program is distributed in the hope that it will be useful,\n"
	"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
	"GNU General Public License for more details.\n"
	"\n"
	"You should have received a copy of the GNU General Public License\n"
	"along with the program; see the file LICENSE.DOC.  If not, write to\n"
	"the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.\n"
	"\n"
	"The author can be reached at bslantch@cs.angelo.edu\n";

void usage_and_die( void );

ushort exclude_lines  = 0; /* exclude lines starting with ';' or '!' */
ushort exclude_cdrom  = 0; /* don't include cdrom file areas         */
ushort single_line    = 0; /* single line descriptions               */
ushort days_old       = 0; /* file newer than days_old only          */
ushort min_level      = 0; /* minimum security level to include area */
ushort max_level = 65535U; /* maximum security level to include area */
char  *listing_file;       /* name of output listing file            */

char *progname;            /* program name                           */

unsigned int area_kbytes;           /* kbytes in current area (after list)    */
unsigned int area_files;            /* files in current area (after list)     */
char area_name[80];        /* area name (before and after list)      */
int area_number;           /* area number (before and after list)    */

int total_areas = 0;       /* total number of areas                  */
long total_files = 0L;     /* total number of files                  */
long total_kbytes = 0L;    /* total kbytes in files                  */


int
main( int argc, char *argv[] )
{
	char     *root_dir, areafile_path[MAXPATH];
	char      header_path[MAXPATH] = "",
			  footer_path[MAXPATH] = "",
			  template_header[MAXPATH] = "",
			  template_footer[MAXPATH] = "",
			  curdir[MAXPATH], buf[512];
	char      groups[255];
	int       group_min = 0, group_max = 255, group_limits = 0;
	int       i, c;
	FILE     *fp_list, *fp_conf;
	FILEAREA  fa;

	progname = strlwr(file_name(*argv));

	fprintf(stderr, "%s\n", version);

	if( '?' == argv[1][0] || '?' == argv[1][1] ) usage_and_die();

	while(EOF != (c = getopt(argc, argv, "g:f:r:o:t:p:m:M:n:xsvceh")) )
	{
		switch( c )
		{
			case 'g':
				char *ptr;
				/* groups in range form: [min-max] */
				if( '[' == optarg[0] )
				{
					ptr = strtok(&optarg[1], " -");
					if( !ptr ) fail( "%s: syntax error: wrong [] range", progname );
					group_min = atoi( ptr );
					ptr = strtok(NULL, " ]");
					if( !ptr ) fail("%s: syntax error: unbalanced []", progname);
					group_max = atoi( ptr );
					group_limits = 1;
					fprintf(stderr, "\nUsing %d <= group <= %d\n",
						group_min, group_max);
				}
				/* listed, comma-separated: 23,44,12 */
				else
				{
					memset(groups, -1, 255);
					for( i = 0, ptr = strtok(optarg, ",");
						 ptr;
						 ptr = strtok(NULL, "," ), ++i)
					{
						groups[i] = atoi(ptr);
					}
					group_limits = 2;
				}
			break;

			case 'r': strcpy(header_path, optarg); break;
			case 'o': strcpy(footer_path, optarg); break;
			case 't': strcpy(template_header, optarg); break;
			case 'p': strcpy(template_footer, optarg); break;

			case 'v': fail(more_version);
			case 'c': fail(copying);
			case 'h': usage_and_die();

			case 'f': listing_file = optarg; break;
			case 'm': min_level = (ushort)atoi(optarg);	break;
			case 'M': max_level = (ushort)atoi(optarg);	break;
			case 'n': days_old = (ushort)atoi(optarg); break;
			case 'x': exclude_lines = 1; break;
			case 'e': exclude_cdrom = 1; break;
			case 's': single_line = 1; break;

			default :
				fail( "%s: unrecognized option: '%s'",
					progname, argv[optind - 1] );
		}
	}

	/* see if we can access to file area definition file */
	if( !(root_dir = getenv( "PROBOARD" )) )
		fail("%s: environment variable PROBOARD is not set.", progname);
	strcat(strcpy(areafile_path, root_dir), "/FILECFG.PRO");
	if( !(fp_conf = fopen( areafile_path, "rb" )) )
		fail( "%s: could not read from '%s'", progname, areafile_path );

	/* see if we can create the output listing file */
	if( !listing_file ) listing_file = "allfiles.txt";

	if( !(fp_list = fopen( listing_file, "w" )) )
		fail( "%s: couldn't create file '%s'", progname, listing_file );

	/* see if we can construct header/footer file paths */
	strcpy(curdir, file_dirspec(argv[0]));

	if( '\0' == header_path[0] )
	{
		sprintf(buf, "%sallfiles.hdr", curdir);
		if( file_exist(buf) ) strcpy(header_path, buf);
	}

	if( '\0' == footer_path[0] )
	{
		sprintf(buf, "%sallfiles.ftr", curdir);
		if( file_exist(buf) ) strcpy(footer_path, buf);
	}

	if( '\0' == template_header[0] )
	{
		sprintf(buf, "%sfilearea.hdr", curdir);
		if( file_exist(buf) ) strcpy(template_header, buf);
	}

	if( '\0' == template_footer[0] )
	{
		sprintf(buf, "%sfilearea.ftr", curdir);
		if( file_exist(buf) ) strcpy(template_footer, buf);
	}

	/* see if we have the header file */
	if( '\0' != header_path[0] ) parse_file( header_path, fp_list );

	/* process all areas */
	for( total_areas = 0; ; total_areas++ )
	{
		int bytes = fread(&fa, 1, sizeof(FILEAREA), fp_conf);

		/* end of file */
		if( !bytes ) break;

		/* file I/O error (read error) */
		if( sizeof(FILEAREA) != bytes )
			fail( "%s: error reading the area definition file.", progname );

		/* if min/max levels, see if we need to list area */
		if( fa.level < min_level || fa.level > max_level ) continue;

		/* see if we need to skip a cdrom area */
		if( exclude_cdrom && fa.copyLocal ) continue;

		/* see if the group is empty */
		if( '\0' == fa.name[0] ) continue;

		/* check if belongs to group area specs */
		if( group_limits && !fa.allGroups )
		{
			int break_on = 0;

			if( 2 == group_limits )
			{
				for( i = 0; -1 != groups[i]; ++i )
					for( int j = 0; j < 4; ++j )
						if( fa.groups[j] == groups[i] ) goto PROCESS;
				break_on = 1;
			}
			else
			{
				for( i = 0; i < 4; i++ )
					if( fa.groups[i] < group_min || fa.groups[i] > group_max )
					{
						break_on = 1;
						break;
					}
			}
			if( break_on ) continue;
		}

	PROCESS:
		printf("area: %4d '%s'...", total_areas, fa.name);
		strcpy(area_name, fa.name);
		area_number = total_areas + 1;
		parse_file(template_header, fp_list);
		total_files += (long)listarea(total_areas + 1, &fa, fp_list);
		total_kbytes += (long)area_kbytes;
		parse_file(template_footer, fp_list);
		printf(" done\n");
	}

	/* see if we have the footer file */
	parse_file(footer_path, fp_list);
	/* print some statistics */
	printf("\ntotal areas scanned     : %d\n", total_areas);
	printf("total files found       : %ld\n", total_files);
	printf("file listing created in : '%s'\n", listing_file);

	/* cleanup */
	fclose(fp_list);
	fclose(fp_conf);
	if( !total_files ) unlink(listing_file);

	return 0;
}

/*
** prints usage info and terminates program
*/
void
usage_and_die( void )
{
	fprintf(stderr, "Usage: %s [options]\n\n", progname);

	fputs(
"Options:            Description:                 Defaults:\n"
"  -f listing file    output file path             ALLFILES.TXT\n"
"  -g number          include areas in group(s)    all groups and areas\n"
"  -r header file     header file path             ALLFILES.HDR\n"
"  -o footer file     footer file path             ALLFILES.FTR\n"
"  -t area header     area header template path    FILEAREA.HDR\n"
"  -p area footer     area footer template path    FILEAREA.FTR\n"
"  -m level           minimum security level       ignore security\n"
"  -M level           maximum security level       ignore security\n"
"  -n days            list file newer than days    include all files\n"
"  -x                 exclude lines '!' or ';'     list all lines\n"
"  -e                 exclude cd-rom file areas    include cd-rom areas\n"
"  -s                 single line descriptions     copy descriptions\n"
"  -v                 print version information    n/a\n"
"  -c                 print copyright info         n/a\n"
"  -h                 print this help screen       duh!\n",
		stderr );
	exit( EXIT_SUCCESS );
}
