/*
 * This file is part of the KaFKa Kompiler
 * Copyright (C) 1995 by Branislav L. Slantchev
 *
 * Distributed under the GNU General Public License
 * See the file 'copying' for more information about your license.
 *
*/
#ifndef __KAFDLG_H
#define __KAFDLG_H

#define Uses_TApplication
#define Uses_TDeskTop
#define Uses_TDialog
#define Uses_TListBox
#define Uses_TScrollBar
#define Uses_TRect
#define Uses_TLabel
#define Uses_TButton
#define Uses_MsgBox
#define Uses_TFileDialog
#define Uses_TKeys
#include <tv.h>

#include <stdio.h>
#include "kafcoll.h"
#include "kaffile.h"

// general-purpose functions
Boolean  fileExists(const char *fileName);
char    *makeFont(char *aString);

// the list box class
class TKafListBox: public TListBox
{
public:
	TKafListBox(const TRect &bounds, TScrollBar *aScrollBar);
    virtual void getText(char *aDest, short item, short maxLen);
    virtual void handleEvent(TEvent &event);
};

// The main editor dialog
class TKafDialog: public TDialog
{
public:
	TKafDialog(const char *aTitle, const char *aFileName);
    ~TKafDialog();

    virtual void handleEvent(TEvent &event);
    virtual Boolean valid(ushort aCommand);

    void    editKafData(Boolean newData);
    void    deleteKafData();
    void    compileIndex();
    void    exportKaf();
    void    viewKaf();

	Boolean addRawData();
    Boolean loadKafData();
    Boolean saveKafData();

	TKafCollection *kafCollection;
    TKafListBox    *list;

private:
	Boolean  modified;
    Boolean  indexed;
    Boolean  isValid;
	char    *fileName;
};

#endif
