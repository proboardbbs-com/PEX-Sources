/*
 * This file is part of P2iLC (EXE)
 *
 * Copyright (C) 1995-1997 by Branislav L. Slantchev
 * A Product of Silicon Creations, Inc.
 *
 * This file is distributed under the terms and conditions of the GNU
 * General Public License. For more information, refer to the file
 * Copying.Doc which is included in the archive.
 *
*/
#ifndef __P2ILC_H
#define __P2ILC_H

#include "typedef.h"
#include "stdmac.h"

typedef struct
 {
   char     log[71];
   word     loglevel;                /* See --- Loglevels */
   char     prefix[31];              /* Always added */
   char     hidden[10][31];          /* Strip these if they are in # */
   char     postfix[31];             /* Always appended */
   long     flags;                   /* See --- Miscellaneous flags */
   long     flags_reserved;          /* Reserved flags */
   word     audio;                   /* See --- Audio flags */
   byte     synchtimer;              /* Number of seconds for sync */
   byte     crashexit;               /* Mail exit */
   byte     bbs300;
   byte     bbs1200;
   byte     bbs1275;
   byte     bbs2400;
   byte     bbs4800;
   byte     bbs9600;
   byte     bbs19200;
   byte     bbs38400;
   word     modembaud;               /* 30=300, 24=2400, etc. */
   byte     modemport;               /* 1-255 (COM1=1, COM2=2, etc.) */
   byte     modemdelay;              /* 1/10 seconds delay / line sent */
   char     b300msg[16];
   char     b1200msg[16];
   char     b1275msg[16];
   char     b2400msg[16];
   char     b4800msg[16];
   char     b9600msg[16];
   char     b19200msg[16];
   char     b38400msg[16];
   char     errormsg[16];
   char     busymsg[16];
   char     carriermsg[16];
   char     okmsg[16];
   char     ringmsg[16];
   char     nodialmsg[16];
   char     noanswmsg[16];
   char     voicemsg[16];
   char     escapestr[11];
   char     offhookstr[11];
   char     reconnectstr[11];
   char     init1[50];
   char     init2[50];
   char     init3[50];
   char     resetstr[50];
   char     downstr[50];
   char     hangupstr[11];
   char     dialstr[11];
   char     modemanswer[11];
   byte     answerdelay;
   byte     begin_hour;
   byte     begin_minute;
   byte     end_hour;
   byte     end_minute;
   byte     retrybusy;
   byte     retryresend;
   byte     retrydelay;
   char     reqlist[71];             /* List to scan for reqable dirs    */
   char     reqalias[71];            /* Magic filenames                  */
   char     reqmessage[71];          /* Appended to FAILED REQUEST message */
   byte     reqtype;                 /* Bit field                        */
   byte     reqmaxfiles;             /* Max number of files to send on 1 req*/
   word     reqmaxtime;              /* Maximum number of minutes for req*/
   word     reqmaxsize;              /* Maximum size (in KB) for req     */
   word     reqminbaud;              /* Minimum baudrate for req         */
   byte     reqstarthr;              /* Start time for file requests, can be*/
   byte     reqstartmin;             /* combined with the reqdays field. */
   byte     reqendhr;
   byte     reqendmin;
   byte     reqdays;                 /* 7 6 5 4 3 2 1    0x80==All days */
   char     bbsname[11];
   char     beforebbsbanner[71];
   struct
	 {
	  char     cmd[61];
	  char     title[26];
	  byte     behavior;             /* 1-Pause, 2-Process msg base */
	}
	key[24];
   byte     color[11];
   byte     keep_history;
   char     slavepwd[21];
   char     ineventfile[71];
   char     mailonlyfile[71];
   struct
	{
	  char     wakeupstr[40];
	  byte     errorlevel;
	}
	externmail[10];
   byte     audio_begin_hour;
   byte     audio_begin_minute;
   byte     audio_end_hour;
   byte     audio_end_minute;
   word     min_undial_cost;
   char     RESERVERAT[886];
   char     extrnreq  [71];
   char     modem_name[61];            /* modem name, for 'modem selection' */
 }
 _mailer;

typedef struct
	{
	char    path[65];      /* Path if "board==0", otherwise empty (65) */
	byte    ftype;         /* Folder type                              */
	char    areatag[39];   /* Echomail area tag                        */
	byte    origin;        /* Default origin line, 0-19                */
	char    title[41];     /* Title to appear on screen                */
	byte    useaka;        /* AKA to use, 0==primary                   */
	word    board;         /* QuickBBS/RemoteAccess/WC board number    */
	word    upzone;        /* Uplink zone                              */
	word    upnet;         /* Uplink net                               */
	word    upnode;        /* Uplink node                              */
	word    uppoint;       /* Uplink point                             */
	long    behave;        /* Behavior, see above                      */
	long    hiwater;       /* Highwater mark for echomail              */
	long    pwdcrc;        /* CRC32 of password or -1L if unprotected  */
	long    userok;        /* Users with initial access                */
	long    accflags;      /* access flags, for network environment    */
	char    reserved[8];   /* for future expansion                     */
	}
	_eFOLDER;

typedef struct
 {
   char     macrokey[24][61];        /* F1-F12, Shift F1-F12 */
   byte     margin;
   word     msgbits;
   long     flags;
   char     origin[20][61];
   byte     color[15];
   long     netfolderflags;
   byte     translate_in[256];
   byte     translate_out[256];
   char     qbase[71];
   char     WCmain[71];
   char     RESERVERAT[255];
   long     echoflags;
   _eFOLDER BBSnet;
   _eFOLDER dupes;
   _eFOLDER badecho;
   char     echolog[65];
   char     IMEWork[71];
 }
 _editor;

