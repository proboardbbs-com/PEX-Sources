/*
 * This file is part of P2iLC (EXE)
 *
 * Copyright (C) 1995-1997 by Branislav L. Slantchev
 * A Product of Silicon Creations, Inc.
 *
 * This file is distributed under the terms and conditions of the GNU
 * General Public License. For more information, refer to the file
 * Copying.Doc which is included in the archive.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "pblsdk.h"
#include "file.h"
#include "p2ilc.h"
#include "utils.h"
#include "_pbinit.h"

extern void print_banner();
extern void print_help();
extern void print_license();
extern void print_version();

static void read_options(int argc, char *argv[]);

extern void read_user(int node, USER_REC *pUser);
extern void write_caller(USER_REC *pUser, const char *path);
extern void write_history(USER_REC *pUser, const char *path);
extern void write_site(const char *site, const char *swpath);

struct
{
	Boolean bSwitchSite;
	Boolean bImportCaller;
	char    Caller[MAXPATH];
	char    Site[MAXPATH];
	char    History[MAXPATH];
	char    Switch[MAXPATH];
	int     nNode;
} Options;

int
main( int argc, char *argv[] )
{
	USER_REC CurUser;

    proboard_emulator_init();

	print_banner();
	read_options(argc, argv);
	if( Options.bImportCaller )
	{
		read_user(Options.nNode, &CurUser);
		write_caller(&CurUser, Options.Caller);
		write_history(&CurUser, Options.History);
	}
	if( Options.bSwitchSite ) write_site(Options.Site, Options.Switch);

	proboard_emulator_term();
	return 0;
}

/*
 * parse command line options
*/
void
read_options(int argc, char *argv[])
{
	Boolean      bForceFD = False;
	FILE        *fp;
	char        *pEnviron;
	char         MailerPath[MAXPATH];
	char         History[MAXPATH], Caller[MAXPATH], Site[MAXPATH];
	struct _ctl  MailerConfig;

	// init the options to defaults here
	Options.nNode = 1;
	Options.bSwitchSite = False;
	Options.bImportCaller = True;
	Options.Caller[0]  = EOS;
	Options.History[0] = EOS;
	Options.Site[0]    = EOS;

	// set switch file name
	file_chext(Options.Switch, strupr(argv[0]), ".CTL");

	for( int i = 1; i < argc; ++i )
	{
		// see if this is an option
		if( '-' == argv[i][0] || '/' == argv[i][0] )
		{
			switch( tolower(argv[i][1]) )
			{
				case 's': Options.bSwitchSite = True; break;
				case 'f': bForceFD = True;  break;
				case 'n': Options.bImportCaller = False; break;
				case '?': print_help(); exit(EXIT_SUCCESS);
				case 'v': print_version(); exit(EXIT_SUCCESS);
				case 'c': print_license(); exit(EXIT_SUCCESS);
				default :
					print_help();
					fail("\n   ! Unknown command-line option '%s'\n", argv[i]);
			}
		}
		// or the server number then
		else Options.nNode = atoi(argv[i]);
	}

	// find the system setup file (format is the same for both)
	// also setup the root names for the other files
	if( 0 != (pEnviron = getenv("IM")) && !bForceFD )
	{
		sprintf(MailerPath, "%s\\FD.SYS", pEnviron);
		if( 1 < Options.nNode )
		{
			sprintf(Caller, "LASTCALL.%03d", Options.nNode);
			sprintf(History, "INBOUND.%03d", Options.nNode);
			sprintf(Site, "MAILER.%03d", Options.nNode);
		}
		else
		{
			strcpy(Caller, "LASTCALL.IM");
			strcpy(History, "INBOUND.HIS");
			strcpy(Site, "MAILER.001");
		}
	}
	else if( 0 != (pEnviron = getenv("FD")) )
	{
		Options.bSwitchSite = False;
		sprintf(MailerPath, "%s\\SETUP.FD", pEnviron);
		if( 1 < Options.nNode )
		{
			sprintf(Caller, "LAST%d.FD", Options.nNode);
			sprintf(History, "INB%d.FD", Options.nNode);
		}
		else
		{
			strcpy(Caller, "LASTCALL.FD");
			strcpy(History, "INBOUND.HIS");
		}
	}
	else fail("   ! Unable to locate the correct environment!\n");

	// find out the location of the system directory
	if( 0 != (fp = fopen(MailerPath, "rb")) )
	{
		file_read(fp, &MailerConfig);
		fclose(fp);
	}
	else fail("   ! Unable to read setup file '%s'\n", MailerPath);

	// create the history, lastcaller and (optionally) the site info names
	sprintf(Options.Caller, "%s%s", MailerConfig.s.systempath, Caller);
	sprintf(Options.History, "%s%s", MailerConfig.s.systempath, History);
	if( Options.bSwitchSite )
	{
		sprintf(Options.Site, "%s%s", MailerConfig.s.systempath, Site);
	}
}
