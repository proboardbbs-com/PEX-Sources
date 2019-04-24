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
#include "../lzconfig.h"
#include "lzs_cmds.h"

typedef struct
{
	boolean UserDoes;
	boolean Doing;
	char    text[37];
} DropFileRecord;

extern FrameArray lz_FrameType;
extern ColorSet   lz_Colors;
extern LZCONFIG   lz_Config;
extern boolean    lz_Dirty;

static word
InitDropFile(EntryScreen &ES, ColorSet &EsColors, DropFileRecord &UR)
{
	char pic[255];
	long options = wBordered|wClear|wUserContents;

	if( !ES.InitCustom(16, 10, 72, 14, EsColors, options) )
		return ES.InitStatus;

	ES.wFrame.SetFrameType(lz_FrameType);
	ES.EnableExplosions(15);
	ES.wFrame.AddShadow(shBR, shSeeThru);
	ES.wFrame.AddHeader(" Useron Support ", heTR);
	ES.esFieldOptionsOn(efClearFirstChar|efAutoAdvance);
	ES.esFieldOptionsOff(efTrimBlanks|efShowReadChar|efDefaultAccepted);
	ES.SetWrapMode(WrapAtEdges);

	ES.AddYesNoField("UserDoes      ", 2, 3, "Y", 2, 23,
		hi_UserDoes, UR.UserDoes);
	ES.AddYesNoField("Doing (Fe-line)", 3, 3, "Y", 3, 23,
		hi_Doing, UR.Doing);
	CharStr('X', 36, pic);
	ES.AddStringField("Text to write", 4, 3, pic, 4, 23, 33,
		hi_UseronText, UR.text);

	if( ES.RawError() ) ES.Done();
	return ES.RawError();
}

void
DropFile()
{
	DropFileRecord record;
	EntryScreen    dialog;

	record.UserDoes = lz_Config.drop_UserDoes;
	record.Doing = lz_Config.drop_Doing;
	strcpy(record.text, lz_Config.drop_Text);

	if( 0 == InitDropFile(dialog, lz_Colors, record) )
	{
		dialog.Draw();
		dialog.Process();
		dialog.Erase();
		dialog.Done();

		if( record.UserDoes != lz_Config.drop_UserDoes )
		{
			lz_Config.drop_UserDoes = Boolean(record.UserDoes);
			lz_Dirty = TRUE;
		}
		if( record.Doing != lz_Config.drop_Doing )
		{
			lz_Config.drop_Doing = Boolean(record.Doing);
			lz_Dirty = TRUE;
		}
		if( 0 != strcmp(record.text, lz_Config.drop_Text) )
		{
			strcpy(lz_Config.drop_Text, record.text);
			lz_Dirty = TRUE;
		}
	}
}
