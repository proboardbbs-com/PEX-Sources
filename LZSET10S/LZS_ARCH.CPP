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
#include <opentry.h>
#include <opstring.h>
#include "lzs_cmds.h"
#include "../lzconfig.h"

extern boolean    lz_Dirty;
extern FrameArray lz_FrameType;
extern ColorSet   lz_Colors;

typedef struct
{
	char    path[80];
	char    options[36];
	boolean enabled;
	boolean swap;
	char    header[30];
} ArchiveRecord;

static word
InitArchiveScreen(EntryScreen &ES, ColorSet &EsColors, ArchiveRecord &UR)
{
	char Pic[255];
	long WinOptions = wBordered | wClear | wUserContents;

	if( !ES.InitCustom(14, 8, 69, 14, EsColors, WinOptions) )
		return ES.InitStatus;

	ES.wFrame.SetFrameType(lz_FrameType);
	ES.EnableExplosions(15);
	ES.wFrame.AddShadow(shBR, shSeeThru);
	ES.wFrame.AddHeader(UR.header, heTR);
	ES.esOptionsOff(esMousePage);
	ES.esFieldOptionsOn(efCursorToEnd | efClearFirstChar);
	ES.esFieldOptionsOff(efInsertPushes | efAllowEscape |
		efShowReadChar | efDefaultAccepted);
	ES.SetWrapMode(WrapAtEdges);

	CharStr('!', 79, Pic);
	ES.AddStringField("Archiver path   ", 2, 3, Pic, 2, 20, 35,
		hi_ArchPath, UR.path);
	CharStr('X', 35, Pic);
	ES.AddStringField("À Options       ", 3, 3, Pic, 3, 20, 35,
		hi_Options, UR.options);
	ES.AddYesNoField("Enabled         ", 5, 3, "Y", 5, 20,
		hi_Enabled, UR.enabled);
	ES.AddYesNoField("Swap to EMS/XMS ", 6, 3, "Y", 6, 20,
		hi_Swap, UR.swap);

	if( ES.RawError() ) ES.Done();
	return ES.RawError();
}

void
ArchiverSetup( int which )
{
	char          *path, *options;
	Boolean       *enabled, *swap;
	char          *header;
	EntryScreen    dlg;
	ArchiveRecord  rec;

	// get pointers to the correct info in lz_Config
	switch( which )
	{
		case ARCHIVER_ZIP:
			path    = lz_Config.zip.path;
			options = lz_Config.zip.options;
			enabled = &lz_Config.zip.enabled;
			swap    = &lz_Config.zip.swap;
			header  = " ZIP Archiver Setup ";
			break;
		case ARCHIVER_RAR:
			path    = lz_Config.rar.path;
			options = lz_Config.rar.options;
			enabled = &lz_Config.rar.enabled;
			swap    = &lz_Config.rar.swap;
			header  = " RAR Archiver Setup ";
			break;
		case ARCHIVER_ARJ:
			path    = lz_Config.arj.path;
			options = lz_Config.arj.options;
			enabled = &lz_Config.arj.enabled;
			swap    = &lz_Config.arj.swap;
			header  = " ARJ Archiver Setup ";
			break;
		case ARCHIVER_LHA:
			path    = lz_Config.lha.path;
			options = lz_Config.lha.options;
			enabled = &lz_Config.lha.enabled;
			swap    = &lz_Config.lha.swap;
			header  = " LHA Archiver Setup ";
			break;
		case ARCHIVER_ZOO:
			path    = lz_Config.zoo.path;
			options = lz_Config.zoo.options;
			enabled = &lz_Config.zoo.enabled;
			swap    = &lz_Config.zoo.swap;
			header  = " ZOO Archiver Setup ";
			break;
		default:
			return;
	}

	// setup the record
	strcpy(rec.path, path);
	strcpy(rec.options, options);
	strcpy(rec.header, header);
	rec.enabled = *enabled;
	rec.swap    = *swap;

	if( 0 == InitArchiveScreen(dlg, lz_Colors, rec) )
	{
		dlg.Draw();
		dlg.Process();
		dlg.Erase();
		dlg.Done();

		if( strcmp(rec.path, path) )
		{
			strcpy(path, rec.path);
			lz_Dirty = TRUE;
		}
		if( strcmp(rec.options, options) )
		{
			strcpy(options, rec.options);
			lz_Dirty = TRUE;
		}
		if( rec.enabled != *enabled )
		{
			*enabled = Boolean(rec.enabled);
			lz_Dirty = TRUE;
		}
		if( rec.swap != *swap )
		{
			*swap = Boolean(rec.swap);
			lz_Dirty = TRUE;
		}
	}
}
