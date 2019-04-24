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
#include "templ.h"
#include "str.h"
#include <time.h>

typedef unsigned char uchar;

typedef struct
{
	uchar key;                    /* key that would trigger the fn call  */
	char  *(*fn)(void);     /* call fn that will return subst text */
} TEMPLATE;

char *curTime( void );
char *curDate( void );
char *areaKbytes( void );
char *areaFiles( void );
char *areaName( void );
char *areaNumber( void );
char *totalFiles( void );
char *totalKbytes( void );
char *totalAreas( void );
char *daysOld( void );

TEMPLATE template_opt[] =
{
	{ 0x01, curTime     },   /*  9  */
	{ 0x02, curDate     },   /*  9  */
	{ 0x03, areaKbytes  },   /*  8  */
	{ 0x04, areaFiles   },   /*  8  */
	{ 0x05, areaName    },   /* 40  */
	{ 0x06, areaNumber  },   /*  7  */
	{ 0x0b, totalFiles  },   /*  8  */
	{ 0x0e, totalKbytes },   /*  8  */
	{ 0x0f, totalAreas  },   /*  7  */
	{ 0x10, daysOld     },   /*  6  */
	{ 0x00, 0            }
};


void
parse_file(const char *name, FILE *fp_output)
{
	FILE *fp_input;

	if( !(fp_input = fopen( name, "r" )) )
		return;

	for( ;; )
	{
		int found_match = 0;
		int c = fgetc( fp_input );

		if( EOF == c ) break;

		for( TEMPLATE *p = template_opt; p->key; ++p )
		{
			if( p->key == c )
			{
				if( p->fn )
				{
					fputs( p->fn(), fp_output );
					found_match = 1;
				}
				break;
			}
		}

		if( found_match ) continue;
		else fputc( c, fp_output );
	}

	fclose( fp_input );
}

char*
curTime( void )
{
	static char buf[9];

	time_t t = time(NULL);
	strftime( buf, 9, "%H:%M:%S", localtime(&t) );

	return buf;
}


char*
curDate( void )
{
	static char buf[9];

	time_t t = time(NULL);
	strftime( buf, 9, "%d/%m/%y", localtime(&t) );

	return buf;
}

char*
areaKbytes( void )
{
	static char buf[8];
	char temp[10];

	sprintf( temp, "%u", area_kbytes );
	return strcma( buf, temp );
}

char*
areaFiles( void )
{
	static char buf[8]; /* 999,999 */
	char temp[10];

	sprintf( temp, "%u", area_files );
	return strcma( buf, temp );
}

char*
areaName( void )
{
	static char buf[41];
	sprintf( buf, "%-40s", area_name );
	return buf;
}

char*
areaNumber( void )
{
	static char buf[7];  /* 10,000 */
	char temp[10];

	sprintf( temp, "%d", area_number );
	return strcma( buf, temp );
}

char*
totalFiles( void )
{
	static char buf[8]; /* 999,999 */
	char temp[10];

	sprintf( temp, "%lu", total_files );
	return strcma( buf, temp );
}

char*
totalKbytes( void )
{
	static char buf[8];
	char temp[10];

	sprintf( temp, "%lu", total_kbytes );
	return strcma( buf, temp );
}

char*
totalAreas( void )
{
	static char buf[7];
	char temp[10];

	sprintf( temp, "%d", total_areas );
	return strcma( buf, temp );
}

char*
daysOld( void )
{
	static char buf[6]; /* 9,999 */
	char temp[10];

	sprintf( temp, "%u", days_old );
	return strcma( buf, temp );
}
