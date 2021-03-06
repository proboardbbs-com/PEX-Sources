/*
 * This file is part of the KaFKa Kompiler
 * Copyright (C) 1995 by Branislav L. Slantchev
 *
 * Distributed under the GNU General Public License
 * See the file 'copying' for more information about your license.
 *
*/
#define Uses_TApplication
#define Uses_TStatusLine
#define Uses_TMenuBar
#define Uses_TSubMenu
#define Uses_TMenuItem
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TChDirDialog
#define Uses_TEvent
#define Uses_TDeskTop
#define Uses_TKeys
#define Uses_MsgBox
#define Uses_TFileDialog
#define Uses_TDialog
#include <tv.h>

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "/c/h/spawno.h"
#include "fsys.h"
#include "kafdlg.h"
#include "kafcmds.h"
#include "kaffile.h"
#include "kaftitle.h"

static Boolean fileInit(const char *fileName);
extern int isansi(void);

#define __VERSION "v3.0r4"
#define ABOUT_MSG	"\x3 KaFKa " __VERSION " (KaF KoMPiL�R) \n "\
					"\n\x3 CoPyRiGHT (C) 1995 By BRaNiSLaV L. SLaNTCH�V \n "\
					"\n\x3 A PRoDuCT oF SiLiCoN CR�aTioNS, iNC.\n"

int ansiSysLoaded = 0;

// Create and initialize the files, then close them (they don't exist!)
Boolean fileInit(const char *fileName)
{
	char          buf[150];
    char          name[MAXPATH];
    FILE         *fp;
    KafDataFile   dataHeader;

    makeFont(strcpy(name, fileName));
    sprintf(buf, "\x3 '%s' \n \n\x3 Do�S NoT �XiST, CR�aT� iT? ", name);
	if( cmYes == messageBox(buf, mfConfirmation|mfYesButton|mfNoButton) ){
    	fp = fopen(chext(buf, fileName, ".KDT"), "wb");
        if( fp ){
        	fwrite(&dataHeader, sizeof(dataHeader), 1, fp);
        	fclose(fp);
            return True;
        }
        else{
        	messageBox("\x3 CouLDN'T oP�N TH� FiL�, SoRRy!", mfError|mfOKButton);
            return False;
		}
	}
    return False;
}

// Here's the application!
class TKafka: public TApplication
{
public:
	TKafka();

    void handleEvent(TEvent &event);
    static TMenuBar *initMenuBar(TRect bounds);
    static TStatusLine *initStatusLine(TRect bounds);

    void changeDir();
    void dosShell();
    void openKafDialog();
};

TKafka::TKafka():
	TApplication(),
	TProgInit(TKafka::initStatusLine,
			  TKafka::initMenuBar,
			  TKafka::initDeskTop)
{
	const char titleString[] = "KaF MaiNT�NaNC� uTiLiTy";

	TRect bounds = getExtent();
    bounds.b.y = bounds.a.y + 1;
    bounds.a.x = bounds.b.x - sizeof(titleString) - 1;
	insert(new TTitleLine(bounds, titleString));

	TEvent event;
    event.what = evCommand;
    event.message.command = cmAboutBox;
    putEvent(event);
}

TStatusLine *TKafka::initStatusLine(TRect bounds)
{
	bounds.a.y = bounds.b.y - 1;
    return new TStatusLine(bounds,
    	*new TStatusDef(0, 0xffff) +
        *new TStatusItem("~Alt-X~ By�, By�", kbAltX, cmQuit) +
        *new TStatusItem("~F3~ oP�N KaF", kbF3, cmOpenList) +
        *new TStatusItem("~F10~ M�Nu", kbF10, cmMenu)
	);
}

TMenuBar *TKafka::initMenuBar(TRect bounds)
{
	bounds.b.y = bounds.a.y + 1;
    return new TMenuBar(bounds,
        *new TSubMenu("~S~yST�M", hcNoContext) +
        *new TMenuItem("~a~BouT", cmAboutBox, kbNoKey, hcNoContext) +
             newLine() +
        *new TMenuItem("~o~P�N KaF...", cmOpenList, kbF3, hcNoContext, "F3") +
        *new TMenuItem("CHaNG� ~D~iR...", cmChDir, kbNoKey, hcNoContext) +
        *new TMenuItem("~S~H�LL To DoS", cmDosShell, kbAltJ, hcNoContext, "Alt-J")+
        	 newLine() +
        *new TMenuItem("~B~y�, By�", cmQuit, kbAltX, hcNoContext, "Alt-X")
	);
}

void TKafka::changeDir()
{
	TView *dlg = validView( new TChDirDialog(0, hlChangeDir));

    if( dlg ){
    	deskTop->execView(dlg);
        destroy(dlg);
	}
}

void TKafka::dosShell()
{
	suspend();
    system(getenv("COMSPEC"));
    resume();
    redraw();
}

void TKafka::handleEvent(TEvent &event)
{
	TApplication::handleEvent(event);
    if( evCommand == event.what ){
    	switch(event.message.command){
        	case cmAboutBox:
            	messageBoxRect(TRect(15,5,65,16), ABOUT_MSG, mfInformation|mfOKButton);
            break;

            case cmOpenList: openKafDialog(); break;

            case cmDosShell: dosShell();  break;
            case cmChDir   : changeDir(); break;

        	default:
				return; // do not clear the event
		}
        clearEvent(event);
	}
}

void TKafka::openKafDialog()
{
	char        *fileName;
    char         msg[150];
    char         drive[MAXDRIVE], dir[MAXDIR], fname[MAXFILE], ext[MAXEXT];
    TFileDialog *dlg;

	dlg = new TFileDialog("*.KDT", "oP�N a KaF", "FiL� ~N~aM�",fdOpenButton,hlOpenList);
    if( validView(dlg) ){
		ushort cmd = deskTop->execView(dlg);
        if( cmCancel != cmd && cmClose != cmd ){
        	fileName = new char [MAXPATH];
			dlg->getFileName(fileName);
            if( !fileExists(fileName) && !fileInit(fileName)){
            	sprintf(msg, "\x3 CaNNoT iNiTiaLiZ� \n \n\x3 '%s' ", makeFont(fileName));
                messageBox(msg, mfError | mfOKButton);
			}
            else{
            	fnsplit(fileName, drive, dir, fname, ext);
				deskTop->execView(validView(new TKafDialog(makeFont(fname), fileName)));
			}
            delete fileName;
		}
        destroy(dlg);
	}
}

/*                                                                        *\
////////////////////////////////////////////////////////////////////////////
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
\*                                                                        */

static void copyright()
{
    if( ansiSysLoaded ) fprintf(stdout, "\r\x1b[0m\x1b[2J");
	else clrscr();
    fprintf(stderr, "KaFKa %s - KaF KoMPiL�R\n"
			" � Compiled on %s at %s\n"
			" � Copyright (C) 1995 by Branislav L. Slantchev\n"
            " � A Product of Silicon Creations, Inc. (PB-GNU Project)\n\n",
		 __VERSION, __DATE__, __TIME__);
    exit(EXIT_SUCCESS);
}

#pragma exit copyright 31

int main()
{
	TKafka kafka;
    ansiSysLoaded = isansi();
    kafka.run();
    return 0;
}
