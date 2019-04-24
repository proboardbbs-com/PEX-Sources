#define Uses_TApplication
#define Uses_TStatusLine
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TDeskTop
#define Uses_TEvent
#define Uses_TKeys
#define Uses_MsgBox
#define Uses_TDialog
#define Uses_TCollection
#include <tv.h>

#include <string.h>
#include "tagtst2.h"
static const ushort cmTestDriver = 0x1234;

class TTestApp: public TApplication
{
public:
	TTestApp();
    virtual void handleEvent(TEvent &event);
    static TStatusLine* initStatusLine(TRect bounds);
    virtual void testDriver();
};

TTestApp::TTestApp():
	TApplication(),
    TProgInit(TTestApp::initStatusLine,
			  TTestApp::initMenuBar,
			  TTestApp::initDeskTop)
{
}

void TTestApp::handleEvent(TEvent &event)
{
	TApplication::handleEvent(event);

    if( evCommand == event.what ){
    	switch( event.message.command ){
        	case cmTestDriver: testDriver(); break;
        	default:
            	return;
		}
        clearEvent(event);
	}
}

TStatusLine* TTestApp::initStatusLine(TRect bounds)
{
	bounds.a.y = bounds.b.y - 1;
    return new TStatusLine(bounds,
    	*new TStatusDef(0, 0xffff) +
        *new TStatusItem("~Alt-X~ Exit", kbAltX, cmQuit) +
        *new TStatusItem("~Alt-R~ Run driver", kbAltR, cmTestDriver)
	);
}

#include "taglist.h"
void TTestApp::testDriver()
{
//	messageBox("\x3 Running your test driver!", mfOKButton|mfInformation);
	TTagDialog *dlg = new TTagDialog;
    if( dlg ){
    	deskTop->execView(dlg);
	}
    destroy(dlg);
}

////////////////////////////////////////////////////////////////////////////
// the main function
////////////////////////////////////////////////////////////////////////////

	int
main( void )
{
	TTestApp app;
    app.run();
	return 0;
}

