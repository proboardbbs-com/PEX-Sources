/*
 * This file is part of Listerz v2.0 (VE)
 *
 * Copyright (C) 1995-1997 by Branislav L. Slantchev
 * A Product of Silicon Creations, Inc.
 *
 * This file is distributed under the terms and conditions of the GNU
 * General Public License. For more information, refer to the file
 * Copying.Doc which is included in the archive.
*/
#include "lsz_appl.h"
#include "file.h"
#include "pblsdk.h"
#include "proboard.h"
#include "pbnames.h"
#include "stdmac.h"
#include "lsz_area.h"
#include "kbcodes.h"

static enum { TAG_SELECT, TAG_DESELECT, TAG_TOGGLE } g_TagState;

static void
ChangeTag(void *pItem, void*)
{
	Area *item = (Area *)pItem;

	switch( g_TagState )
	{
		case TAG_SELECT  : item->m_tagged = True;  break;
		case TAG_DESELECT: item->m_tagged = False; break;
		case TAG_TOGGLE  : item->toggleTag();      break;
	}
}

const char * Application::s_AppName = "Listerz";
const char * Application::s_DefaultText = "Building lists in Listerz";
const char * Application::s_Version = "v2.0";

Application::Application(const char *argv0)
	:m_AreaList(0)
	,m_GroupList(0)
	,m_Bucket(0)
	,m_adjusted(False)
{
	char  fileName[80];
	FILE *fp;

	// load the configuration from binary .CNF file
	file_chext(fileName, argv0, ".cnf");
	if( 0 == (fp = fopen(fileName, "rb")) )
	{
		Log(LOG_FRIEND, "%s: cannot find config '%s'", s_AppName, fileName);
		exit(EXIT_FAILURE);
	}
	fread(&m_Config, sizeof(m_Config), 1, fp);
	fclose(fp);

	// do some adjustments
	AdjustBounds();
	sprintf(m_MainScreen, "%s%s", m_Config.sys_Path, m_Config.sys_MainFile);
	sprintf(m_ProcScreen, "%s%s", m_Config.sys_Path, m_Config.sys_ProcFile);
	sprintf(m_MainHeader, "%s%s", m_Config.sys_Path, m_Config.text_HeaderAll);
	sprintf(m_MainFooter, "%s%s", m_Config.sys_Path, m_Config.text_FooterAll);
	sprintf(m_AreaHeader, "%s%s", m_Config.sys_Path, m_Config.text_Header);
	sprintf(m_AreaFooter, "%s%s", m_Config.sys_Path, m_Config.text_Footer);
	memset(&m_CurArea, 0x00, sizeof(m_CurArea));

	// load the group definitions (if any)
	if( 0 != (fp = pb_fopen(fnFGROUPS, "rb", PBDIR_SYS)) )
	{
		GROUP_PB group;
		short    num = 0;

		if( 0 == (m_GroupList = new zDoubleList(True)) )
		{
			Log(LOG_FRIEND, "%s: Memory allocation failure", s_AppName);
			fclose(fp);
			exit(EXIT_FAILURE);
		}

		file_read(fp, &group);
		while( !feof(fp) )
		{
			num++;
			if( pb_access(group.level, group.flags, group.flagsNot)
				&& EOS != group.name[0] )
			{
				Group *link = new Group(num, group);
				if( link ) m_GroupList->link(link);
			}
			file_read(fp, &group);
		}
		fclose(fp);

		// setup the listbox
		m_GroupBox = new zListViewer(m_Config.lbox_Bounds, m_GroupList);
		m_GroupBox->setClearColor(m_Config.lbox_Color);
	}

	// assign the bucket to the file area definitions data
	strcpy(fileName, pb_getpath(fnFILECFG, PBDIR_SYS));
	if( 0 == (m_Bucket = new zBucket<FILECFG>(fileName, 50, False)) )
	{
		if( m_GroupList ) delete m_GroupList;
		Log(LOG_FRIEND, "%s: could not load area definitions.", s_AppName);
		exit(EXIT_FAILURE);
	}

	// load the area indices for no group, current access only
	if( 0 == (m_AreaList = new zDoubleList(True)) )
	{
		if( m_GroupList ) delete m_GroupList;
		delete m_Bucket;
		exit(EXIT_FAILURE);
	}
	LoadAreaDefs(0);
	m_AreaBox = new zListViewer(m_Config.lbox_Bounds, m_AreaList);
	m_AreaBox->setClearColor(m_Config.lbox_Color);

	// now create the command bars and the listbox viewers
	zCommandItem *items =
		 new zCommandItem("e~x~it program", 'X', cmExit) +
		*new zCommandItem("select ~a~ll areas", 'A', cmSelectAll) +
		*new zCommandItem("~d~on't change group", 'D', cmIgnore);
	m_GroupBar = new zCommandBar(m_Config.cbar_Bounds, items);

	items = new zCommandItem("e~x~it", 'X', cmExit) +
		*new zCommandItem("~g~roups", 'G', cmGroups) +
		*new zCommandItem("~m~ake list", 'M', cmMakeList) +
		*new zCommandItem("~s~elect", 'S', cmSelectAll) +
		*new zCommandItem("~d~eselect", 'D', cmUntagAll) +
		*new zCommandItem("~t~oggle", 'T', cmToggleAll);
	m_AreaBar = new zCommandBar(m_Config.cbar_Bounds, items);

	items = new zCommandItem("~z~ip", 'Z', cmZip) +
		*new zCommandItem("~r~ar", 'R', cmRar) +
		*new zCommandItem("~a~rj", 'A', cmArj) +
		*new zCommandItem("~l~ha", 'L', cmLha) +
		*new zCommandItem("z~o~o", 'O', cmZoo) +
		*new zCommandItem("do ~n~ot compress", 'N', cmNoCompression) +
		*new zCommandItem("a~b~ort", 'B', cmExit);
	m_ArchiveBar = new zCommandBar(m_Config.cbar_Bounds, items);

	// disable relevant commands
	if( !m_GroupList || !m_GroupList->size() )
		m_AreaBar->enableCommand(cmGroups, False);
	if( !m_Config.zip.enabled || !file_exist(m_Config.zip.path) )
		m_ArchiveBar->enableCommand(cmZip, False);
	if( !m_Config.rar.enabled || !file_exist(m_Config.rar.path) )
		m_ArchiveBar->enableCommand(cmRar, False);
	if( !m_Config.arj.enabled || !file_exist(m_Config.arj.path) )
		m_ArchiveBar->enableCommand(cmArj, False);
	if( !m_Config.lha.enabled || !file_exist(m_Config.lha.path) )
		m_ArchiveBar->enableCommand(cmLha, False);
	if( !m_Config.zoo.enabled || !file_exist(m_Config.zoo.path) )
		m_ArchiveBar->enableCommand(cmZoo, False);

	// set up the palettes and various options
	char Palette[15];
	memset(Palette, 0x00, sizeof(Palette));
	Palette[0]  = m_Config.cbar_Color;
	Palette[1]  = m_Config.cbar_HotkeyColor;
	Palette[2]  = (m_Config.cbar_Color & 0xF0) | 0x08;
	Palette[3]  = m_Config.cbar_HiliteColor;
	Palette[4]  = (m_Config.cbar_HiliteColor & 0xF0) | 0x07;
	Palette[5]  = m_Config.cbar_Background;
	Palette[6]  = Palette[11] = ' ';
	Palette[7]  = m_Config.cbar_OffColor;
	Palette[8]  = Palette[7];
	Palette[9]  = (m_Config.cbar_OffColor & 0xF0) | 0x08;
	Palette[10] = m_Config.cbar_OffBackground;

	m_GroupBar->setPalette(Palette);
    m_GroupBar->options &= ~zCommandBar::delim;
	m_GroupBar->options &= ~zCommandBar::focus;
	m_AreaBar->setPalette(Palette);
	m_AreaBar->options &= ~zCommandBar::delim;
	m_AreaBar->options &= ~zCommandBar::focus;
	m_ArchiveBar->setPalette(Palette);
	m_ArchiveBar->options &= ~zCommandBar::delim;
	m_ArchiveBar->options &= ~zCommandBar::focus;

	// are we going to have online user information
	m_Useron = new zUserOnline(m_Config.drop_UserDoes, m_Config.drop_Doing);
	if( m_Useron )
	{
		if( EOS == m_Config.drop_Text[0] )
		{
			strcpy(m_Config.drop_Text, s_DefaultText);
		}
		m_Useron->Create(NodeNumber, m_Config.drop_Text);
	}

	// setup the terminal emulators
	m_Terminal.RegisterHandler(&m_avt);
	m_Terminal.RegisterHandler(&m_pcb);
	m_Terminal.RegisterHandler(&m_ans);

	// install the keyboard handler
	m_Keyboard = new zKeyboard;

	// setup the listerz file name
	sprintf(fileName, "LISTERZ.%d", NodeNumber);
	strcpy(m_fileName, pb_getpath(fileName, PBDIR_SYS));
}

