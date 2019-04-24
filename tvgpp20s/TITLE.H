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
#ifndef INCLUDED_TLINE_H
#define INCLUDED_TLINE_H

#define Uses_TView
#include <tv.h>
#include "tvdefs.h"

#define cpTitleLine "\x04\x1D"    // red on light gray

class TTitleLine: public TView
{
public:
	TTitleLine(const TRect &bounds, const char *aTitle);

	void      draw();
	TPalette& getPalette() const;
	void      newTitle(const char *aTitle);

private:
	char text[80];
};

#endif /* INCLUDED_TITLE_H */
