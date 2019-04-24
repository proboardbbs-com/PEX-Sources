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
#if !defined( __LISTAREA_H )
#define __LISTAREA_H

#include "pbstruct.h"
#include <stdio.h>

extern ushort exclude_cdrom;
extern ushort exclude_lines;
extern ushort single_line;
extern ushort days_old;
extern char   *progname;
extern unsigned int area_kbytes;
extern unsigned int area_files;



#if defined( __cplusplus)
	extern "C" {
#endif

int listarea( int num, const FILEAREA *fa, FILE *fp );

#if defined( __cplusplus)
	}
#endif

#endif