// free up any memory we allocated and remove useron info
Application::~Application()
{
	if( m_Bucket     ) delete m_Bucket;
	if( m_AreaList   ) delete m_AreaList;
	if( m_GroupList  ) delete m_GroupList;
	if( m_GroupBar   ) delete m_GroupBar;
	if( m_AreaBar    ) delete m_AreaBar;
	if( m_ArchiveBar ) delete m_ArchiveBar;
	if( m_GroupBox   ) delete m_GroupBox;
	if( m_AreaBox    ) delete m_AreaBox;
	if( m_Keyboard   ) delete m_Keyboard;

	if( m_Useron )
	{
		m_Useron->Remove(NodeNumber);
        delete m_Useron;
	}
}

Boolean
Application::BelongsToGroup(short nGroup, const FILECFG &area)
{
	Boolean retval = True;

	if( 0 != nGroup && !area.allGroups )
	{
		retval = False;
		for( int i = 0; i < 4; ++i )
		{
			if( nGroup == area.groups[i] )
			{
				retval = True;
				break;
			}
		}
	}

	return retval;
}

// load area definitions for 'nGroup'. if nGroup == 0, all areas loaded
// the return value signifies whether we have any entries in the new list
Boolean
Application::LoadAreaDefs(short nGroup)
{
	FILECFG  area;

	// get rid of the old list
	while( m_AreaList->size() ) m_AreaList->unlink();
	// build the new list
	for( int i = 0; i < m_Bucket->nelems(); ++i )
	{
		area = (*m_Bucket)[i];
		if( pb_access(area.level, area.flags, area.flagsNot)
			&& EOS != area.name[0]
			&& BelongsToGroup(nGroup, area) )
		{
			Area *area = new Area(i);
			if( area ) m_AreaList->link(area);
		}
	}

	return Boolean( 0 != m_AreaList->size() );
}

