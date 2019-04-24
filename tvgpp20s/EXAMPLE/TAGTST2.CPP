#include "tagtst2.h"
#include <string.h>
#include <stdio.h>

const cmDeleteTagged = 0x3350;

char *titleList[] = {
    "fpbase",             "fpstream",           "ifpstream",
    "Int11trap",          "iopstream",          "ipstream",
    "MsgBoxText",         "ofpstream",          "opstream",
    "otstream",           "pstream",            "TApplication",
    "TBackground",        "TBufListEntry",      "TButton",
    "TChDirDialog",       "TCheckBoxes",        "TCluster",
    "TCollection",        "TColorDialog",       "TColorDisplay",
    "TColorGroup",        "TColorGroupList",    "TColorItem",
    "TColorItemList",     "TColorSelector",     "TCommandSet",
    "TCrossRef",          "TDeskInit",          "TDeskTop",
    "TDialog",            "TDirCollection",     "TDirEntry",
    "TDirListBox",        "TDisplay",           "TDrawBuffer",
    "TEditor",            "TEditWindow",        "TEventQueue",
    "TFileCollection",    "TFileDialog",        "TFileEditor",
    "TFileInfoPane",      "TFileInputLine",     "TFileList",
    "TFrame",             "TGroup",             "THelpFile",
    "THelpIndex",         "THelpTopic",         "THelpViewer",
    "THelpWindow",        "THistInit",          "THistory",
    "THistoryViewer",     "THistoryWindow",     "THWMouse",
    "TIndicator",         "TInputLine",         "TLabel",
    "TListBox",           "TListViewer",        "TMemo",
    "TMenu",              "TMenuBar",           "TMenuBox",
    "TMenuItem",          "TMenuView",          "TMonoSelector",
    "TMouse",             "TNSCollection",      "TNSSortedCollection",
    "TObject",            "TPalette",           "TParagraph",
    "TParamText",         "TPoint",             "TPReadObjects",
    "TProgInit",          "TProgram",           "TPWObj",
    "TPWrittenObjects",   "TRadioButtons",      "TRect",
    "TResourceCollection","TResourceFile",      "TScreen",
    "TScrollBar",         "TScroller",          "TSItem",
    "TSortedCollection",  "TSortedListBox",     "TStaticText",
    "TStatusDef",         "TStatusItem",        "TStatusLine",
    "TStreamable",        "TStreamableClass",   "TStreamableTypes",
    "TStrIndexRec",       "TStringCollection",  "TStringList",
    "TStrListMaker",      "TSubMenu",           "TSystemError",
    "TTerminal",          "TTextDevice",        "TView",
    "TVMemMgr",           "TWindow",            "TWindowInit"
};

TTagRecord::TTagRecord(const char *aName, short aNum)
{
	strcpy(name, aName);
	num = aNum;
}

TTagCollection::TTagCollection(short aLimit, short aDelta):
	TCollection(aLimit, aDelta)
{
}

char* TTagRecord::getText()
{
	return name;
}

TTagDialog::TTagDialog():
	TDialog(TRect(0, 0, 41, 20), "Sample Taglist"),
	TWindowInit(TTagDialog::initFrame)
{
	options |= ofCentered;

	// put some entries
	list = new TTagCollection(30, 5);
	for( short i = 0; i < 111; ++i ){
		list->insert( new TTagRecord(titleList[i], i) );
	}

	// create the list box
	TScrollBar *sb = new TScrollBar(TRect(38, 2, 39, 14));
	insert(sb);
	listBox = new TTagListBox(TRect(2, 2, 38, 14), sb, 1000);
	listBox->newList(list);
	insert(listBox);

	// put some buttons
	TButton *btn = new TButton(TRect(0, 16, 30, 18), "~D~elete tagged", cmDeleteTagged, bfNormal);
	btn->options |= ofCenterX;
	insert(btn);

	selectNext(False);
}

void TTagDialog::handleEvent(TEvent &event)
{
	Boolean dirty = False;
	char    msgText[255];

	TDialog::handleEvent(event);

	if( evCommand == event.what && cmDeleteTagged == event.message.command){
		for( short i = 0; i < list->getCount(); ++i ){
			TTagRecord *rec = list->at(i);
			if( !listBox->IsTagged(i) ) continue;
			dirty = True;
			sprintf(msgText, "\003%s\n \n\003Delete?", rec->name );
			short cmd = messageBox(msgText, mfYesNoCancel);
			if( cmCancel == cmd ) break;
			if( cmNo == cmd ) continue;
			list->remove(rec);  // can't use atRemove (indices changed)
			listBox->DeleteTag(i);  // adjust the tag fields
			listBox->setRange(list->getCount());
			--i; // restore the index number (we just removed one)
			listBox->drawView();
		}
		if( !dirty ) messageBox("\003Gotta tag something first!", mfWarning|mfOKButton);
		clearEvent(event);
	}
	else if( evBroadcast == event.what && cmTagFocused == event.message.command ){
		sprintf(msgText, "\003'%s'\n \n\003Editing...", list->at(listBox->focused)->name);
		messageBox(msgText, mfOKButton);
		clearEvent(event);
	}
}