/*
 * This file is part of LZSETUP (Configuration program for Listerz)
 *
 * Copyright (c) 1997 Branislav L. Slantchev (gargoyle)
 * A fine product of Silicon Creations, Inc.
 *
 * This file is released under the terms and conditions of the GNU
 * General Public License Version 2. The full text of the license is
 * supplied in the Copying.Doc file included with this archive. This
 * free software comes with absolutely no warranty, as outlined in the
 * licensing text. You are not allowed to remove this copyright notice.
 *
 * Contact: Branislav L. Slantchev at 73023.262@compuserve.com
*/
#include <opcmddef.h>
#include "lzs_cmds.h"

byte far LzsKeySet[LZS_KEYSMAX + 1] =
{
  //length keys         command type      key sequence
  3,      0x00, 0x47,    ccHome,           //Home
  3,      0x00, 0x48,    ccUp,             //Up
  3,      0x00, 0x49,    ccPageUp,         //PgUp
  3,      0x00, 0x4B,    ccLeft,           //Left
  3,      0x00, 0x4D,    ccRight,          //Right
  3,      0x00, 0x4F,    ccEnd,            //End
  3,      0x00, 0x50,    ccDown,           //Down
  3,      0x00, 0x51,    ccPageDn,         //PgDn
  3,      0x11, 0x04,    ccEnd,            //Q^D
  3,      0x11, 0x13,    ccHome,           //Q^S    (40)
  2,      0x03,          ccPageDn,         //C
  2,      0x04,          ccRight,          //D
  2,      0x05,          ccUp,             //E
  2,      0x09,          ccTab,            //I, Tab
  2,      0x0D,          ccSelect,         //M, Enter
  2,      0x12,          ccPageUp,         //R
  2,      0x13,          ccLeft,           //S
  2,      0x17,          ccUp,             //W
  2,      0x18,          ccDown,           //X
  2,      0x1A,          ccDown,           //Z
  2,      0x1B,          ccQuit,           //Esc    (33)
  0,0,0,0,0,0,0,0,0,0,0
};