// adjust the rectangles to our own system: the grid we are using is
// not the cell pattern, but rather, it is the grid between the cells
void
Application::AdjustBounds()
{
	if( !m_adjusted )
	{
		m_Config.lbox_Bounds.b.x += 1;
		m_Config.lbox_Bounds.b.y += 1;
		m_Config.cbar_Bounds.b.x += 1;
		m_Config.cbar_Bounds.b.y += 1;
		m_Config.proc_Bounds.b.x += 1;
		m_Config.proc_Bounds.b.y += 1;
		m_Config.area_Bounds.b.x += 1;
		m_Config.area_Bounds.b.y += 1;
		m_Config.area_Percent.b.x += 1;
		m_Config.area_Percent.b.y += 1;
		m_Config.total_Progbar.bounds.b.x += 1;
		m_Config.total_Progbar.bounds.b.y += 1;
		m_Config.area_Progbar.bounds.b.x += 1;
		m_Config.area_Progbar.bounds.b.y += 1;
		m_Config.total_Percent.b.x += 1;
		m_Config.total_Percent.b.y += 1;
		m_Config.total_Areas.b.x += 1;
		m_Config.total_Areas.b.y += 1;
		m_Config.total_Files.b.x += 1;
		m_Config.total_Files.b.y += 1;
		m_Config.total_Size.b.x += 1;
		m_Config.total_Size.b.y += 1;
		m_Config.total_ListSize.b.x += 1;
		m_Config.total_ListSize.b.y += 1;

		m_adjusted = True;
	}
}

