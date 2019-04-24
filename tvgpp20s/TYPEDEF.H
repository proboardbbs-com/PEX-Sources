/*
 * This file is part of the TurboVision Gadgets++ v2.0 Library
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

#ifndef INCLUDED_TYPEDEF_H
#define INCLUDED_TYPEDEF_H

#ifndef PB_SDK
	#include <stddef.h>
#else
	typedef unsigned int size_t;
#endif

typedef unsigned char  uchar;
typedef unsigned char  byte;
typedef unsigned char  BOOL;

typedef unsigned short ushort;
//typedef unsigned short word;

typedef unsigned int   uint;

typedef unsigned long  ulong;
typedef unsigned long  dword;

#ifndef _TV_VERSION
	typedef enum { False, True } Boolean;
#endif

// function types
typedef Boolean (*fpTestFunc)(const void *item, void *arg);
typedef void    (*fpAppFunc) (void *item, void *arg);
typedef int     (*fpCompFunc)(const void *item1, const void *item2);

typedef unsigned char  DateType[3];         //........day[0], mon[1], year[2]
//.............................day (1..31), month (1..12), year (year - 1900)
typedef unsigned char  TimeType[3];         //........hour[0], min[1], sec[2]
//...............................hour (0..11), minute (0..59), second (0..59)
typedef unsigned char  TimeFrame[7][6];     //...........7-days, 48-slots/day
typedef unsigned char  combinedboards[125]; //..........1,000 areas, bit/area
typedef unsigned short KEY;                 //......for ScanKey return values
typedef unsigned long  accessflags;         //......flags mapped onto 32 bits
typedef unsigned char  DateRaType[9];       //....pascal string with MM-DD-YY
typedef unsigned char  TimeRaType[6];       //.......pascal string with HH:MM

typedef struct{		  //....................standard MS-DOS packed date stamp
	ushort day : 5;   //.............................day of the month (1..31)
	ushort mon : 4;	  //................month of the year (Jan = 1, Dec = 12)
	ushort year: 7;	  //..................................current year - 1980
} datestamp_t;

typedef struct{		  //....................standard MS-DOS packed time stamp
	ushort sec : 5;	  //..............................current seconds (0..59)
	ushort min : 6;	  //..............................current minutes (0..59)
	ushort hour: 5;	  //.................................current hour (0..23)
} timestamp_t;

typedef struct{                   //...............packed date/time structure
	union{
		datestamp_t st_date;	  //................date stamp, MS-DOS format
		ushort      st_ssdate;	  //........same, used for reading as integer
	};
	union{
		timestamp_t st_time;	  //................time stamp, MS-DOS format
		ushort      st_sstime;	  //........same, used for reading as integer
	};
} stamp_t;

#endif /* INCLUDED_TYPEDEF_H */
