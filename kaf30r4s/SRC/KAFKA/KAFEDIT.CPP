/*
 * This file is part of the KaFKa Kompiler
 * Copyright (C) 1995 by Branislav L. Slantchev
 *
 * Distributed under the GNU General Public License
 * See the file 'copying' for more information about your license.
 *
*/
#include "kafedit.h"
#include "kafcmds.h"
#include "kafcoll.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

////////////////////////////////////////////////////////////////////////////
TIntLine::TIntLine(const TRect &bounds, int aMaxLen, char low, char high):
	TInputLine(bounds, aMaxLen)
{
	lowChar  = low;
    highChar = high;
}

void TIntLine::handleEvent(TEvent &event)
{
	char key = event.keyDown.charScan.charCode;

	if( !(evKeyboard == event.what &&
		  isprint(key) &&
    	  (key < lowChar || key > highChar)) )
    {
    	TInputLine::handleEvent(event);
	}
}

////////////////////////////////////////////////////////////////////////////
TKafEditor::TKafEditor():
	TDialog(TRect(15, 6, 64, 16), "KaF RîCoRD PRoPîRTiîZ"),
	TWindowInit(TKafEditor::initFrame)
{
	options |= ofCenterX | ofCenterY;
	flags &= ~wfClose;

	aName = new TInputLine(TRect(3, 3, 30, 4), 25);
	insert(aName);
	insert(new TLabel(TRect(2, 2, 7, 3), "~N~aMî", aName));

	aSpeed = new TIntLine(TRect(3, 6, 15, 7), 6);
	insert(aSpeed);
	insert(new THistory(TRect(15, 6, 18, 7), aSpeed, hlSpeedList));
	insert(new TLabel(TRect(2, 5, 8, 6), "~S~PîîD", aSpeed));

	aMaxChars = new TIntLine(TRect(19, 6, 30, 7), 4);
	insert(aMaxChars);
	insert(new TLabel(TRect(18, 5, 25, 6), "~L~îNGTH", aMaxChars));

	insert(new TButton(TRect(32, 3, 46, 5), "~a~CCîPT", cmOK, bfDefault));
	insert(new TButton(TRect(32, 6, 46, 8), "~C~aNCîL", cmCancel, bfNormal));

	selectNext(False);
}

void TKafEditor::setData(void *aData)
{
	char      buf[15];
	TKafData *p = (TKafData *)aData;

    aName->setData(p->name);
    aSpeed->setData(itoa(p->speed, buf, 10));
    aMaxChars->setData(itoa(p->maxChars, buf, 10));
}

void TKafEditor::getData(void *aData)
{
	TKafData *pData = (TKafData *)aData;

    pData->speed = atoi(aSpeed->data);
    pData->maxChars = atoi(aMaxChars->data);
	strcpy(pData->name, aName->data);
}

ushort TKafEditor::dataSize()
{
	return aName->dataSize() + (2 * sizeof(short));
}

void TKafEditor::handleEvent(TEvent &event)
{
	TDialog::handleEvent(event);
}
