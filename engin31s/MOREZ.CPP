/*
 * This file is part of Enginez.
 *
 * Copyright (c) 1995, 1996 Branislav L. Slantchev
 * A Product of Silicon Creations, Inc.
 *
 * This source is distributed under the terms and conditions of the
 * GNU General Library License. A copy of the license is included with
 * this distrbiution (see the file 'Copying.Pbl').
 *
 * Contact: 73023.262@compuserve.com
*/
#ifndef PB_SDK
	#include <stdlib.h>
	#include <dos.h>
	#include <ctype.h>
#endif

#include "profile.h"
#include "file.h"
#include "stdmac.h"
#include "proboard.h"
#include "pblsdk.h"
#include "terminal.h"

// all options are in this global structure
struct{
	Boolean newLine;
	short   fixedRow;
	char    path[MAXPATH];
	char    onEnter;
	char    yes[20];
	char    no[20];
	char    nonStop[20];
} Options;

// this is the structure of the index file
typedef struct{
	ushort size;     // data size in data file
	short  lines;    // # lines in the animation
	ushort delay;    // delay factor b/n steps, 0=static
	short  column;   // start at column#
	long   offset;   // offset of data into the data file
	char   extra[4]; // spare bytes
} ACL_INDEX;

// global constants
char * const szDomain = "Morez";

// local prototypes
void LoadProfile();
void FreeData(char **lines, int maxNum);
void StuffKey(int key);
void SetCursor(Boolean hide);

int
main( int /*argc*/, char *argv[] )
{
	char         **lines;
	int            i;
	ushort         nTotal, nPrompt;
	ACL_INDEX      index;
	FILE          *fp;
	char           dataPath[MAXPATH], indexPath[MAXPATH], aChar;
	Boolean        done = False;
	zTerminal      terminal;
	ansi_interp    ansi;
	avatar_interp  avatar;
	pcboard_interp pcboard;

	randomize();
	LoadProfile();

	terminal.RegisterHandler(&avatar);
	terminal.RegisterHandler(&ansi);
	terminal.RegisterHandler(&pcboard);

	file_chext(dataPath, Options.path, ".adt");
	file_chext(indexPath, Options.path, ".aix");
	// check for file existence like they are
	if( !file_exist(dataPath) || !file_exist(indexPath) )
	{
		// check in program's startup directory
		sprintf(dataPath, "%s%s.adt", file_dirspec(*argv),
									  file_name(Options.path));
		file_chext(indexPath, dataPath, ".aix");
		if( !file_exist(dataPath) || !file_exist(indexPath) )
		{
			Log(LOG_FRIEND, "Enterz: unable to find data files:");
			Log(LOG_FRIEND, "Enterz: %s and %s", dataPath, indexPath);
			return 1;
		}
	}

	// here we have verified the presence of the two data files
	// now, obtain the maximum prompt number and select one randomly
	nTotal = (ushort)l_div(file_size(indexPath), sizeof(ACL_INDEX));
	nPrompt = rand() % nTotal;

	// now load the index for the prompt
	if( 0 == (fp = fopen(indexPath, "rb")) ) return 1;
	fseek(fp, l_mul((long)nPrompt, sizeof(ACL_INDEX)), SEEK_SET);
	fread(&index, sizeof(index), 1, fp);
	fclose(fp);

	// now load the data for the prompt
	if( 0 == (fp = fopen(dataPath, "rb")) ) return 1;
	fseek(fp, index.offset, SEEK_SET);
	lines = (char **)new char* [index.lines];
	if( !lines ) return 1;

	for( i = 0; i < index.lines; ++i )
	{
		ushort datLen;

		fread(&datLen, sizeof(datLen), 1, fp);
		lines[i] = new char [datLen + 1];
		if( !lines[i] )
		{
			FreeData(lines, i);
			fclose(fp);
			return 1;
		}
		fread(lines[i], 1, datLen, fp);
		lines[i][datLen] = EOS;
	}
	fclose(fp);

	// see if we need to do some additional preparations, like
	// an extra line feed and/or fixed display line (row)
	if( Options.newLine ) putchar('\n');
	if( -1 == Options.fixedRow ) Options.fixedRow = wherey();

	// now loop and display the prompt
	SetCursor(True);
	while( !done )
	{
		for( i = 0; i < index.lines; ++i )
		{
			// display line and wait
			gotoxy(index.column, Options.fixedRow);
			terminal.handle(lines[i]);
			aChar = toupper(PeekChar());
			delay(index.delay);
			// this is needed because strchr() treats NUL as part of string
			if( !aChar ) continue;
			// now see if we recognize the key
			if( strchr(Options.yes, aChar) )
			{
				aChar = Options.yes[0];
				done = True;
				break;
			}
			else if( strchr(Options.no, aChar) )
			{
				aChar = Options.no[0];
				done = True;
				break;
			}
			else if( strchr(Options.nonStop, aChar) )
			{
				aChar = Options.nonStop[0];
				done = True;
				break;
			}
			else if( '\r' == aChar )
			{
				aChar = Options.onEnter;
				done = True;
				break;
			}
		}
	}

	// clear the prompt line and restore the colors
	gotoxy(1, Options.fixedRow);
	printf("\x1b[0m\x1b[K");

	// free data, clear line, stuff key and return immediately
	FreeData(lines, index.lines);
	SetCursor(False);
	StuffKey(aChar);

	return 0;
}

// set the global Options structure from INI file or defaults
void
LoadProfile()
{
	zProfile ini;

	ini.Assign(pb_getpath("SILICON.INI", PBDIR_SYS));
	Options.newLine = ini.ReadBool(szDomain, "ExtraCR", True);
	strcpy(Options.path, ini.ReadString(szDomain, "File", "MOREZ"));
	Options.fixedRow = ini.ReadShort(szDomain, "FixedRow", -1);
	Options.onEnter = toupper(ini.ReadString(szDomain, "Default", "Y")[0]);
	strupr(strcpy(Options.yes, ini.ReadString(szDomain, "Yes", "YC")));
	strupr(strcpy(Options.no, ini.ReadString(szDomain, "No", "NA")));
	strupr(strcpy(Options.nonStop, ini.ReadString(szDomain, "Nonstop", "S")));
}

// frees the line array
void
FreeData(char **lines, int maxLine)
{
	for( int i = 0; i < maxLine; ++i ) delete[] lines[i];
	delete[] lines;
}

// stuffs a key in the keyboard buffer
void
StuffKey(int key)
{
	union REGS r;

	r.h.ah = 5;
	r.x.cx = key;
	int86(0x16, &r, &r);
}

void
SetCursor(Boolean hide)
{
	union REGS r;

	r.h.ah = 1;
	r.x.cx = hide ? 0x2000 : 0x0d0e;
	int86(0x10, &r, &r);
}
