/*
 * This file is part of the TurboVision Gadgets++ v2.0 Library
 *
 * Copyright (c) 1995, 1997 by Branislav L. Slantchev
 * A fine product of Silicon Creations, Inc. (gargoyle)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the License which accompanies this
 * software. This library is distributed in the hope that it will
 * be useful, but without any warranty; without even the implied
 * warranty of merchantability or fitness for a particular purpose.
 *
 * You should have received a copy of the License along with this
 * library, in the file LICENSE.DOC; if not, write to the address
 * below to receive a copy via electronic mail.
 *
 * You can reach Branislav L. Slantchev (Silicon Creations, Inc.)
 * at bslantch@cs.angelo.edu. The file SUPPORT.DOC has the current
 * telephone numbers and the postal address for contacts.
*/
#ifndef INCLUDED_TVDEFS_H
#define INCLUDED_TVDEFS_H

/*
 * t u r b o v i s i o n   c o n s t a n t s
 * 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴
 * these constants are used in the turbovision modules in the library only.
*/
const
	cmSelectColor = 0x7530,		// broadcast when a new color is selected
	cmUpdateDemo  = 0x7540,		// broadcast when color demo needs updating
	cmTagFocused  = 0x7550;		// broadcast when a tag is focused

#ifdef __DJGPP__
	typedef unsigned int TVINT;
#else
	typedef unsigned short TVINT;
#endif

#endif /* INCLUDED_TVDEFS_H */
