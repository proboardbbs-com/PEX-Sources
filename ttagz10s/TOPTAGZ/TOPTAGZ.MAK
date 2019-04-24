#
# This file is part of TopTagz
#
# Copyright (c) 1996 by Branislav L. Slantchev
# A Product of Silicon Creations, Inc.
#
# This source is distributed under the terms and conditions of the
# GNU General Public License. A copy of the license is included with
# this distrbiution (see the file 'Copying.Doc').
#
# Contact: 73023.262@compuserve.com
#

.AUTODEPEND
CC = bcc +TOPTAGZ.CFG
TLINK = tlink
LIBPATH = E:\SCI\LIB
INCLUDEPATH = E:\SCI\H

.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

EXE_dependencies =  toptagz.obj listbox.obj listrec.obj \
                    effects.obj gadgets.obj

toptagz.pex: toptagz.cfg $(EXE_dependencies)
  $(TLINK) /C/n/x/P-/L$(LIBPATH) @&&|
\c\lib\pb_sdk.obj+
toptagz.obj+
listbox.obj+
listrec.obj+
effects.obj+
gadgets.obj
toptagz.pex
		# no map file
\sci\lib\pblcs_px.lib+
\sci\lib\pblib_px.lib
|

toptagz.obj: toptagz.cfg toptagz.cpp
listbox.obj: toptagz.cfg listbox.cpp
listrec.obj: toptagz.cfg listrec.cpp
effects.obj: toptagz.cfg effects.c
gadgets.obj: toptagz.cfg gadgets.cpp

toptagz.cfg: toptagz.mak
  copy &&|
-ml
-v
-r-
-vi-
-wbbf
-wpin
-wamb
-wasm
-wpro
-wcln
-wdef
-wsig
-wnod
-wstv
-wucp
-wuse
-weas
-wpre
-I$(INCLUDEPATH)
-L$(LIBPATH)
-DPB_SDK
-P
| toptagz.cfg


