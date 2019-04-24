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
#include <stdio.h>
#include "p2ilc.h"

#define _VERSION "v3.0"

static const char LicenseText[] =
"   This program is free software; you can redistribute it and/or modify\n"
"   it under the terms of the GNU General Public License as published by\n"
"   the Free Software Foundation; either version 2 of the License, or\n"
"   (at your option) any later version.\n"
"\n"
"   This program is distributed in the hope that it will be useful,\n"
"   but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
"   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
"   GNU General Public License for more details.\n"
"\n"
"   You should have received a copy of the GNU General Public License\n"
"   along with this program; if not, write to the Free Software\n"
"   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.\n";

/*
 * program startup message
*/
void
print_banner()
{
	fprintf(stderr,
		"P2iLC %s - Update InterMail/FrontDoor WFC/HIS with last ProBoard caller\n"
		"   þ Copyright (C) 1995-1997 by Branislav L. Slantchev /gargoyle\n"
		"   þ A Product of Silicon Creations, Inc. (tm) [%s %s]\n\n",
		_VERSION, __DATE__, __TIME__ );
}

/*
 * print help screen
*/
void
print_help()
{
	fputs("Usage: P2iLC [server#] [-n] [-f] [-s] [-?]\n\n", stdout);
	fputs("       server#   : server whose WFC/HIS to update (defaults to 1)\n", stdout);
	fputs("       -n        : do not update WFC/HIS (useful with -s option)\n", stdout);
	fputs("       -f        : force FrontDoor if both IM and FD variables exist\n", stdout);
	fputs("       -s        : switch InterMail site name and location\n", stdout);
	fputs("       -v        : display the program version number\n", stdout);
	fputs("       -c        : display license note (full text in Copying.Doc)\n", stdout);
	fputs("       -?        : display this (brief) help screen\n\n", stdout);
	fputs("   * The 'server#' is optional. Call with 'p2ilc 2' for node 2\n", stdout);
	fputs("   * If both IM and FD environments exist, defaults to IM (see -f)\n", stdout);
	fputs("   * The '-s' option uses a .CTL file in same directory as the .EXE\n", stdout);
	fputs("   * The switch command is not available when running in FD mode\n", stdout);
}

/*
 * print program version
*/
void
print_version()
{
	fprintf(stderr,
		"   þ Compiled on %s at %s\n   þ Program version: %s\n",
		__DATE__, __TIME__, _VERSION);
}

/*
 * print parts of the license
*/
void
print_license()
{
	fputs(LicenseText, stderr);
}
