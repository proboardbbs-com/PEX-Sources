/*
 * This file is part of the KaFKa Kompiler
 * Copyright (C) 1995 by Branislav L. Slantchev
 *
 * Distributed under the GNU General Public License
 * See the file 'copying' for more information about your license.
 *
*/
#include "kaftitle.h"
#include <string.h>

TTitleLine::TTitleLine(const TRect &bounds, const char *aTitle):
	TView(bounds)
{
	strncpy(text, aTitle, sizeof(text));
	text[sizeof(text) - 1] = EOS;
}

void TTitleLine::newTitle(const char *aTitle){
	strncpy(text, aTitle, sizeof(text));
    text[sizeof(text) - 1] = EOS;
    drawView();
}

TPalette& TTitleLine::getPalette() const
{
  static TPalette palette( cpTitleLine, sizeof(cpTitleLine)-1 );
  return palette;
}

void TTitleLine::draw()
{
  TDrawBuffer b;
  ushort color = getColor( 0x0201 );
  b.moveChar( 0, ' ', color, size.x );

  b.moveCStr( (size.x - strlen(text))/2, text, color );
  writeLine( 0, 0, size.x, 1, b );
}

