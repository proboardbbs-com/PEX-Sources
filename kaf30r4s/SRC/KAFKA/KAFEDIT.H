/*
 * This file is part of the KaFKa Kompiler
 * Copyright (C) 1995 by Branislav L. Slantchev
 *
 * Distributed under the GNU General Public License
 * See the file 'copying' for more information about your license.
 *
*/
#ifndef __KAFEDIT_H
#define __KAFEDIT_H

#define Uses_TDialog
#define Uses_TInputLine
#define Uses_TLabel
#define Uses_THistory
#define Uses_TButton
#define Uses_TEvent
#include <tv.h>

class TIntLine: public TInputLine
{
public:
	TIntLine(const TRect &bounds, int aMaxLen, char low = '0', char high = '9');
    virtual void handleEvent(TEvent &event);

private:
	char lowChar, highChar;
};

class TKafEditor: public TDialog
{
public:
	TKafEditor();

    virtual void   handleEvent(TEvent &event);
    virtual void   setData(void *aData);
    virtual void   getData(void *aData);
    virtual ushort dataSize();

private:
	TInputLine *aName;
    TIntLine   *aSpeed;
    TIntLine   *aMaxChars;
};

#endif
