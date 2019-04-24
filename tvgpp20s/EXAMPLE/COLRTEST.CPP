#define Uses_TApplication
#define Uses_TStatusLine
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TDeskTop
#define Uses_MsgBox
#define Uses_TKeys
#include <tv.h>
#include <stdio.h>

#include "colrsel.h"

const cmShowChart = 0x5001;

class TMyApp: public TApplication
{
public:
	TMyApp();

    static TStatusLine *initStatusLine(TRect bounds);

    virtual void handleEvent(TEvent &event);
    void showChart();

    ushort aColor;
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
        *new TStatusItem("~Alt-S~ Show Chart", kbAltS, cmShowChart)
	);
}

void TMyApp::handleEvent(TEvent &event)
{
	TApplication::handleEvent(event);

    if( evCommand == event.what && event.message.command == cmShowChart ){
    	showChart();
        clearEvent(event);
	}
    else if( evBroadcast == event.what ){
    	if( cmSelectColor == event.message.command ){
        	aColor = event.message.infoInt;
            clearEvent(event);
		}
	}
}

void TMyApp::showChart()
{
	TColorSelect *cht = new TColorSelect(0x4e, "Select");

    if( cmCancel != deskTop->execView(cht) ){
    	char buf[125];
    	ushort aColor = cht->getColorCode();
		sprintf(buf, "\x3Hex code selected: %02Xh", aColor);
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
