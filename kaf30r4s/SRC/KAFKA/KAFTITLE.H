/*
 * This file is part of the KaFKa Kompiler
 * Copyright (C) 1995 by Branislav L. Slantchev
 *
 * Distributed under the GNU General Public License
 * See the file 'copying' for more information about your license.
 *
*/
#ifndef __TTLINE_H
#define __TTLINE_H

#define Uses_TView
#include <tv.h>

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

#endif
