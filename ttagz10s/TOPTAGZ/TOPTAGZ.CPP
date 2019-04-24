/*
 * This file is part of TopTagz
 *
 * Copyright (c) 1996 by Branislav L. Slantchev
 * A Product of Silicon Creations, Inc.
 *
 * This source is distributed under the terms and conditions of the
 * GNU General Public License. A copy of the license is included with
 * this distrbiution (see the file 'Copying.Doc').
 *
 * Contact: 73023.262@compuserve.com
*/

#include "toptagz.h"
#include "listbox.h"
#include "effects.h"
#include "gadgets.h"
#include "stdini.h"
#include "strlib.h"
#include "fsys.h"
#include "class/lang.h"

#ifndef PB_SDK
	#include <ctype.h>
	#define MenuFunction(n,s)
    #define Log(a,s)
	#define NodeNumber        1
#endif

#define __VERSION "1.0r1"

char *bragText = "coded by the eccentric (aka branislav l. slantchev) for SCi";
char *topText = "download top files";
char *botMore = "more";
char *botEnd = "end of list";

static void initApp();
static TDoubleList *loadData();
static void toggle(void *, void*);
static void drawScreen(const TListBox &);
static int  makeListFile(char *, TDoubleList *);

ttConfig   cfg;
TLanguage *lang;
short      statusPrompt = 0;

	int
main( void )
{
    // load data from the index (compiled with 'SCIUTIL TF')
    TDoubleList *list;
    TKeyboard    kbd;
    short        done = 0, more;
    char         cmd[255], path[MAXPATH];

#ifdef PB_SDK
    if( !(CurUser->uFlags & UFLAG_ANSI) ){
    	printf("\nTopTagz requires ANSI or AVATAR to run.\n\t");
        return 1;
	}
#endif

    list = loadData();
    if( !list ){	// could not find index file or out of memory
    	printf("\nTopTagz cannot find the index. Inform the Sysop.\n\t");
        Log(LOG_FRIEND, "TopTagz: cannot find TOPFILES.IDX");
        Log(LOG_FRIEND, "TopTagz: run 'SCIUTIL TF' to create");
    	return 1;
	}

    initApp();		// see if we will load something from the .INI file

    TListBox listBox(list);
    drawScreen(listBox);
    more = !listBox.lastItem();
    do{
    	TTopFile *p;
        short  mode = 0;
    	ushort key = kbd.get();

    	switch( tolower(key) ){
        	case kbdEsc: done = 1; break;

            case kbdEnter:
            	p = (TTopFile *)listBox.focused();
            	sprintf(cmd, "/F=[%d]%s /K=%c /Q",
						p->file.area, p->file.name, cfg.prot);
                ClrScr();
                MenuFunction(MENU_DOWNLOAD, cmd);
				drawScreen(listBox);
            break;

            case 'g':
            	if( makeListFile(path, list) ){
	                sprintf(cmd, "/F=@%s /K=%c /Q", path, cfg.prot);
                    ClrScr();
    	            MenuFunction(MENU_DOWNLOAD, cmd);
                    rmfile(path);
        	        drawScreen(listBox);
                }
            break;

            case 'd': drawScreen(listBox); break;
            case 's': mode = 1;	// fall-through to 'r'
            case 'r':	// mode is always = 0 when we enter the case
            	list->forEach(toggle, &mode);
                listBox.draw();
            break;

            case ' ':
            	p = (TTopFile *)listBox.focused();
                p->toggleTag();
                listBox.drawFocus();
                key = kbdDown;
            	// fall-through to handle down arrow
        	default:
            	listBox.handle(key);
		}
        if( more != !listBox.lastItem() ){
        	more = !listBox.lastItem();
            drawBotSep(more ? botMore : botEnd);
		}
	}while( !done );

    delete list;
    if( lang ) delete lang;
    GotoXY(1, 24);
    SetColor(0); ClrEol();
    fade(bragText, (80 - strlen(bragText)) / 2, 24);
	return 0;
}

TDoubleList *loadData()
{
	TDoubleList *list = new TDoubleList(True);
    if( list ){
    	TTopTag top;
        FILE *fp = pbfopen("TOPFILES.IDX", "rb");
        if( !fp ){
        	delete list;
            return 0;
		}
        fread(&top, sizeof(top), 1, fp);
        while( !feof(fp) ){
        	list->link(new TTopFile(0, top));
            fread(&top, sizeof(top), 1, fp);
		}
        fclose(fp);
	}
    return list;
}

void toggle(void *item, void *mode)
{
    ((TTopFile *)item)->tagged = Boolean(*(short *)mode);
}

#pragma warn -ncf
void drawScreen(const TListBox &listBox)
{
	SetColor(0);
	ClrScr();
    drawTitle();
    drawTopSep(topText);
    listBox.draw();
    drawBotSep(listBox.lastItem() ? botEnd : botMore);
    drawStatus();
}
#pragma warn .ncf

int makeListFile(char *path, TDoubleList *list)
{
	char filename[20], line[50];
    TListCursor lc(*list);
    FILE *fp;

    sprintf(filename, "TOPFILES.%d", NodeNumber);
	if( !(fp = pbfopen(filename, "wt")) ) return 0;
    do{
    	TTopFile *p = (TTopFile *)lc.get();
        if( p->tagged ){
        	sprintf(line, "[%d]%s\n", p->file.area, p->file.name);
            fputs(line, fp);
		}
	}while( lc++ );
    fclose(fp);
    strcpy(path, _mkSysPath(filename));
    return 1;
}

void initApp()
{
	char  path[MAXPATH], s[255];
    int   n;
    strcpy(path, _mkSysPath("SILICON.INI"));

    IniReadStr(path, "TopTagz", "TagChar", s, "");
    cfg.tagChar = s[0];
    IniReadStr(path, "TopTagz", "FillChar", s, "ú");
    cfg.fillChar = s[0];
    IniReadStr(path, "TopTagz", "Protocol", s, "Z");
    strtrm(s); cfg.prot = toupper(s[0]);
    IniReadInt(path, "TopTagz", "FillColor", &n, 2); cfg.fillColor = uchar(n);
    IniReadInt(path, "TopTagz", "TagColor", &n, 12); cfg.tagColor = uchar(n);
    IniReadInt(path, "TopTagz", "DescColor", &n, 15); cfg.descColor = uchar(n);
    IniReadInt(path, "TopTagz", "NumColor", &n, 14); cfg.numColor = uchar(n);
    IniReadInt(path, "TopTagz", "HighColor", &n, 0x1e); cfg.highColor = uchar(n);
    IniReadInt(path, "TopTagz", "NormColor", &n, 14); cfg.normColor = uchar(n);
    IniReadInt(path, "TopTagz", "DelimColor", &n, 9); cfg.delimColor = uchar(n);
    IniReadInt(path, "TopTagz", "DlsColor", &n, 5); cfg.dlsColor = uchar(n);

    // do we have to load a status line
    IniReadInt(path, "TopTagz", "StatusPrompt", &n, 0);
    if( n ){
#ifdef PB_SDK
    	lang = new TLanguage(CurUser->language);
#else
		lang = new TLanguage("ENGLISH");
#endif
        if( lang ) lang->load(n);
        if( !lang->error() ) statusPrompt = n;
	}

    // do we need to change the default protocol, this field in the
    // user record can be changed by pb v2.15+, if we have an older
    // pb, then we need to use the setting from the config file
#ifdef PB_SDK
	if( CurUser->defaultProtocol ) cfg.prot = CurUser->defaultProtocol;
#endif
}
