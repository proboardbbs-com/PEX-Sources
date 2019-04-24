#define Uses_TApplication
#define Uses_TStatusLine
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TDeskTop
#define Uses_MsgBox
#define Uses_TDialog
#define Uses_TButton
#define Uses_TLabel
#define Uses_TInputLine
#include <tv.h>
#include <stdio.h>
#include "flagsel.h"

const cmShowChart = 0x5001;

class TMyApp: public TApplication
{
public:
	TMyApp();

    static TStatusLine *initStatusLine(TRect bounds);

    virtual void handleEvent(TEvent &event);
    void showChart();

    long flagsNormal;
    long flagsReverse;
};

TMyApp::TMyApp():
	TApplication(),
	TProgInit(TMyApp::initStatusLine,
			  TMyApp::initMenuBar,
			  TMyApp::initDeskTop)
{
}

TStatusLine *TMyApp::initStatusLine(TRect bounds)
{
	bounds.a.y = bounds.b.y - 1;
    return new TStatusLine( bounds,
    	*new TStatusDef(0, 0xffff) +
        *new TStatusItem("~Alt-X~ Exit", kbAltX, cmQuit) +
        *new TStatusItem("~Alt-F~ Flags", kbAltF, cmShowChart)
	);
}

void TMyApp::handleEvent(TEvent &event)
{
	TApplication::handleEvent(event);

    if( evCommand == event.what && event.message.command == cmShowChart ){
    	showChart();
        clearEvent(event);
	}
}

void TMyApp::showChart()
{
	TDialog   *dlg = new TDialog(TRect(0,0,62,14), "Sample flags");
	dlg->options |= ofCentered;
	TPoint org; org.x = 20; org.y = 2;
	TFlagView *flags = new TFlagView(org, 0, 0);

    dlg->insert(flags);
    dlg->insert(new TLabel(TRect(2,2,15,3), "Flags: ", flags));
    dlg->insert(new TButton(TRect(2,10,12,12), "OK", cmOK, bfDefault));
    dlg->insert(new TButton(TRect(20,10,30,12), "Cancel", cmCancel, bfNormal));
    dlg->selectNext(False);

    if( cmCancel != deskTop->execView(dlg) ){
    	char buf[250];
        sprintf(buf, "\003Flags: %08lx\n \n\003Rvrsd: %08lx",
			flags->flags, flags->flagsRev);
        messageBox(buf, mfOKButton);
	}
    else messageBox("\x3Selection aborted!", mfOKButton);
    destroy(dlg);
}


	int
main( void )
{
	TMyApp app;
    app.run();
	return 0;
}
