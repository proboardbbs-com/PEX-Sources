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
#include "tpicture.h"

#ifndef INCLUDED_STRING_H
#define INCLUDED_STRING_H
#include <string.h>
#endif

#ifndef INCLUDED_CTYPE_H
#define INCLUDED_CTYPE_H
#include <ctype.h>
#endif

#ifndef INCLUDED_DOS_H
#define INCLUDED_DOS_H
#include <dos.h>
#endif

/* this is the code for Ctrl+Y keypress (event.keyDown.charScan.charCode) */
#define CONTROL_Y 25
/* all recognized meta-characters */
const char *TPicture::m_Reserved = "#+%$\\?^!|@*";

/*
 * Constructor: make a TInputLine with 'bounds' for dimensions. Save the
 * 'aPicture' user picture internally. The length of the allowable data
 * string is set according to the picture clause (special processing to
 * allow for the escape characters). The call-back validator function
 * pointer is saved (defaults to NULL). We then clear the data string and
 * set all literals in it. If there are no meta-characters in the picture,
 * the view will disable itself (and prohibit editing). We also place the
 * cursor over the first editable character.
 *
 * Note on the call-back function pointer: this function will be called when
 * the user accepts the data and attempts to close the dialog which contains
 * the TPicture object. Your routine will received a pointer to the current
 * data string (you may not modify it). You should perform the necessary
 * checking and then return True to signal that the string is valid, or False
 * to prevent the dialog from closing. This is useful if additional
 * validation is needed beyond the picture specifications (e.g. check for
 * a valid date string). If you don't specify a call-back routine, a simple
 * picture validation is performed.
*/
TPicture::TPicture(const TRect &bounds, const char *aPicture,
				   Boolean (*fpValidator)(const char*)):
	TInputLine(bounds, cstrlen(aPicture)),
	m_Picture(newStr(aPicture)),
	m_Validator(fpValidator)
{
	SetCharData();
	if( -1 == (curPos = GetEditPos(0, next)) )
	{
		setState(sfDisabled, False);
		curPos = 0;
	}
}

/*
 * Destructor: release the memory allocated for the picture.
*/
TPicture::~TPicture()
{
	if( m_Picture ) delete[] m_Picture;
}

/*
 * Blank out the data line and put all literals from the picture in it.
*/
void
TPicture::SetCharData()
{
	int slen = strlen(m_Picture);

	for( int i = 0; i < slen; ++i )
	{
		if( '~' == m_Picture[i] )
		{
			data[i] = m_Picture[i+1];
			i++;
		}
		else if( strchr(m_Reserved, m_Picture[i]) ) data[i] = ' ';
		else data[i] = m_Picture[i];
	}
	data[slen] = EOS;
}

/*
 * Checks if the character 'ch' is valid for the position 'nPos' in the
 * data string (NOT the picture string which may be out of sync with the
 * position because of the escape characters). Note that this works fine
 * with both meta-characters and literals. Returns True if input is valid.
*/
Boolean
TPicture::ValidChar(char ch, int nPos)
{
	int     maskPos = GetTruePos(nPos);
	Boolean rval = False;

	// see if the current picture clause char is a literal
	// and then check for an exact match (should always be)
	if( maskPos > 0 && '~' == m_Picture[maskPos - 1] )
	{
		if( m_Picture[maskPos] == ch ) return True;
		else return False;
	}

	// check the type of the char specified by the picture clause
	switch( m_Picture[maskPos] )
	{
		case '#' : if( isdigit(ch) ) rval = True; break;
		case '+' : if( isdigit(ch || strchr("+- ", ch)) ) rval = True; break;
		case '%' : if( ' ' == ch || isdigit(ch) ) rval = True; break;
		case '$' : if( isalnum(ch) ) rval = True; break;
		case '\\': if( ' ' == ch || isalnum(ch) ) rval = True; break;
		case '?' : if( isalpha(ch) ) rval = True; break;
		case '^' : if( isalpha(ch) && isupper(ch) ) rval = True; break;
		case '!' : if( isalpha(ch) && islower(ch) ) rval = True; break;
		case '|' : if( ' ' == ch || isalpha(ch) ) rval = True; break;
		case '@' : if( ' ' != ch ) rval = True; break;
		case '*' : rval = True; break;
		// this is a literal, must have exact match
		default  : rval = Boolean( m_Picture[maskPos] == ch );
	}

	return rval;
}

