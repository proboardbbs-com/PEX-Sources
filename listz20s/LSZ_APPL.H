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
#ifndef INCLUDED_LSZ_APPL_H
#define INCLUDED_LSZ_APPL_H

#include "dlist.h"
#include "bucket.h"
#include "lzconfig.h"
#include "pbstruct.h"
#include "useron.h"
#include "cmdbar.h"
#include "terminal.h"
#include "listview.h"
#include "keyboard.h"
#include "febbs.h"

const ushort
	cmExit     = 0x0100, cmSelectAll = 0x0101, cmGroups        = 0x0102,
	cmMakeList = 0x0103, cmUntagAll  = 0x0104, cmToggleAll     = 0x0105,
	cmZip      = 0x0106, cmRar       = 0x0107, cmArj           = 0x0108,
	cmLha      = 0x0109, cmZoo       = 0x010A, cmNoCompression = 0x010B,
	cmIgnore   = 0x010C;

class Application
{
public:
	Application(const char *argv0);
	~Application();

	void run();

	LZCONFIG*         config() { return &m_Config; }
	zBucket<FILECFG>* bucket() { return m_Bucket; }

private:
	// DATA MEMBERS
	LZCONFIG          m_Config;         // configuration record
	zUserOnline      *m_Useron;         // useron information (if any)
	zDoubleList      *m_AreaList;       // dynamic list of areas
	zDoubleList      *m_GroupList;      // list of all file groups, if any
	zBucket<FILECFG> *m_Bucket;         // bucket object tied to FILECFG.PRO
	zCommandBar      *m_GroupBar;       // group menu command bar
	zCommandBar      *m_AreaBar;        // area menu command bar
	zCommandBar      *m_ArchiveBar;     // archive menu command bar
	zListViewer      *m_GroupBox;       // group list box
	zListViewer      *m_AreaBox;        // area list box
	zKeyboard        *m_Keyboard;       // the keyboard handler
	zTerminal         m_Terminal;       // the terminal emulator
	ansi_interp       m_ans;            // the ANSi interpreter
	avatar_interp     m_avt;            // the avatar interpreter
	pcboard_interp    m_pcb;            // the pcboard/wildcat! interpreter
	Boolean           m_adjusted;       // have we adjusted bounds already
	FILE             *m_fp;             // file that we store the lists in
	char              m_fileName[80];   // path to the listerz file
	FILECFG           m_CurArea;        // current area configuration
	short             m_CurAreaNo;      // current area number processed
	ulong             m_AreaBytes;      // total bytes for the current area
	ulong             m_AreaFdls;       // total downloads for current area
	ulong             m_AreaFiles;      // total files for current area
	ulong             m_TotalFiles;     // total files processed
	ulong             m_TotalKbytes;    // total file sizes processed
	ulong             m_TotalFdls;      // total number of downloads
	short             m_TotalAreas;     // total number of tagged areas
	int               m_DeltaBytes;     // adjustment for the KByte value
	ulong             m_ListerSize;     // size of the created list

	char              m_MainScreen[80]; // complete name for the main screen
	char              m_ProcScreen[80]; // complete name for the processing
	char              m_MainHeader[80]; // complete name for main header
	char              m_MainFooter[80]; // complete name for main footer
	char              m_AreaHeader[80]; // complete name for area header
	char              m_AreaFooter[80]; // complete name for area footer

	// PRIVATE METHODS
	Boolean BelongsToGroup(short nGroup, const FILECFG &area);
	Boolean LoadAreaDefs(short nGroup);
	void    AdjustBounds();
	ushort  Process();
	ushort  SetNewGroup();
	ushort  ProcessGroups();
	void    MakeAreaList();
	void    ProcessFileArea(short index);
	Boolean CompressFile(ushort command);
	void    FormatLine(zFbbsLine &fbbs);
	char*   ParseLineWithMacros(char *line);
	int     MakeMacro(char *dest, char *src, int maxlen, char justify);
	void    WriteHeaderOrFooter(char *fileTemplate);

public:
	// STATIC CONSTANTS
	static const char *s_AppName;     // application name
	static const char *s_DefaultText; // default useron text
	static const char *s_Version;     // application version string
};

#endif /* INCLUDED_LSZ_APPL_H */
