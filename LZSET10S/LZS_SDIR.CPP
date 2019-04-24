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
#include <opdefine.h>
#include <string.h>
#include <opcolor.h>
#include <opentry.h>
#include <opstring.h>
#include "lzs_cmds.h"
#include "../lzconfig.h"
#include "file.h"

extern FrameArray lz_FrameType;
extern boolean    lz_Dirty;
extern ColorSet   lz_Colors;

typedef struct
{
	char system[80];
	char ansiMain[80];
	char procAnsi[80];
	char globalHeader[80];
	char globalFooter[80];
	char areaHeader[80];
	char areaFooter[80];
} SysdirRecord;

static word
InitSysdirDlg(EntryScreen &ES, ColorSet &EsColors, SysdirRecord &UR)
{
	char Pic[255];
	long options = wBordered | wClear | wUserContents;

	if( !ES.InitCustom(9, 7, 68, 18, EsColors, options) )
		return ES.InitStatus;

	ES.wFrame.SetFrameType(lz_FrameType);
	ES.EnableExplosions(15);
	ES.wFrame.AddShadow(shBR, shSeeThru);
	ES.wFrame.AddHeader(" System Paths ", heTR);
	ES.esFieldOptionsOff(efInsertPushes | efAllowEscape |
		efShowReadChar | efDefaultAccepted);
	ES.esFieldOptionsOn(efAutoAdvanceChar | efAutoAdvanceCursor |
		efCursorToEnd | efTrimBlanks | efClearFirstChar);
	ES.SetWrapMode(WrapAtEdges);

	CharStr('!', 79, Pic);

	ES.AddStringField("System directory  ", 2, 3, Pic, 2, 22, 37,
		hi_System, UR.system);
	ES.AddStringField("Main screen       ", 4, 3, Pic, 4, 22, 37,
		hi_AnsiMain, UR.ansiMain);
	ES.AddStringField("Processing screen ", 5, 3, Pic, 5, 22, 37,
		hi_ProcAnsi, UR.procAnsi);
	ES.AddStringField("Allfiles header   ", 7, 3, Pic, 7, 22, 37,
		hi_GlobalHeader, UR.globalHeader);
	ES.AddStringField("Allfiles footer   ", 8, 3, Pic, 8, 22, 37,
		hi_GlobalFooter, UR.globalFooter);
	ES.AddStringField("Area header       ", 10, 3, Pic, 10, 22, 37,
		hi_AreaHeader, UR.areaHeader);
	ES.AddStringField("Area footer       ", 11, 3, Pic, 11, 22, 37,
		hi_AreaFooter, UR.areaFooter);

	if( ES.RawError() ) ES.Done();
	return ES.RawError();
}

void
SystemPaths()
{
	SysdirRecord rec;
	EntryScreen  dlg;

	strcpy(rec.system, lz_Config.sys_Path);
	strcpy(rec.ansiMain, lz_Config.sys_MainFile);
	strcpy(rec.procAnsi, lz_Config.sys_ProcFile);
	strcpy(rec.globalHeader, lz_Config.text_HeaderAll);
	strcpy(rec.globalFooter, lz_Config.text_FooterAll);
	strcpy(rec.areaHeader, lz_Config.text_Header);
	strcpy(rec.areaFooter, lz_Config.text_Footer);

	if( 0 == InitSysdirDlg(dlg, lz_Colors, rec) )
	{
		dlg.Draw();
		dlg.Process();
		dlg.Erase();
		dlg.Done();

		if( strcmp(rec.system, lz_Config.sys_Path) )
		{
			file_appchr(lz_Config.sys_Path, rec.system);
			lz_Dirty = TRUE;
		}
		if( strcmp(rec.ansiMain, lz_Config.sys_MainFile) )
		{
			strcpy(lz_Config.sys_MainFile, rec.ansiMain);
			lz_Dirty = TRUE;
		}
		if( strcmp(rec.procAnsi, lz_Config.sys_ProcFile) )
		{
			strcpy(lz_Config.sys_ProcFile, rec.procAnsi);
			lz_Dirty = TRUE;
		}
		if( strcmp(rec.globalHeader, lz_Config.text_HeaderAll) )
		{
			strcpy(lz_Config.text_HeaderAll, rec.globalHeader);
			lz_Dirty = TRUE;
		}
		if( strcmp(rec.globalFooter, lz_Config.text_FooterAll) )
		{
			strcpy(lz_Config.text_FooterAll, rec.globalFooter);
			lz_Dirty = TRUE;
		}
		if( strcmp(rec.areaHeader, lz_Config.text_Header) )
		{
			strcpy(lz_Config.text_Header, rec.areaHeader);
			lz_Dirty = TRUE;
		}
		if( strcmp(rec.areaFooter, lz_Config.text_Footer) )
		{
			strcpy(lz_Config.text_Footer, rec.areaFooter);
			lz_Dirty = TRUE;
		}
	}
}
