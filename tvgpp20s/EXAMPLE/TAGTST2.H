#ifndef __TAGDLG_H
#define __TAGDLG_H

#define Uses_TDialog
#define Uses_TEvent
#define Uses_TButton
#define Uses_TCollection
#define Uses_MsgBox
#define Uses_TScrollBar
#include <tv.h>

#include "taglist.h"

class TTagRecord: public TTagEntry
{
public:
	TTagRecord(const char *aName, short aNum);
	virtual char *getText();

	char  name[80];
    short num;
};

class TTagCollection: public TCollection
{
public:
	TTagCollection(short aLimit, short aDelta);
    TTagRecord *at(short item){ return (TTagRecord *)TCollection::at(item); }

protected:
	void  writeItem(void *item, opstream &os){ return; }
    void *readItem(ipstream &is){ return 0; }
};

class TTagDialog: public TDialog
{
public:
	TTagDialog();

    virtual void handleEvent(TEvent &event);

protected:
	TTagCollection *list;
    TTagListBox    *listBox;
};

#endif