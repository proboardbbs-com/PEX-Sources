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


#if !defined( __PBSTRUCT_H )
#define __PBSTRUCT_H

/*
 ***************************************************************************
				Structures that are not available in PB_SDK.H
 ***************************************************************************
*/

/*
 * Standard include file for all PB-Lib files
*/
#if !defined( __MACDEF_H )
	#include <macdef.h>
#endif

/*
 * ProBoard-specific data types
*/
#if !defined( __PBTYPES_H )
	#include <pbtypes.h>
#endif

/*
 * structure of FILESIDX.PB
*/
typedef struct{
	char name[13];       /* Name of the file                            */
	word area;           /* Area this file belongs to                   */
} FILESIDX;

/*
 * structure of FGROUPS.PB record
*/
typedef struct{
	char name[80];       /* Name of this group                          */
	int  level;          /* Level needed to access the group            */
	long flags;          /* Flags needed to access the group            */
	long flagsNot;       /* Reverse flags: should NOT be set for access */
	char extra[10];
} FILEGROUP;

/*
 * structure of MGROUPS.PB record
*/
typedef FILEGROUP MSGGROUP;

/*
 * structure of TIMELOG.PRO record
*/
typedef struct{
	int  numdays;        /* Number of days active                       */
	Date lastdate;       /* Last update                                 */
	long hours[24];      /* # minutes per hour usage (obsolete)         */
	long days[7];        /* # minutes per day usage  (obsolete)         */
	long weeks[53];      /* # minutes per week usage (obsolete)         */
	long totalcalls;     /* Total calls to system                       */
} TIMELOG;

/*
 * structure of ONLINE.PRO record
*/
typedef struct{
	char name[36];       /* Name of user online                         */
	char city[16];       /* City of user online                         */
	int  baud;           /* Baud rate of user                           */
	int  status;         /* 0 = online , <0 = not online or unavailable */
	char extra[8];
} ONLINE;

/*
 * structure of PROTOCOL.PRO record
*/
typedef struct{
	char name[50];       /* Name of protocol                            */
	char key;            /* Hotkey for Protocol                         */
	char flags;          /* Protocol behavior                           */
	char logfile[60];    /* Logfile from protocol                       */
	char ctlfile[60];    /* Control file (redirection file)             */
	char dlcom[128];     /* Download command line                       */
	char ulcom[128];     /* Upload command line                         */
	char dlstring[80];   /* String to place in download control file    */
	char ulstring[80];   /* String to place in upload control file      */
	char dlkeyword[40];  /* Download keyword to search for in logfile   */
	char ulkeyword[40];  /* Upload keyword to search for in logfile     */
	int  wordnr;         /* File name word nr after keyword (1-..)      */
} PROTOCOL;

/*
 * structure of MODEM.PRO record
*/
typedef struct{
	uint baud;               /* Baud rate                               */
	int  port;               /* COM port                                */
	int  quiet;              /* Modem quiet                             */
	int  blanktime;          /* Screen saver: blank screen after x min. */
	char initstring[70];     /* Modem initialization string             */
	char busystring[70];     /* BUSY response string                    */
	char initok[40];         /* OK response string                      */
	char busyok[40];
	char connect300[40];     /* 300 baud connect modem string           */
	char connect1200[40];	 /* 1200 baud connect modem string          */
	char connect2400[40];	 /* 2400 baud connect modem string          */
	char connect4800[40];	 /* 4800 baud connect modem string          */
	char connect9600[40];	 /* 9600 baud connect modem string          */
	char connect19200[40];	 /* 19200 baud connect modem string         */
	char connect38400[40];	 /* 38400 baud connect modem string         */
	char ring[40];           /* RING string                             */
	char answer[40];         /* Answer string                           */
	char connect7200[40];	 /* 7200 baud connect modem string          */
	char connect12000[40];	 /* 12400 baud connect modem string         */
	bool forceanswer;        /* Force answer                            */
	byte extra1[5];
	int  delay;              /* Delay for slow modems                   */
	char connect14400[40];	 /* 14400 baud connect modem string         */
	char initstring2[70];
	char connect16800[40];	 /* 16800 baud connect modem string         */
	char connect28800[40];	 /* 28800 baud connect modem string         */
	char ext_connect1[40];
	char ext_connect2[40];
	int  ext_errlevel1;
	int  ext_errlevel2;
	char extra[36];
} MODEM;

