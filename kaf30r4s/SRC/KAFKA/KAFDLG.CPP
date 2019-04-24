/*
 * This file is part of the KaFKa Kompiler
 * Copyright (C) 1995 by Branislav L. Slantchev
 *
 * Distributed under the GNU General Public License
 * See the file 'copying' for more information about your license.
 *
*/
#include "kafdlg.h"
#include "kafcmds.h"
#include "kafedit.h"
#include "fsys.h"
#include <dir.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <conio.h>

extern int ansiSysLoaded;

////////////////////////////////////////////////////////////////////////////

Boolean fileExists(const char *fileName)
{
    struct ffblk srec;
	int          ccCode;

	ccCode = findfirst(fileName, &srec, 0);
    if( !ccCode ) return True;
    else return False;
}

char *makeFont(char *aString)
{
	for( char *p = aString; EOS != *p; ++p ){
    	if( strchr("AYUIO", *p) ) *p = tolower(*p);
        else if( 'e' == *p || 'E' == *p ) *p = 'Ó';
        else *p = toupper(*p);
	}
    return aString;
}

////////////////////////////////////////////////////////////////////////////
TKafListBox::TKafListBox(const TRect &bounds, TScrollBar *aScrollBar):
	TListBox(bounds, 1, aScrollBar)
{
}

void TKafListBox::getText(char *aDest, short item, short maxLen)
{
	TKafData *pData = ((TKafCollection *)list())->at(item);
    strncpy(aDest, pData->name, maxLen);
    aDest[maxLen] = EOS;
}

void TKafListBox::handleEvent(TEvent &event)
{
	if( (event.what == evMouseDown) && (event.mouse.doubleClick) ){
		message(owner, evBroadcast, cmListItemSelected, this);
		clearEvent(event);
	}
    TListBox::handleEvent(event);
}

////////////////////////////////////////////////////////////////////////////
TKafDialog::TKafDialog(const char *aTitle, const char *aFileName) :
	TDialog(TRect(12, 1, 68, 21), aTitle),
	TWindowInit(TKafDialog::initFrame),
    modified(False), indexed(True), isValid(True)
{
	fileName = newStr(aFileName);

    if( False == loadKafData() ){
    	messageBox("\x3 THÓ FiLÓ HaZ aN iNVaLiD HÓaDÓR", mfError | mfOKButton);
        isValid = False;
        return;
	}

	options |= ofCenterX | ofCenterY;

	TScrollBar *sb = new TScrollBar(TRect(28,3,29,18));
    insert(sb);
	list = new TKafListBox(TRect(3,3,28,18), sb);
	list->options |= ofFramed;
    list->newList(kafCollection);
	insert(list);
	insert(new TLabel(TRect(3,2,16,3), "KaF ~R~ÓCoRDZ ", list));

	insert(new TButton(TRect(32,3,53,5), "~N~ÓW", cmNew, bfNormal));
	insert(new TButton(TRect(32,5,53,7), "ÓDi~T~", cmEdit, bfDefault));
	insert(new TButton(TRect(32,7,53,9), "~D~ÓLÓTÓ", cmDelete, bfNormal));
	insert(new TButton(TRect(32,9,53,11), "~V~iÓW", cmView, bfNormal));
	insert(new TButton(TRect(32,11,53,13), "Ó~X~PoRT", cmExtract, bfNormal));
    insert(new TButton(TRect(32,13,53,15), "~S~aVÓ", cmSaveList, bfNormal));
    insert(new TButton(TRect(32,15,53,17), "CoMPiLÓ ~i~NDÓX", cmCompile, bfNormal));
	insert(new TButton(TRect(32,17,53,19), "~C~LoZÓ", cmCancel, bfNormal));

	selectNext(False);
}

TKafDialog::~TKafDialog()
{
    delete fileName;
}

