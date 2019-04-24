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
#include <string.h>
#include <oppick.h>
#include <opstring.h>

extern ColorSet   lz_Colors;
extern FrameArray lz_FrameType;
extern boolean    lz_Dirty;

typedef char     csArray[256][6];
typedef csArray* csArrayPtr;

class CharSelector: public PickList
{
	csArrayPtr csItemPtr;
	void csItemStringPrim(word I, char* IString);

public:
	boolean Init(byte X1, byte Y1, byte X2, byte Y2);

	boolean      InitCustom(byte X1, byte Y1, byte X2, byte Y2,
							ColorSet &Colors, long Options);
	virtual void Done();
	virtual void ItemString(word, pkMode mode, pkItemType &type, char* s);
	void         SetChar(char Ch);
	char         GetChar();
};

/////////////////////////////////////////////////////////////////////////////
// CharSelector methods
/////////////////////////////////////////////////////////////////////////////
boolean
CharSelector::Init(byte X1, byte Y1, byte X2, byte Y2)
{
	return CharSelector::InitCustom(X1,Y1,X2,Y2,lz_Colors,DefWindowOptions);
}

boolean
CharSelector::InitCustom(byte X1, byte Y1, byte X2, byte Y2,
						 ColorSet &Colors, long Options)
{
	word I;
	csItemPtr = NULL;

	if( !PickList::InitAbstract(X1, Y1, X2, Y2, Colors, Options, 9, 256,
								PickSnaking, SingleChoice) )
	{
		return FALSE;
	}

	// try to pre-calculate the strings and store them on the heap
	if( GetMemCheck((void *&)csItemPtr, sizeof(csArray)) )
	{
		for( I = 0; I <= 255; I++ ) csItemStringPrim(I, (*csItemPtr)[I]);
	}

	return TRUE;
}

void
CharSelector::Done()
{
	FreeMemCheck(csItemPtr);
	PickList::Done();
}

void
CharSelector::ItemString(word Item, pkMode Mode, pkItemType &IType,
						 char *IString)
{
	if( csItemPtr != NULL ) strcpy(IString, (*csItemPtr)[Item-1]);
	else csItemStringPrim(Item-1, IString);

	if( pkOptionsAreOn(pkSetDefault) )
		if( Item == GetDefaultChoice() )
			IType = pkAlternate;
	Insert(" ", IString, 0);
	strcat(IString, " ");
}

void
CharSelector::SetChar(char Ch)
{
	SetInitialChoice(((byte)Ch)+1);
}

char
CharSelector::GetChar()
{
	return (char)GetLastChoice() - 1;
}

void
CharSelector::csItemStringPrim(word index, char *s)
{
	switch( (char)index )
	{
		case  00: strcpy(s, "0   0"); break;
		case ' ': strcpy(s, "32 sp"); break;
		default : sprintf(s, "%-4d%c", index, (char)index);
	}
}

////////////////////////////////////////////////////////////////////////////
// the public function called by the setup program
////////////////////////////////////////////////////////////////////////////
void
SelectSymbol(byte *symbol, char *header)
{
	CharSelector CS;
	long         options = DefWindowOptions | wBordered;

	if( CS.InitCustom(29, 4, 74, 21, lz_Colors, options) )
	{
		CS.EnableExplosions(15);
		CS.wFrame.SetFrameType(lz_FrameType);
		CS.wFrame.AddHeader(header, heTR);
		CS.wFrame.AddShadow(shBR, shSeeThru);
		CS.wFrame.AddCustomScrollBar(frBB,0,MaxLongInt,1,1,'²','°',lz_Colors);
		CS.SetChar(*symbol);
		CS.Draw();
		CS.Process();

		if( ccQuit != CS.GetLastCommand() && *symbol != CS.GetChar() )
		{
			*symbol = CS.GetChar();
			lz_Dirty = TRUE;
		}

		CS.Erase();
		CS.Done();
	}
}