/*
 * structure of USERSPB.BBS record
*/
typedef struct{
	 char name[36];               /* User's name                        */
	 char country[26];            /* User's country                     */
	 char state[26];			  /* User's state                       */
	 char faxPhone[16];			  /* User's fax phone number            */
	 char passWord[16];			  /* User's password                    */
	 char language[9]; 			  /* Selected language name             */
	 Date lastPwdChange;		  /* Date of last password change       */
	 Date lastNewFilesCheck;	  /* Date of last check for new files   */
	  int logLevel;				  /* Log level for this user            */
	  int tbTimeBalance;		  /* TimeBank time balance              */
	  int tbKbBalance;			  /* TimeBank KBytes balance            */
	  int tbTimeWithdrawn;        /* TimeBank time withdrawn            */
	  int tbKbWithdrawn;          /* TimeBank KBytes withdrawn          */
	 uint tbTimeDeposited;        /* TimeBank time deposited            */
	 uint tbKbDeposited;          /* TimeBank KBytes deposited          */
	 uint tbTimeLoaned;           /* TimeBank time loaned               */
	 uint tbKbLoaned;             /* TimeBank KBytes loaned             */
	 Date tbTimePayback;          /* TimeBank time payback              */
	 Date tbKbPayback;            /* TimeBank KBytes payback            */
	 Date tbLastUsed;             /* TimeBank last use                  */
	 uint expLevel;               /* Level to give on expiration        */
	ulong expFlagsOn;             /* Flags to turn on on expiration     */
	ulong expFlagsOff;            /* Flags to turn off on expiration    */
	ulong uFlags;                 /* User flags (see PBUF_???)          */
	uchar mailCheckBoards[125];   /* Check areas, bit/area: max 1,000   */
	ulong totalTimeUsed;          /* Total time used on system, min     */
	 uint qwkMaxMsgsPerArea;      /* QWK max messages per area          */
	 uint qwkMaxMsgs;             /* QWK max messages in packet         */
	 byte qwkArchiver;            /* QWK archiver type                  */
	 byte ripFont;                /* RIP font                           */
	 byte checkMail;              /* Check for mail at logon            */
	 byte checkNewFiles;          /* Check for new files at logon       */
	 byte extra[398];
} USERSPB;


/*
 * Structure of FLAGDESC.PB record
*/
typedef struct{
	char name[40];                /* Description of this flag           */
} FLAGDESC;

/*
 * Structure of TEMPLATE.PB record
*/
typedef struct{
	 char name[61];               /* Name of this template              */
	 char comment[81];            /* Comment to place in user record    */
	 char valMsgFile[9];          /* Validation message filename        */
	 word newLevel;               /* Assign this as the new user level  */
	 word subDays;                /* Days the new level will last       */
	 word expLevel;               /* Assign this level when it expires  */
	 word tbTime;                 /* Bank: deposit time to user account */
	 word tbKBytes;               /* Bank: deposit KB to user account   */
	 word netCredit;              /* Netmail credit user can use        */
	  int logLevel;               /* Log: 0=Fnd,1=Nml,2=Sus,3=Ext,4=Ign */
	 byte hidden;                 /* Set HIDDEN (0=No, 1=Yes, 2=Ignore) */
	 byte noTops;                 /* Set the NOTOPS flag in user record */
	 byte attention;              /* Set ATTENTION flag in user record  */
	 byte noKill;                 /* Set NOKILL flag in user's record   */
	 byte ignoreDL;               /* Set IGNOREDL flag in user record   */
	 bool deleted;                /* Template marked for deletion       */
	 char extra[99];
	dword newFlagsOn;             /* Flags to turn on when validated    */
	dword newFlagsOff;            /* Flags to turn off when validated   */
	dword expFlagsOn;             /* Flags to turn on when expired      */
	dword expFlagsOff;            /* Flags to turn off when expired     */
} TEMPLATE;