Boolean TKafDialog::loadKafData()
{
	char         path[MAXPATH];
    FILE        *fpData;
    KafDataFile  kafData;

    // open the data file
    chext(path, fileName, ".KDT");
	if( !(fpData = fopen(path, "rb")) ) return False;

    // read in and verify the header
    fread(&kafData, sizeof(kafData), 1, fpData);
    if( memcmp(kafData.idString, kafDataID, sizeof(kafDataID)) ){
    	fclose(fpData);
        return False;
	}

    // now, load the data headers from the data file
    kafCollection = new TKafCollection(kafData.numRecs, 10);

    for( short i = 0; i < kafData.numRecs; ++i ){
    	KafEntryData data;
        long offset = ftell(fpData);

    	fread(&data, sizeof(data), 1, fpData);
        kafCollection->insert( new TKafData(data, offset) );

        // skip to the next record
		fseek(fpData, data.dataSize, SEEK_CUR);

        if( feof(fpData) ){
        	fclose(fpData);
            kafCollection->freeAll();
            fclose(fpData);
            return False;
		}
	}

    fclose(fpData);
    modified = False;
    indexed = True;
    return True;
}

// save the KaF data and update the offsets on the fly
Boolean TKafDialog::saveKafData()
{
	KafDataFile  dataFile;
	char         dataPath[MAXPATH], newPath[MAXPATH];
	FILE        *fpData, *fpNew;

    chext(newPath, fileName, ".TMP"); // new file (renamed later)
    if( !(fpNew = fopen(newPath, "wb")) ) return False;

    chext(dataPath, fileName, ".KDT");  // the data file (read/only)
    if( !(fpData = fopen(dataPath, "rb")) ){
    	fclose(fpNew);
        unlink(newPath);
        return False;
	}

    // update and write the headers
    dataFile.numRecs = kafCollection->getCount();
    fwrite(&dataFile, sizeof(dataFile), 1, fpNew);

    // loop and save all data and indices
    for( short i = 0; i < kafCollection->getCount(); ++i ){
    	KafEntryData  locEntry;
        TKafData     *p = kafCollection->at(i);
        long          offset = ftell(fpNew);

        fseek(fpData, p->offset, SEEK_SET);             // get to the record
        fread(&locEntry, sizeof(locEntry), 1, fpData);  // read the header
        fwrite(&locEntry, sizeof(locEntry), 1, fpNew);  // write to new file
        for( short j = 0; j < locEntry.numLines; ++j ){ // save all the lines
        	char   buf[4096];
            short  lineLen;

            fread(&lineLen, sizeof(short), 1, fpData);
            if( sizeof(buf) < lineLen ){
            	fclose(fpData);
                fclose(fpNew);
                unlink(newPath);
                messageBox("\x3 FaTaL ÓRRoR! ÓXiTiNG... ", mfError|mfOKButton);
                exit(EXIT_FAILURE);
			}
            fread(buf, sizeof(char), lineLen, fpData);
            fwrite(&lineLen, sizeof(short), 1, fpNew);
            fwrite(buf, sizeof(char), lineLen, fpNew);
		}
        // update the offset in the collection
        kafCollection->at(i)->offset = offset;
	}

    fclose(fpNew);
    fclose(fpData);
    unlink(dataPath);
    rename(newPath, dataPath);

    modified = False;
    indexed = False;

    messageBox("\x3 aLL KaFS WÓRÓ SaVÓD ", mfInformation|mfOKButton);

    return True;
}

void TKafDialog::deleteKafData()
{
    if( 0 == kafCollection->getCount() ) return;

    if( cmYes == messageBox("\x3 DÓLÓTÓ THiS KaF RÓCoRD? \n", mfWarning | mfYesNoCancel) ){
    	kafCollection->atFree(list->focused);
        list->setRange(kafCollection->getCount());
        list->drawView();
        modified = True;
        indexed = False;
	}
}