/*
 * Finds the actual position in the picture string which corresponds to the
 * 'nPos' position in the data string. Note that these may not be the same
 * because the picture may contain escape sequences which consist of two
 * characters but which really count as one. Returns the character offset
 * into the picture that corresponds to the 'nPos' offset in the data string.
*/
int
TPicture::GetTruePos(int nPos)
{
	int nTruePos = 0;

	for( int i = 0; i < nPos; ++i, ++nTruePos )
	{
		if( '~' == m_Picture[i] ) nTruePos++;
	}
	return nTruePos;
}

/*
 * This virtual function is called internally by the framework. We use it
 * only for the cmOK command (which closes the dialog and signals that the
 * input should be accepted) to verify the contents of the data string. If
 * the user has specified a validating call-back routine, it will be used
 * to check the data string. Otherwise, a simple picture verification is
 * performed. While the latter may seem redundant, we should allow for the
 * possibility that the caller has modified that 'data' member directly.
 * Returns False (which pervents the dialog from closing) if the data is not
 * valid. In the case when a validating call-back routine is not specified,
 * an error message is also displayed.
*/
Boolean
TPicture::valid(ushort command)
{
	Boolean retval = TInputLine::valid(command);

	if( cmOK == command )
	{
		if( m_Validator )
		{
			if( !m_Validator(data) ) retval = False;
		}
		else
		{
			for( int i = 0; i < strlen(data); ++i )
			{
				if( !ValidChar(data[i], i) )
				{
					ErrorMessage();
					retval = False;
					break;
				}
			}
		}
		if( !retval ) select();
	}
	return retval;
}

/*
 * Displays a default error message if data validation fails.
*/
void
TPicture::ErrorMessage()
{
	messageBox( mfError|mfOKButton,
				"\003Data does not correspond to picture:\n\003'%s'",
				m_Picture);
}

/*
 * Finds the next/previous editable position. This function will start from
 * character offset 'nCurPos' in the current data string and attempts to
 * locate the next (whereTo = next) or previous (whereTo = prev) offset that
 * allows editing (i.e. the picture has a meta-character for it). Note that
 * the current offset is also considered in the process. If no such offset
 * is found, this function returns -1; otherwise, the offset is returned.
*/
int
TPicture::GetEditPos(int nCurPos, seek_t whereTo)
{
	int    delta = (next == whereTo) ? 1 : -1;
	size_t len   = strlen(data);

	// special adjustment for the end position
	if( nCurPos && nCurPos >= len) nCurPos = len - 1;

	for( int i = nCurPos; i >= 0 && i < len; i += delta )
	{
		if( CanEdit(i) ) return i;
	}
	return -1;
}

/*
 * Checks if the picture clause allows editing of the character at the
 * offset 'nPos' in the data string. (Simply makes sure if the picture
 * has a meta-character for this position). Returns True if editing is
 * allowed and False otherwise (i.e. there's a literal in this position).
*/
Boolean
TPicture::CanEdit(int nPos)
{
	int nPicPos = GetTruePos(nPos);

	// if the character is in the list of meta-characters and
	// it is not part of an escape sequence, editing is allowed
	if( strchr(m_Reserved, m_Picture[nPicPos]) &&
		((nPicPos == 0) || (nPicPos > 0 && '~' != m_Picture[nPicPos - 1])) )
	{
		return True;
	}
	return False;
}

