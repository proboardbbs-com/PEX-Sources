/*
 * This file is part of the KaFKa Kompiler
 * Copyright (C) 1995 by Branislav L. Slantchev
 *
 * Distributed under the GNU General Public License
 * See the file 'copying' for more information about your license.
 *
*/
#ifndef __KAFCMDS_H
#define __KAFCMDS_H

const
	// UI commands
	cmAboutBox         = 2000,
    cmChDir            = 2001,
    cmDosShell         = 2002,

    cmOpenList         = 3000,
    cmSaveList         = 3001,

    // the dialog commands
    cmNew              = 4001,
    cmEdit             = 4002,
    cmDelete           = 4003,
    cmView             = 4004,
    cmExtract          = 4005,
    cmCompile          = 4006;

const
	// history list IDs
	hlChangeDir  = 1,
    hlOpenList   = 2,
    hlRawFile    = 3,
    hlSpeedList  = 4,
    hlExportList = 5;

#endif