Boolean TKafDialog::addRawData()
{
	char         path[MAXPATH], dataPath[MAXPATH];
    KafDataFile  dataHeader;
    KafEntryData locHeader;
    long         offset;
    char         msg[150];
    FILE        *fpRaw, *fpData;

	TFileDialog *dlg = new TFileDialog("*.*", "RaW DaTa FiLÓ", "NaMÓ", fdOpenButton, hlRawFile );

    if( !TApplication::application->validView(dlg) ) return False;
    if( cmCancel == TProgram::deskTop->execView(dlg) ){
    	destroy(dlg);
        return False;
    }
    else{
    	dlg->getFileName(path);
        destroy(dlg);
	}

	if( !fileExists(path) ){
        sprintf(msg, "\x3 '%s' \n \n\x3 DoÓZ NoT ÓXiST ", makeFont(path));
        messageBox(msg, mfError | mfOKButton);
        return False;
	}

    fpRaw = fopen(path, "rt");
    if( !fpRaw ) goto reportError;

    // write the raw information and the default header
    chext(dataPath, fileName, ".KDT");
    fpData = fopen(dataPath, "r+b");
    if( !fpData ){ fclose(fpRaw); goto reportError; }
    fread(&dataHeader, sizeof(dataHeader), 1, fpData);
    dataHeader.numRecs++;
	rewind(fpData);
    fwrite(&dataHeader, sizeof(dataHeader), 1, fpData);

    fseek(fpData, 0, SEEK_END);
    offset = ftell(fpData);
    fwrite(&locHeader, sizeof(locHeader), 1, fpData);
    for(locHeader.numLines = locHeader.dataSize = 0; ; ++locHeader.numLines){
        char  buf[4096];
        short length;

        fgets(buf, sizeof(buf), fpRaw);
        if( feof(fpRaw) ) break;
        buf[sizeof(buf) - 1] = EOS; // just in case to prevent strlen from death
        length = strlen(buf);
        if( '\n' == buf[length - 1] ) buf[length - 1] = EOS;
        else length++;

        fwrite(&length, sizeof(short), 1, fpData);
        fwrite(buf, sizeof(char), length, fpData);

        locHeader.dataSize += length + sizeof(short);
	}
    fclose(fpRaw);

    // go back and update the local header
    fseek(fpData, offset, SEEK_SET);
    locHeader.speed = 0;
	locHeader.maxChars = 0;
    strcpy(locHeader.name, "uNNaMÓD FoR ÓVÓRMoRÓ");
    fwrite(&locHeader, sizeof(locHeader), 1, fpData);
    fclose(fpData);

    // now, insert the new item in kafCollection and focus it
    kafCollection->insert( new TKafData(locHeader, offset) );
    list->setRange(kafCollection->getCount());
    list->focusItem(kafCollection->getCount() - 1);
    list->drawView();

    indexed = False;
    modified = True;
    messageBox("\x3 FiLÓ iMPoRTÓD FiNÓ! ", mfInformation|mfOKButton);
    return True;

reportError:
	messageBox("\x3 uNRÓCoVÓRaBLÓ ÓRRoR! ", mfError | mfOKButton);
    return False;
}

void TKafDialog::editKafData(Boolean newData)
{
    KafEntryData locHeader;
	TKafData     data(locHeader, 0);
    char         path[MAXPATH];

	if( newData && !addRawData() ) return;

    if( 0 == kafCollection->getCount() ){
    	messageBox("\x3 GoTTa HaVÓ STuFF To ÓDiT! ", mfOKButton);
        return;
	}

    // update the focused item's header data
    TKafEditor *dlg = new TKafEditor;

    dlg->setData(kafCollection->at(list->focused));

    if( cmCancel != TProgram::deskTop->execView(dlg) ){
		dlg->getData(&data);
	    destroy(dlg);
	}
    else{
    	destroy(dlg);
        return;
	}

    // update the local header in the data file
    chext(path, fileName, ".KDT");
    FILE *fpData = fopen(path, "r+b");
    if( !fpData ){
    	messageBox("\x3 ÓRRoR uPDaTiNG THÓ FiLÓ \n", mfError|mfOKButton);
        return;
	}

    // set the values in the local header from the data structure
    long aOffset = kafCollection->at(list->focused)->offset;

    fseek(fpData, aOffset, SEEK_SET);
    fread(&locHeader, sizeof(locHeader), 1, fpData);
    fseek(fpData, aOffset, SEEK_SET);
    strcpy(locHeader.name, data.name);
    locHeader.speed = data.speed;
    locHeader.maxChars = data.maxChars;
    fwrite(&locHeader, sizeof(locHeader), 1, fpData);
    fclose(fpData);

    // update the item in the collection
    delete kafCollection->at(list->focused);
    kafCollection->atPut(list->focused, new TKafData(locHeader, aOffset));

    // redraw the list, just in case the name has changed
    list->drawView();
}

