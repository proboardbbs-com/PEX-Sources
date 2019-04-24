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
#ifndef INCLUDED_TPICTURE_H
#define INCLUDED_TPICTURE_H

#define Uses_TInputLine
#define Uses_TEvent
#define Uses_MsgBox
#define Uses_TKeys
#include <tv.h>

class TPicture: public TInputLine
{
public:
	TPicture(const TRect &r, const char *pic, Boolean (*)(const char*) = 0);
	~TPicture();

	void    handleEvent(TEvent &event);
	Boolean valid(ushort command);
	void    setState(ushort aState, Boolean enable);
	void    setData(void *pData);

protected:
	enum seek_t { prev, next };

protected:
	void    SetCharData();
	void    ErrorMessage();
	Boolean ValidChar(char aChar, int nPos);
	Boolean CanEdit(int nPos);
	int     GetEditPos(int nCurPos, seek_t whereTo);
	int     GetTruePos(int nCurPos);
	void    PlaceChar(char aChar, Boolean force = False);
	void    RemoveSelection();

private:
	char              *m_Picture;
	Boolean           (*m_Validator)(const char*);
	static const char *m_Reserved;
};

#endif /* INCLUDED_TPICTURE_H */

