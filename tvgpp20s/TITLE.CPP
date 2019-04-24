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
#include "title.h"

#ifndef INCLUDED_STRING_H
#define INCLUDED_STRING_H
#include <string.h>
#endif

TTitleLine::TTitleLine(const TRect &bounds, const char *aTitle):
	TView(bounds)
{
	strncpy(text, aTitle, sizeof(text));
	text[sizeof(text) - 1] = EOS;
}

void
TTitleLine::newTitle(const char *aTitle)
{
	strncpy(text, aTitle, sizeof(text));
	text[sizeof(text) - 1] = EOS;
	drawView();
}

TPalette&
TTitleLine::getPalette() const
{
	static TPalette palette( cpTitleLine, sizeof(cpTitleLine)-1 );
	return palette;
}

void
TTitleLine::draw()
{
	TDrawBuffer b;
	ushort color = getColor( 0x0201 );
	b.moveChar( 0, ' ', color, size.x );

	b.moveCStr( (size.x - strlen(text))/2, text, color );
	writeLine( 0, 0, size.x, 1, b );
}