void TKafDialog::compileIndex()
{
	FILE       *fp;
    KafIdxFile  idxHdr;
    char        path[MAXPATH], msg[150];

    if( !(fp = fopen(chext(path, fileName, ".KIX"), "wb")) ){
        sprintf(msg, "\x3 '%s' \n \n\x3 CouLD NoT Be CRÓaTÓD", makeFont(path));
        messageBox(msg, mfError|mfOKButton);
        return;
	}

	idxHdr.numRecs = kafCollection->getCount();
    fwrite(&idxHdr, sizeof(idxHdr), 1, fp);

    for( short i = 0; i < kafCollection->getCount(); ++i ){
    	long offset = kafCollection->at(i)->offset;
    	fwrite(&offset, sizeof(long), 1, fp);
	}

    fclose(fp);
    indexed = True;

    messageBox("\x3 iNDÓX SuCCÓSSFuLLy RÓBuiLT ", mfInformation|mfOKButton);
}

void TKafDialog::handleEvent(TEvent &event)
{
    if( (evKeyDown == event.what) && (kbEsc == event.keyDown.keyCode) ){
    	event.what = evCommand;
        event.message.command = cmClose;
	}

    TDialog::handleEvent(event);

    if( evCommand == event.what ){
    	switch(event.message.command){
        	case cmNew     : editKafData(True); break;
            case cmEdit    : editKafData(False); break;
            case cmDelete  : deleteKafData(); break;
            case cmCompile : compileIndex(); break;
            case cmSaveList: saveKafData(); break;
            case cmExtract : exportKaf(); break;
            case cmView    : viewKaf(); break;
            default:
            	return;
		}
        clearEvent(event);
	}
    else if( evKeyDown == event.what ){
    	switch(event.keyDown.keyCode){
        	case kbIns: editKafData(True); break;
            case kbDel: deleteKafData();   break;
            default:
            	return;
		}
        clearEvent(event);
	}
    else if( (evBroadcast == event.what) &&
			 (cmListItemSelected == event.message.command) )
	{
        editKafData(False);
        clearEvent(event);
	}
}

Boolean TKafDialog::valid(ushort aCommand)
{
	switch(aCommand){
    	case cmQuit  :
        case cmClose :
        case cmCancel:
        	if( modified ){
            	if( cmYes == messageBox("\x3 FiLÓ NÓÓDZ RÓPaCKiNG, Do iT?", mfConfirmation|mfYesButton|mfNoButton) )
                	saveKafData();
	            else indexed = True;  // no save, no index
			}
            if( !indexed ){
            	if( cmYes == messageBox("\x3 FiLÓ NÓÓDZ RÓiNDÓXiNG, Do iT?", mfConfirmation|mfYesButton|mfNoButton) )
                	compileIndex();
			}
        break;
	}
    return isValid ? TDialog::valid(aCommand) : False;
}

