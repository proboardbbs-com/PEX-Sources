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

#if !defined( __FILENAV_H )
#define __FILENAV_H

#include "pbinc.h"

#if defined( PB_SDK )
	#include <pb_sdk.h>
#else
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
#endif

#if defined( __cplusplus )
	extern "C" {
#endif

#define FMN_NOACCESS   0x01       /* user has no access to this area     */
#define FMN_ALLGROUPS  0x02       /* area belongs to all groups          */
#define FMN_FREE       0x04       /* free area                           */
#define FMN_CDROM      0x08       /* cdrom area                          */
#define FMN_NOTOPS     0x10       /* no tops                             */
#define FMN_GROUP      0x20       /* group definition                    */
#define FMN_TAGGED     0x40       /* this area has been tagged           */

#define NAME_LEN         90       /* length of name text (with codes)    */
#define Y_START           2       /* this is the last line of the title  */
#define MAX_DATA        128

#define ClearScr()     printf( "[0;40;37m[2J" );

/* status line definitions */
#define SL_GROUPS         0       /* file and message group menu bar     */
#define SL_FILAREAS       1       /* file areas menu bar                 */
#define SL_FILSEARCH      2       /* file search menu bar                */
#define SL_FILWHERE       3       /* limit file search menu bar          */
#define SL_TRANSFER       4       /* file transfer menu bar              */

/* search modes */
#define SRCH_ALL          1       /* search in all areas                 */
#define SRCH_AREA         2       /* search in current area              */
#define SRCH_GROUP        3       /* search in areas belonging to group  */
#define SRCH_TAGGED       4       /* search in tagged areas              */

/* commands enumeration */
enum{ cmQuit, cmOk, cmNoEntry };

typedef struct{
	int  num;
	word flags;
	char name[NAME_LEN];
	byte groups[4];
} FMN_STRUCT;

extern int  _clrBracket;             /* color for the brackets []          */
extern int  _clrDigit;               /* color for the digits               */
extern int  _clrDotOne;              /* color for the first three dots     */
extern int  _clrDotTwo;              /* color for the dot fill on name     */
extern int  _clrName;                /* color for the name string          */
extern int  _clrTagged;              /* color for the name if tagged       */
extern int  _clrFlagOne;             /* color for the first flag letter    */
extern int  _clrFlagTwo;             /* color for the second flag letter   */
extern int  _clrFlagThree;           /* color for the third flag text      */

extern char _systemPath[MAXPATH];

extern int  _numTagged;
extern int  listFunc;
extern bool listNoParam;
extern char listDataStr[MAX_DATA];
extern int  nameFunc;
extern bool nameNoParam;
extern char nameDataStr[MAX_DATA];
extern int  keywordFunc;
extern bool keywordNoParam;
extern char keywordDataStr[MAX_DATA];
extern int  sendFunc;
extern bool sendNoParam;
extern char sendDataStr[MAX_DATA];
extern int  receiveFunc;
extern char receiveDataStr[MAX_DATA];
extern int  editFunc;
extern char editDataStr[MAX_DATA];

#if defined( __cplusplus )
	}
#endif
#endif