/*
 * Used internally by the framework to process all events.
*/
void
TPicture::handleEvent(TEvent &event)
{
	int pos, temp;

	// we simplify the mouse handler so that it only moves the cursor
	// to the next/previous/current editable slot.
	if( event.what & evMouse )
	{
		TInputLine::handleEvent(event);
		pos = GetEditPos(curPos, next);
		// if current is not editable, try finding the next
		if( -1 == pos ) pos = GetEditPos(curPos, prev);
		// if no next editable, try finding the previous position
		if( -1 != pos )
		{
			curPos = pos;
			if( firstPos > curPos ) firstPos = curPos;
			pos = curPos - size.x + 3;
			if( firstPos < pos ) firstPos = pos;
			drawView();
		}
	}
	else if( event.what & evKeyDown )
	{
		switch( ctrlToArrow(event.keyDown.keyCode) )
		{
			case kbRight:
			case kbHome :
			R_LABEL     : /* see default case, we might end up here! */
				temp = curPos;
				TInputLine::handleEvent(event);
				if( -1 != (pos = GetEditPos(curPos, next)) ) curPos = pos;
				else curPos = temp;
				break;

			case kbLeft:
			case kbEnd :
				temp = curPos;
				TInputLine::handleEvent(event);
				if( -1 != (pos = GetEditPos(curPos, prev)) ) curPos = pos;
				else curPos = temp;
				break;

			case kbBack:
				temp = curPos;
				if( 0 < temp && -1 != (curPos = GetEditPos(temp-1, prev)) )
				{
					temp = curPos;
					PlaceChar(' ', True);
				}
				curPos = temp;
				break;

			case kbDel:
				temp = curPos;
				PlaceChar(' ', True);
				curPos = temp;
				break;

			default:
				char aChar = event.keyDown.charScan.charCode;

				if( ' ' == aChar && !ValidChar(aChar, curPos) )
				{
					event.keyDown.keyCode = kbRight;
					goto R_LABEL;
				}
				if( CONTROL_Y == aChar ) SetCharData();
				else if( ' ' <= aChar ) PlaceChar(aChar);
				else
				{
					TInputLine::handleEvent(event);
					return;
				}
		}
		// make sure we scroll into the correct position
		if( firstPos > curPos ) firstPos = curPos;
		pos = curPos - size.x + 3;
		if( firstPos < pos ) firstPos = pos;
		drawView();
		clearEvent(event);
	}
}

/*
 * Places the character 'aChar' in the data string at the current position
 * (if it is valid input). If the picture requires case conversion for this
 * offset, it is performed as well. If the character is not valid, a short
 * beep will be sounded to alert the user about that. If the 'force'
 * parameter is True (default is False), the validity of the character will
 * not be checked. Instead, it will simply be placed in the position. This
 * specific behavior is used by the Backspace and Delete key handlers to
 * put a space in a position which may not allow it. Note that you should
 * not use it to overwrite literals.
*/
void
TPicture::PlaceChar(char aChar, Boolean force)
{
	int nextPos = GetEditPos(curPos + 1, next);
	int maskPos = GetTruePos(curPos);

	// perform the appropriate case conversions
	if( maskPos && '~' != m_Picture[maskPos-1] )
	{
		switch( m_Picture[maskPos] )
		{
			case '^': aChar = toupper(aChar); break;
			case '!': aChar = tolower(aChar); break;
		}
	}

	// forced or validated input: place character in the data string
	if( force || ValidChar(aChar, curPos) )
	{
		if( selStart != selEnd ) RemoveSelection();
		data[curPos] = aChar;
		if( -1 != nextPos ) curPos = nextPos;
	}
	// invalid input - sound the buzzer
	else
	{
		sound(500);
		delay(50);
		nosound();
	}
}

/*
 * Remove the selected characters (replaces them with blanks). This will
 * not modify any literals. It will also reset the selection to 0.
*/
void
TPicture::RemoveSelection()
{
	for( int i = selStart; i < selEnd; ++i ) if( CanEdit(i) ) data[i] = ' ';
	selStart = selEnd;
}

/*
 * Used internally by the framework. We must override it because we want
 * the cursor positioned correctly over an editable entry whenever the
 * input line comes into focus. Instead of checking for the 'aState'
 * flag, we simply check the result of the TInputLine::setState() handler.
 * If the selection has changed, we need to adjust the cursor (the selection
 * changes whenever the view becomes focused and selected).
*/
void
TPicture::setState(ushort aState, Boolean enable)
{
	TInputLine::setState(aState, enable);
	if( selStart != selEnd )
	{
		int pos = GetEditPos(0, next);

		if( -1 != pos ) curPos = firstPos = pos;
		drawView();
	}
}

/*
 * Used within the framework to set the data string using 'pData'. We must
 * override it to provide picture-safe operation. This causes only valid
 * characters in 'pData' to be copied to the data string. It also does not
 * allow the data to extend beyond what the picture permits.
*/
void
TPicture::setData(void *pData)
{
	char *s = (char *)pData;
	int   maxLen = strlen(data);

	if( 0 != s )
	{
		SetCharData();
		for( int i = 0; i < maxLen && EOS != *s; ++s, ++i )
		{
			if( ValidChar(*s, i) ) data[i] = *s;
		}
	}
}

#undef CONTROL_Y
