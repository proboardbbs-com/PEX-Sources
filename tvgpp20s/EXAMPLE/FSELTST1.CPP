#define Uses_TApplication
#define Uses_TStatusLine
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TDeskTop
#define Uses_MsgBox
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
	TFlagSelect *cht = new TFlagSelect;

    if( cmCancel != deskTop->execView(cht) ){
    	char buf[250];

        sprintf(buf, "\003Flags: %08lx\n \n\003Rvrsd: %08lx",
			cht->getFlags(), cht->getRevFlags());
        messageBox(buf, mfOKButton);
	}
    else messageBox("\x3Selection aborted!", mfOKButton);
    destroy(cht);
}


	int
main( void )
{
	TMyApp app;
    app.run();
	return 0;
}
