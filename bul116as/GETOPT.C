/*
 BULETMAN ProBoard PEX Version
 Copyright (C) 1995 by Branislav L. Slantchev

 This file is part of the PB-GNU Bulletin Manager.

 BULETMAN is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2.

 BULETMAN is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with BULETMAN; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*/


#if !defined( __GETOPT_H )
	#include "getopt.h"
#endif

#if !defined( EOS )
	#define EOS '\0'
#endif
#if !defined( EOF )
	#define EOF (-1)
#endif

#define __SWITCH(c) ( '-' == (c) || '/' == (c) )
#define __PRERR(s,c) if( opterr ){ \
	printf( "%s: %s -- '%c'\n", strlwr(argv[0]), (s), (c) );\
}

int   optopt;         /* holds the option character found       */
int   optind = 1;     /* index into the argv array              */
int   opterr = 1;     /* 1 to report errors, 0 to suppress them */
char *optarg;         /* if option takes argument, points to it */

	int
getopt( int argc, char *argv[], const char *opts )
{
	static char lastfix = 1;
	char *p;

	/* check for option switch character */
	if( 1 == lastfix ){
		if( optind >= argc || !__SWITCH(argv[optind][0]) ||
			EOS == argv[optind][1] ){
			return EOF;
		}
		else if( 0 == strcmp( "--", argv[optind] ) ){
			optind++;
			return EOF;
		}
	}

	/* see if the option is valid */
	optopt = argv[optind][lastfix];
	if( NULL == (p = strchr(opts, optopt)) || ':' == optopt ){
		__PRERR( "unknown option", optopt );
		optind++;
		lastfix = 1;
		return '?';
	}

	/* see if we want an argument with that option */
	if( *++p == ':' ){
		/* any more characters in same arg */
		if( EOS == argv[optind][++lastfix] ){
			if( ++optind >= argc ){
				__PRERR( "option requires an argument", optopt );
				lastfix = 1;
				return '?';
			}
			optarg = argv[optind];
		}
		else{
			optarg = &argv[optind][lastfix];
		}
		optind++;
		lastfix = 1;
	}
	else{
		/* if we reached end of string, get next argument */
		if( EOS == argv[optind][++lastfix] ){
			optind++;
			lastfix = 1;
		}
		optarg = NULL;
	}

	/* return the option */
	return optopt;
}
