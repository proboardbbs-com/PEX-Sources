#ifndef __GETOPT_H
#define __GETOPT_H

/*
 ***************************************************************************
                Simplified command-line options retrieval
 ***************************************************************************
*/

#if 0
#ifndef __PBLIB_H
#include <pblib.h>
#endif
#endif

extern int opterr;
extern int optind;
extern int optopt;
extern char *optarg;

#ifdef __cplusplus
	extern "C"
#endif

int getopt( int argc, char *argv[], const char *opts );

#endif
