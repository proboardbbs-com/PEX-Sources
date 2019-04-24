/*
 * This file is part of LZSETUP (Configuration program for Listerz)
 *
 * Copyright (c) 1997 Branislav L. Slantchev (gargoyle)
 * A fine product of Silicon Creations, Inc.
 *
 * This file is released under the terms and conditions of the GNU
 * General Public License Version 2. The full text of the license is
 * supplied in the Copying.Doc file included with this archive. This
 * free software comes with absolutely no warranty, as outlined in the
 * licensing text. You are not allowed to remove this copyright notice.
 *
 * Contact: Branislav L. Slantchev at 73023.262@compuserve.com
*/
#include <stdio.h>
#include <string.h>
#include <opcrt.h>
#include <dir.h>
#include "file.h"
#include "proboard.h"
#include "../lzconfig.h"

extern void     MsgBox(char *header, char *text, ColorSet &colors);
extern ColorSet lz_Colors;
extern boolean  lz_Dirty;
extern char     lz_ConfigPath[];

void
LoadConfig()
{
	FILE    *fp;
	char    *name = "LISTERZ.CNF";
	char    *msg = "Configuration file not found.\r"
				   "lzSetup will use the built-in default values";

	strcpy(lz_ConfigPath, name);
	if( !file_exist(lz_ConfigPath) )
	{
		strcpy(lz_ConfigPath, pb_getpath(name, PBDIR_PEX));
	}

	memset(&lz_Config, 0x00, sizeof(lz_Config));

	if( 0 != (fp = fopen(lz_ConfigPath, "rb")) )
	{
		fread(&lz_Config, sizeof(lz_Config), 1, fp);
		fclose(fp);
	}
	else
	{
		char *p;

		// display message box with info (defaults loaded)
		MsgBox(" Info ", msg, lz_Colors);

		getcwd(lz_Config.sys_Path, sizeof(lz_Config.sys_Path));
		file_appchr(lz_Config.sys_Path, NULL);
		strcpy(lz_Config.sys_MainFile, "LSZ_MAIN.ANS");
		strcpy(lz_Config.sys_ProcFile, "LSZ_PROC.ANS");
		strcpy(lz_Config.text_HeaderAll, "LSZ_MSTR.HDR");
		strcpy(lz_Config.text_FooterAll, "LSZ_MSTR.FTR");
		strcpy(lz_Config.text_Header, "LSZ_AREA.HDR");
		strcpy(lz_Config.text_Footer, "LSZ_AREA.FTR");

		lz_Config.lbox_Bounds.a.x    = 12;
		lz_Config.lbox_Bounds.a.y    = 7;
		lz_Config.lbox_Bounds.b.x    = 63;
		lz_Config.lbox_Bounds.b.y    = 18;
		lz_Config.lbox_Color         = 0x0C;
		lz_Config.lbox_Hilite        = 0x4A;
		lz_Config.lbox_TagChar       = 'û';
		lz_Config.lbox_TagColor      = 0x0F;
		lz_Config.lbox_TagHilite     = 0x4A;

		lz_Config.cbar_Bounds.a.x    = 12;
		lz_Config.cbar_Bounds.a.y    = 22;
		lz_Config.cbar_Bounds.b.x    = 63;
		lz_Config.cbar_Bounds.b.y    = 22;
		lz_Config.cbar_Color         = 0x04;
		lz_Config.cbar_Background    = 0x00;
		lz_Config.cbar_HotkeyColor   = 0x0C;
		lz_Config.cbar_HiliteColor   = 0x0E;
		lz_Config.cbar_OffColor      = 0x08;
		lz_Config.cbar_OffBackground = 0x00;

		lz_Config.proc_Bounds.a.x    = 14;
		lz_Config.proc_Bounds.a.y    = 7;
		lz_Config.proc_Bounds.b.x    = 66;
		lz_Config.proc_Bounds.b.y    = 15;
		lz_Config.proc_Color         = 0x03;

		lz_Config.area_Bounds.a.x    = 3;
		lz_Config.area_Bounds.a.y    = 19;
		lz_Config.area_Bounds.b.x    = 38;
		lz_Config.area_Bounds.b.y    = 19;
		lz_Config.area_Color         = 0x0F;
		lz_Config.area_Percent.a.x   = 39;
		lz_Config.area_Percent.a.y   = 19;
		lz_Config.area_Percent.b.x   = 43;
		lz_Config.area_Percent.b.y   = 19;

		lz_Config.area_Progbar.bounds.a.x = 3;
		lz_Config.area_Progbar.bounds.a.y = 20;
		lz_Config.area_Progbar.bounds.b.x = 43;
		lz_Config.area_Progbar.bounds.b.y = 20;
		lz_Config.area_Progbar.fgColor    = 0x4C;
		lz_Config.area_Progbar.bgColor    = 0x04;
		lz_Config.area_Progbar.fgChar     = '±';
		lz_Config.area_Progbar.bgChar     = 'þ';

		lz_Config.total_Percent.a.x        = 39;
		lz_Config.total_Percent.a.y        = 21;
		lz_Config.total_Percent.b.x        = 43;
		lz_Config.total_Percent.b.y        = 21;
		lz_Config.total_Progbar.bounds.a.x = 3;
		lz_Config.total_Progbar.bounds.a.y = 22;
		lz_Config.total_Progbar.bounds.b.x = 43;
		lz_Config.total_Progbar.bounds.b.y = 22;
		lz_Config.total_Progbar.fgColor    = 0x4C;
		lz_Config.total_Progbar.bgColor    = 0x04;
		lz_Config.total_Progbar.fgChar     = '±';
		lz_Config.total_Progbar.bgChar     = 'þ';

		lz_Config.total_Areas.a.x    = 62;
		lz_Config.total_Areas.a.y    = 19;
		lz_Config.total_Areas.b.x    = 77;
		lz_Config.total_Areas.b.y    = 19;
		lz_Config.total_Files.a.x    = 62;
		lz_Config.total_Files.a.y    = 20;
		lz_Config.total_Files.b.x    = 77;
		lz_Config.total_Files.b.y    = 20;
		lz_Config.total_Size.a.x     = 62;
		lz_Config.total_Size.a.y     = 21;
		lz_Config.total_Size.b.x     = 77;
		lz_Config.total_Size.b.y     = 21;
		lz_Config.total_ListSize.a.x = 62;
		lz_Config.total_ListSize.a.y = 22;
		lz_Config.total_ListSize.b.x = 77;
		lz_Config.total_ListSize.b.y = 22;
		lz_Config.total_DigitColor   = 0x0B;

		lz_Config.drop_UserDoes = False;
		lz_Config.drop_Doing    = False;
		strcpy(lz_Config.drop_Text, "Using Listerz (The Best)");

		if( 0 != (p = file_scan(NULL, "PKZIP.EXE")) )
		{
			strcpy(lz_Config.zip.path, p);
			strcpy(lz_Config.zip.options, "-ex -m");
			lz_Config.zip.enabled = True;
			lz_Config.zip.swap = True;
		}
		if( 0 != (p = file_scan(NULL, "RAR.EXE")) )
		{
			strcpy(lz_Config.rar.path, p);
			strcpy(lz_Config.rar.options, "m -ep -m5");
			lz_Config.rar.enabled = True;
			lz_Config.rar.swap = True;
		}
		if( 0 != (p = file_scan(NULL, "ARJ.EXE")) )
		{
			strcpy(lz_Config.arj.path, p);
			strcpy(lz_Config.arj.options, "m -e -m1");
			lz_Config.arj.enabled = True;
			lz_Config.arj.swap = True;
		}
		if( 0 != (p = file_scan(NULL, "LHA.EXE")) )
		{
			strcpy(lz_Config.lha.path, p);
			strcpy(lz_Config.lha.options, "m");
			lz_Config.lha.enabled = True;
			lz_Config.lha.swap = True;
		}
		if( 0 != (p = file_scan(NULL, "ZOO.EXE")) )
		{
			strcpy(lz_Config.zoo.path, p);
			strcpy(lz_Config.zoo.options, "ahM:");
			lz_Config.zoo.enabled = True;
			lz_Config.zoo.swap = True;
		}

		lz_Dirty = TRUE;
	}
}


void
SaveConfig()
{
	FILE *fp = fopen(lz_ConfigPath, "wb");

	if( fp )
	{
		fwrite(&lz_Config, sizeof(lz_Config), 1, fp);
		fclose(fp);
	}
	else
	{
		char msg[255];

		sprintf(msg, "Unable to update configuration file\r%s", lz_ConfigPath);
		MsgBox(" Error ", msg, lz_Colors);
	}
}