/*
 * Structure of AKA.PRO record
*/
typedef struct{
	int zone, net, node, point;   /* FidoNet-compatible matrix address  */
} AKA;

/*
 * Structure of PVTFILE.PRO record
*/
typedef struct{
	DateType date;                /* Date the file was posted           */
	char     fileName[13];        /* FILENAME.EXT of the file           */
	char     toUser[36];          /* Name of user to receive the file   */
	char     fromUser[36];        /* Name of user that posted the file  */
} PVTFILE;

/*
 * Structure of EVENTS.PRO
*/
typedef struct{
	int      type;                /* Event type: 0=Errorlevel,1=Command */
	int      errorLevel;          /* Errorlevel for type 0 events       */
	char     command[80];         /* Command string for type 1 events   */
	TimeType startTime;           /* Start time for the event           */
	int      duration;            /* Duration of the event (in minutes) */
	word     daysEnabled;         /* Days event is enabled, see EVT_??? */
	char     nodeNum;             /* Node number this event is for <128 */
	bool     enabled;             /* True if event is enabled,false=not */
} EVENTS;

/*
 * Structure of TOPS.PRO record
 *
 * NOTE: the TOPS.PRO file contains BEST 20 Users Statistics. It has
 *       7 sections, each of which has 20 entries in the same format
 *       as this structure. Here's the list with the sections:
 *
 *            1. Callers by total number of calls (stats = # of calls)
 *            2. Downloaders by KBytes (stats = # of KBytes)
 *            3. Downloaders by # of files (stats = # of files)
 *            4. Uploaders by KBytes (stats = # of KBytes)
 *            5. Uploaders by # of files (stats = # of files)
 *            6. Message writers (stats = # messages written)
 *            7. Callers by time online (stats = minutes online)
*/
typedef struct{
	char name[36];                /* User's name                        */
	word stats;                   /* Statistics information             */
} TOPS;

/*
 * Header of the language files.
 *
 * NOTE: all language files start with this 815-byte header. The
 *       copyright string should state the author of the language.
*/
typedef struct{
	char  name[41];               /* Title of the language file         */
	bool  avail;                  /* TRUE if available, FALSE otherwise */
	word  level;                  /* Security level needed to use it    */
	dword flags;                  /* Flags needed to use it             */
	char  menuPath[61];           /* Directory for the language menus   */
	char  textPath[61];           /* Directory for the textfiles        */
	char  questPath[61];          /* Directory for the questionnaires   */
	char  copyright[40];          /* Copyright string for the file      */
	char  extra[542];
	 int  numEntries;             /* Number of entries: PB 2.02 = 352   */
} LANGUAGE;

/*
 * Header of an entry in the language file.
 *
 * NOTE:	this header is followed by stringLen bytes for the string(s),
 *			which is followed by numHotkeys bytes for the hotkeys.
 *			Neither the string, nor the hotkeys are NUL-terminated, and
 *			the 8 different strings are separated by 0x0A only. The
 *			maximum length of each string is 200 chars, the maxkeys can
 *			be no more than 80.
*/
typedef struct{
	 int stringLen;               /* Length of string, see NOTE         */
	 int numHotkeys;              /* Number of defined hotkeys          */
	byte foreColor;               /* Color attribute for the strings    */
	byte highColor;               /* Highlight color attribute          */
	byte promptColor;             /* Foreground prompt color            */
	bool hasPrompt;               /* TRUE: has a prompt, FALSE: doesn't */
} LANG_HEADER;

#endif