typedef struct
 {
   char     systempath[71];
   char     mailpath[71];
   char     swap_path[71];
   char     semaphore[71];
   char     secfilespath[71];
   char     infilepath[71];
   char     packetpath[71];
   char     nodelistpath[71];
   word     countrycode;
   struct
	 {
	  word     zone;
	  word     net;
	  word     node;
	  word     point;
	}
	aka[11];
   long     flags;
   byte     blackout_time;
   struct
	{
	  char     name[37];
	  long     pwdcrc;           /* Crc-32 of user password, -1L No pwd */
	  dword    flags;
	}
	user[10];
   dword    exitpwdcrc;          /* Password for DOS shell, exits, etc. */
   dword    exitflags;           /* Which flags should be protected */
   char     filler[760];         /* contains domain stuff */
   char     systemname[50];      /* site_info - system name */
   char     systemloc[40];       /* site_info - location */
   char     systemphone[24];     /* site_info - phone */
   char     fill[6];             /* 6 loose bytes :) */
   char     systemflags[20];     /* site_info - nodelist flags */
   char     systemcountry[26];   /* site_info - country */
   char     serial[14];          /* serial number */
   char     outecho[66];         /* outbound echomail packets */
   char     reserved[10];        /* reserved space */
 }
 _shared;

typedef struct
 {
   char     initstring[41];      /* Init string                         */
   word     scrollsize;          /* Max memory to use for buffer (in K) */
   byte     emulation;           /* 0=TTY, 1=ANSI, 2=VT52, 3=VT100      */
   byte     protocol;            /* Index in protocol list              */
   char     shiftkey[12][31];    /* Shift F1-F12 macro settings         */
   char     ctrlkey[12][31];     /* Ctrl  F1-F12 macro settings         */
   char     downloadpath[60];    /* Default download path               */
   char     uploadpath[60];      /* Default upload path                 */
   byte     translate_in[256];   /* Translation table - modem->screen   */
   byte     translate_out[256];  /* Translation table - screen->modem   */
   byte     retrywait;           /* Seconds to wait before next dial..  */
   dword    flags;               /* Behavior, sounds, flashes.. etc.    */
   dword    directorypwd;        /* CRC-32 of password to enter phoneDir*/
   char     editor[60];          /* Invoked with Alt-I                  */
   char     RESERVERAT[796];     /* Reserved space                      */
   struct
	 {
	word     zone;
	word     net;
	word     node;
	word     point;
	  }
	  newaka[21];
 }
 _terminal;

typedef struct
 {
   char     port;       /* 0 LPT1, 1 LPT2, 2 LPT3, 3 COM1, 4 COM2 */
   char     baud;       /* 0 9600, 1 4800, 2 2400, 3 1200 */
   byte     stopbits;   /* 0x00 - 1, 0x01 - 2 */
   byte     wordlength; /* 0x00 - 7, 0x01 - 8 */
   byte     parity;     /* 0x00 - Even, 0x01 - Odd, 0x02 - None */
   byte     pagelen;
   long     behavior;
   char     init[71];
   char     reset[71];
   char     bold_on[31];
   char     bold_off[31];
   char     pr1RESERVED[62];
   char     italic_on[31];
   char     italic_off[31];
   byte     pagewidth;           /* Width in columns of a page */
   byte     leftmargin;          /* Left margin, ie. start printing at column*/
   byte     footer;              /* Footer margin, ie. leave nn lines */
   byte     header;              /* Header margin, ie. skip nn lines */
   byte     translate_out[256];  /* Translation table - disk->printer */
   char     pr2RESERVED[100];
 }
 _printer;

#define IM_THISREV 0x0100

struct _ctl
 {
   char      fingerprint[5];  /* Must contain "JoHo" */
   word      sysrev;          /* Must contain THISREV above */
   dword     ctlcrc;          /* CRC-32 of struct excluding the 1st 11 bytes */
   _mailer   m;
   _editor   e;
   _shared   s;
   _terminal t;
   _printer  p;
   dword     ctlcrc2;            /* CRC-32 of all the above */
 };

/////////////////////////////////////////////////////////////////////////////

typedef struct{
	ushort year;                           /* 1990 - xxxx */
	ushort month;                          /*    1 - 12   */
	ushort day;                            /*    1 - 31   */
	ushort hour;                           /*    0 - 23   */
	ushort min;                            /*    0 - 59   */
	ushort sec;                            /*    0 - 59   */
	ushort zone;
	ushort net;
	ushort node;
	ushort point;
	char   system_name[31];
	char   city[41];
	ulong  rcvd_bytes;
	ulong  sent_bytes;
	ushort cost;
} MAILHIST;

typedef struct{
	char   system_name[31];
	char   location[41];
	ushort zone;
	ushort net;
	ushort node;
	ushort point;
	ulong  time;                           /* UNIX-style timestamp */
} LASTCALL;

typedef struct{
	char packets[71];         // packet dir for this mail server
	char logfile[71];         // log filename for this mail server
	char systemname[50];      // site_info - system name
	char systemloc[40];       // site_info - location
	char systemcountry[26];   // site_info - country
	char systemphone[24];     // site_info - phone
	char systemflags[20];     // site_info - nodelist flags
} SITEINFO;

#endif /* __P2ILC_H */