void TKafDialog::exportKaf()
{
	TFileDialog  *dlg;
	char          path[MAXPATH], dataPath[MAXPATH], msg[150], buf[4096];
    FILE         *fpRaw, *fpData;
    KafEntryData  hdr;
    TKafData     *data;

    if( 0 == kafCollection->getCount() ) return;

    sprintf(path, "DUMP%04d.ANS", list->focused + 1);
    dlg = new TFileDialog(path, "ÓXPoRT To FiLÓ", "~S~aVÓ aS", fdOKButton, hlExportList);

    if( TApplication::application->validView(dlg) ){
	    if( cmCancel == TProgram::deskTop->execView(dlg) ){
	    	destroy(dlg);
	        return;
	    }
	    else{
	    	dlg->getFileName(path);
	        destroy(dlg);
		}

        fpRaw = fopen(path, "wt");
        if( !fpRaw ){
        	sprintf(msg, "\x3 '%s'\n \nCaN'T oPÓN", makeFont(path));
        	messageBox(msg, mfError|mfOKButton);
            return;
		}

        fpData = fopen(chext(dataPath, fileName, ".KDT"), "rb");
        if( !fpData ){
        	sprintf(msg, "\x3 CouLDN'T oPÓN SouRCÓ:\n \n\x3 '%s'", dataPath);
            messageBox(msg, mfError|mfOKButton);
            fclose(fpRaw);
            unlink(path);
            return;
		}

        // look for the data and dump it to the new file
        data = kafCollection->at(list->focused);
        fseek(fpData, data->offset, SEEK_SET);
        fread(&hdr, sizeof(hdr), 1, fpData);

        if( cmYes == messageBox("\x3 SaVÓ NaMÓ, LÓNGTH & SPÓÓD To FiLÓ? ",
			mfConfirmation|mfYesButton|mfNoButton)){
	 		sprintf(buf, "%s %d %d\n", hdr.name, hdr.maxChars, hdr.speed);
	        fputs(buf, fpRaw);
        }

        for(short i = 0; i < hdr.numLines; ++i ){
        	short slen;

            fread(&slen, sizeof(short), 1, fpData);

            if( slen > sizeof(buf)){
            	messageBox("\x3 LiNÓ Too LoNG. \n \n\x3 FiLÓ oNLy PaRTiaLLy SaVÓD",
								mfWarning|mfOKButton);
                fclose(fpData);
                fclose(fpRaw);
                return;
			}

            fread(buf, sizeof(char), slen, fpData);
            fputs(buf, fpRaw); fputc('\n', fpRaw);
		}

        fclose(fpData);
        fclose(fpRaw);

        messageBox("\x3 ÓXPoRT CoMPLÓTÓD ", mfInformation|mfOKButton);
    }
}

void TKafDialog::viewKaf()
{
	FILE          *fpData;
    char           path[MAXPATH];
    short          x, y;
    TKafData      *data;
    KafEntryData   hdr;
    TNSCollection *lines;

	if( 0 == kafCollection->getCount() ) return;

    if( !ansiSysLoaded ){
    	if( cmYes != messageBox("\x3 aNSi.SyS (oR CoMPaTiBLÓ) NoT LoaDeD\n \n\x3 CoNTiNuÓ aNyWay?", mfWarning|mfYesNoCancel) )
        	return;
	}

    fpData = fopen(chext(path, fileName, ".KDT"), "rb");
    if( !fpData ){
    	messageBox("\x3 FaTaL ÓRRoR oPÓNiNG SouRCÓ", mfError|mfOKButton);
        return;
	}

    data = kafCollection->at(list->focused);
    fseek(fpData, data->offset, SEEK_SET);
    fread(&hdr, sizeof(hdr), 1, fpData);

    lines = new TNSCollection(hdr.numLines, 0);
    if( !lines ){
    	fclose(fpData);
        messageBox("\x3 NoT ÓNouGH MÓMoRy To ViÓW THaT!", mfError|mfOKButton);
        return;
	}

    for( short i = 0; i < hdr.numLines; ++i ){
    	short  slen;
        char  *str;

        fread(&slen, sizeof(short), 1, fpData);
        str = new char [slen];
        if( !str ){
        	messageBox("\x3 MÓMoRy aLLoCaTioN FaiLuRÓ!", mfError|mfOKButton);
            fclose(fpData);
            delete lines;
            return;
		}
        fread(str, sizeof(char), slen, fpData);
        lines->insert(str);
	}
    fclose(fpData);

    if( 0 == hdr.maxChars ) x = 1;
    else x = (80 - hdr.maxChars) / 2;

    if( 0 == hdr.speed) y = 1;
    else y = 12;

    TApplication::application->suspend();
    clrscr();
    cputs("HiT a KÓy To QiuT...");

    do{
	    for( i = 0; i < hdr.numLines; ++i ){

	    	gotoxy(x, y);
	        cout << (char *)(lines->at(i));

	        if( hdr.speed ) delay(hdr.speed);
	        else{
				y = (y + 1) % 25;
	            cout << endl; // force scroll if necessary
			}
	        if( kbhit() ){
				getch();
	            goto enoughAlready;
            }
		}

        if( 0 == hdr.speed ) break; // no animation, do it once
    }while(1);

enoughAlready:
	gotoxy(1, 24);
    textattr( 15 );
    clreol();
    cputs("PRÓSS a KÓy To RÓTuRN To KaFKa...");
    while( !kbhit() )
    	;
    getch();

    TApplication::application->resume();
    TApplication::application->redraw();
    delete lines;
}