ushort
Application::Process()
{
	ushort       retval = cmCancel;
	keyDownEvent event;

	m_AreaBar->setState(True);

	for( ;; )
	{
		m_Keyboard->getEvent(event);
		retval = m_AreaBar->handle(event.keyCode);

		switch( retval )
		{
			case cmSelectAll:
				g_TagState = TAG_SELECT;
				m_AreaList->forEach(ChangeTag, 0);
				m_AreaBox->draw();
				break;

			case cmToggleAll:
				g_TagState = TAG_TOGGLE;
				m_AreaList->forEach(ChangeTag, 0);
				m_AreaBox->draw();
				break;

			case cmUntagAll:
				g_TagState = TAG_DESELECT;
				m_AreaList->forEach(ChangeTag, 0);
				m_AreaBox->draw();
				break;

			case cmMakeList:
				MakeAreaList();
				retval = cmCancel;
				goto FunctionExit;

			case cmGroups:
				retval = SetNewGroup();
				/* fall-through to default case */
			case cmCancel:
			case cmExit  :
				goto FunctionExit;
		}
	}

FunctionExit:
	m_AreaBar->setState(False);
	return retval;
}

ushort
Application::ProcessGroups()
{
	ushort       retval = cmCancel;
	keyDownEvent event;

	m_GroupBar->setState(True);

	for( ;; )
	{
		m_Keyboard->getEvent(event);
		retval = m_GroupBar->handle(event.keyCode);

		switch( retval )
		{
			case cmSelectAll:
				LoadAreaDefs(0);
				m_AreaBox->newList(m_AreaList);
				/* fall-through to the next case */
			case cmIgnore:
			case cmCancel:
			case cmExit  :
				goto FunctionExit;
		}
	}

FunctionExit:
	m_GroupBar->setState(False);
	return retval;
}

// select a new group (note that this ASSUMES that m_GroupList and
// m_GroupBox are valid). since we disable the group command in the
// other case, there should be no problems as long as cmGroup is checked
ushort
Application::SetNewGroup()
{
	keyDownEvent event;
	int          retval = 0;
	Boolean      done = False;

	// display the group listbox and command bar
	m_GroupBox->draw();
	m_GroupBar->draw();

	while( !done )
	{
		m_Keyboard->getEvent(event);

		switch( event.keyCode )
		{
			case kbEsc:
				retval = ProcessGroups();
				if( retval != cmCancel ) done = True;
				break;

			case kbEnter:
				LoadAreaDefs(((Group*)m_GroupBox->focused())->m_num);
				m_AreaBox->newList(m_AreaList);
				done = True;
				break;

			default:
				m_GroupBox->handle(event.keyCode);
		}
	}

	// display the area list box and command bar
	if( cmExit != retval )
	{
		m_AreaBox->draw();
		m_AreaBar->draw();
	}

	return retval;
}

/*
 ***************************************************************************
 * the principal application driver: runs the whole thing
 ***************************************************************************
*/
void
Application::run()
{
	keyDownEvent event;
	ushort       command;
	Boolean      done = False;

	// display the initial screen, list, and command bar
	clrscr();
	pb_showfile(m_MainScreen, NULL, &m_Terminal);
	m_AreaBox->draw();
	m_AreaBar->draw();

	for( ; !done; )
	{
		m_Keyboard->getEvent(event);

		switch( event.keyCode )
		{
			case kbEsc:
				command = Process();
				if( cmExit == command ) done = True;
				break;

			case ' '    :
			case kbEnter:
			case kbIns  :
				m_AreaBox->focused()->toggleTag();
				m_AreaBox->drawFocus();
				event.keyCode = kbDown;
				/* fall-through to default handler */
			default:
				m_AreaBox->handle(event.keyCode);
		}
	}
}
