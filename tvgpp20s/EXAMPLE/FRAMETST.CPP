#define Uses_TApplication
#define Uses_TStatusLine
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TDeskTop
#define Uses_MsgBox
#define Uses_TKeys
#include <tv.h>
#include <stdio.h>

#include "tmframe.h"

const cmShowChart = 0x5001;

class TMyApp: public TApplication
{
public:
	TMyApp();

    static TStatusLine *initStatusLine(TRect bounds);

    virtual void handleEvent(TEvent &event);
    void showChart();
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
}

void TMyApp::showChart()
{
	TTimeFrame *dlg = new TTimeFrame("Time Frame Setup");
	TimeFrame tf;

	if( cmCancel != deskTop->execView(dlg) ){
		dlg->getData(&tf);
		FILE *fp = fopen("FRAMETST.DAT", "wt");
		if( fp ){
			fprintf(fp, "Time Frame test results:\n");
			for( int i = 0; i < 7; ++i ){
				fprintf(fp, "\nday %d: ", i);
				for( int j = 0; j < 6; ++j ){
					fprintf(fp, "%02X", tf[i][j]);
				}
			}
			fclose(fp);
			messageBox("\x3Saved settings in FRAMETST.DAT!", mfOKButton);
		}
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
