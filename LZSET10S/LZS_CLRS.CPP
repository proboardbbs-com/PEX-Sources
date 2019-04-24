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
#include <stdio.h>
#include <opcrt.h>
#include <opwindow.h>
#include "geometry.h"

extern ColorSet         lz_Colors;
extern boolean          lz_Dirty;
extern FrameArray       lz_FrameType;
extern CommandProcessor lz_Commands;

#define ccToggleBlink   201
#define ModuleCode      40

/////////////////////////////////////////////////////////////////////////////
// color selector class
/////////////////////////////////////////////////////////////////////////////
class ColorSelector: public CommandWindow
{
	byte     m_MaxColor;
	byte     m_MaxBg;
	byte     m_Width;
	byte     m_Color;
	byte     m_BoxColor;
	byte     m_BoxMono;
	byte     m_BoxAttr;
	byte     m_index;
	boolean  m_blink;
	// THE SELECTOR VALUES
	zRect    m_rect;
	boolean  m_selInited;
	char     m_selBuf[50];

	static const char  m_BoxChars[3][3];
	static const byte  m_Height;
	static const char *m_ColorChar;
	static const char  m_BlinkChar;

public:
	boolean      Init(byte X1, byte Y1);
	boolean      InitCustom(byte X1, byte Y1, ColorSet &Colors, long Options);
	void         SetColor(byte Color);
	byte         GetColor();

private:
	virtual void UpdateContents();
	virtual void ProcessSelf();
	void         SetBoxAttr(byte color, byte mMono);
	void         ColorToPos(byte attr, byte &row, byte &col);
	byte         PosToColor(byte row, byte col);
	void         DrawSelector(byte attr, byte row, byte col);
	void         UpdateSelector(byte attr);
};

const char*  ColorSelector::m_ColorChar      = "X";
const char   ColorSelector::m_BlinkChar      = 'B';
const byte   ColorSelector::m_Height         = 10;
const char   ColorSelector::m_BoxChars[3][3] =
				{
					{'Ú', 'Ä', '¿'},
					{'³', 'X', '³'},
					{'À', 'Ä', 'Ù'}
				};

/////////////////////////////////////////////////////////////////////////////
// ColorSelector methods
/////////////////////////////////////////////////////////////////////////////
boolean
ColorSelector::Init(byte col, byte row)
{
	return ColorSelector::InitCustom(col, row, lz_Colors, DefWindowOptions);
}

boolean
ColorSelector::InitCustom(byte x1, byte y1, ColorSet &Colors, long Options)
{
	byte    x2, y2;
	boolean retval = FALSE;

	m_selInited = FALSE;
	m_blink     = FALSE;

	m_MaxColor = 0x7F;
	m_MaxBg    = 0x70;
	m_Width    = 18;

	x2 = x1 + m_Width  - 1;
	y2 = y1 + m_Height - 1;

	Options &= ~wResizeable;
	Options |= (wClear | wUserContents | wBordered);

	lz_Commands.AddCommand(ccToggleBlink, 1, 0x3000, 0);

	if( CommandWindow::InitCustom(x1, y1, x2, y2, Colors, Options,
								  lz_Commands, ModuleCode) )
	{
		wFrame.AddHeader(" 00 ", heBC);
		if( RawError() != 0 )
		{
			Done();
			InitStatus = RawError();
		}
		else
		{
			m_index = wFrame.GetLastHeaderIndex();
			SetBoxAttr(Colors.FrameColor, Colors.FrameMono);
			retval = TRUE;
		}
	}

	return retval;
}

void
ColorSelector::SetBoxAttr(byte attrColor, byte attrMono)
{
	m_BoxColor = attrColor;
	m_BoxMono  = MapMono(attrColor, attrMono);
	m_BoxAttr  = ColorMono(m_BoxColor, m_BoxMono);
}

byte
ColorSelector::PosToColor(byte row, byte col)
{
	row -= wYL;
	col -= wXL;

	if(    (row == 0)
		|| (col == 0)
		|| (row == m_Height - 1)
		|| (col == m_Width - 1) )
	{
		return m_BoxAttr;
	}
	else
	{
		return ((row - 1) << 4) + (m_BoxAttr & 0x0F);
	}
}

void
ColorSelector::ColorToPos(byte attr, byte &row, byte &col)
{
	col = (wXL + 1) + (attr & 0xF);
	row = (wYL + 1) + (attr >> 4);
}

void
ColorSelector::UpdateContents()
{
	byte row, col, attrib;

	for( attrib = 0; attrib <= m_MaxColor; attrib++ )
	{
		ColorToPos(attrib, row, col);
		FastWrite((char*)m_ColorChar, row, col, attrib);
	}

	StackWindow::UpdateContents();
}

void
ColorSelector::SetColor(byte aColor)
{
	m_Color = aColor;
	m_blink = !!(aColor & 0x80);
}

byte
ColorSelector::GetColor()
{
	return m_Color | (m_blink ? 0x80 : 0x00);
}

void
ColorSelector::DrawSelector(byte attrib, byte row, byte col)
{
	char s[2];
	byte attr;
	int  x, y, rowDelta, colDelta;

	s[1] = '\0';  // to make a string later
	for( rowDelta = -1; rowDelta < 2; rowDelta++ )
	{
		y = rowDelta + row;
		for( colDelta = -1; colDelta < 2; colDelta++ )
		{
			s[0] = m_BoxChars[rowDelta+1][colDelta+1];
			x    = colDelta + col;

			if( (rowDelta == 0) && (colDelta == 0) )
			{	// leave attrib of X alone
				attr = attrib;
				if( m_blink )
				{	// change to blink and char
					attr |= 0x80;
					s[0] = m_BlinkChar;
				}
			}
			else attr = m_BoxAttr;

			FastWrite(s, y, x, attr);
		}
	}
}

void
ColorSelector::UpdateSelector(byte attr)
{
	byte     row, col;
	boolean  redraw;
	char     s[20];
	void    *p = m_selBuf;

	if( m_selInited )
	{	// not first call, restore last area
		RestoreWindow(m_rect.a.x,m_rect.a.y,m_rect.b.x,m_rect.b.y, FALSE, p);
	}
	else
	{	// first call, mark for restore
		m_selInited = TRUE;
	}

	ColorToPos(attr, row, col);
	m_rect.a.x = col - 1;
	m_rect.a.y = row - 1;
	m_rect.b.x = col + 1;
	m_rect.b.y = row + 1;
	SaveWindow(m_rect.a.x, m_rect.a.y, m_rect.b.x, m_rect.b.y, FALSE, p);
	DrawSelector(attr, row, col);
	sprintf(s, " %02X ", attr | (m_blink ? 0x80 : 0x00));
	wFrame.ChangeHeaderString(m_index, s, redraw);
	wFrame.DrawHeader(m_index);
}

void
ColorSelector::ProcessSelf()
{
	byte     saveAttr, attr;
	boolean  done, saveBlink;
	void    *p = m_selBuf;

	ClearErrors();
	Draw();
	if( RawError() != 0 ) return;

	SetCursor(cuHidden);
	done         = FALSE;
	attr         = m_Color & m_MaxColor;
	UpdateSelector(attr);

	do
	{
		GetNextCommand();

		saveAttr  = attr;
		saveBlink = m_blink;

		switch( cwCmd )
		{
			case ccLeft:
				if( (attr & 0x0F) == 0 ) attr += 0x0F;
				else attr--;
				break;

			case ccRight:
				if( (attr & 0x0F) == 0x0F ) attr -= 0x0F;
				else attr++;
				break;

			case ccUp:
				if( attr <= 0x0F ) attr += m_MaxBg;
				else attr -= 0x10;
				break;

			case ccDown:
				if( attr >= m_MaxBg ) attr -= m_MaxBg;
				else attr += 0x10;
				break;

			case ccPageUp: attr = attr & 0x0F;            break;
			case ccPageDn: attr = (attr & 0x0F) + m_MaxBg; break;
			case ccHome  : attr = attr & m_MaxBg;          break;
			case ccEnd   : attr = (attr & m_MaxBg) + 0x0F; break;

			case ccSelect:
				m_Color = attr;
				done = TRUE;
				break;

			case ccQuit:
				attr = m_Color;
				done = TRUE;
				break;

			case ccToggleBlink:
				m_blink = !m_blink;
				break;

			default:
				if( cwCmd >= ccUser0 )
				{
					m_Color = attr;
					done = TRUE;
				}
				else cwCmd = ccNone;
		}

		if( attr != saveAttr || m_blink != saveBlink )
		{ // draw new choice
			UpdateSelector(attr);
		}
	}while( !done );

	RestoreWindow(m_rect.a.x, m_rect.a.y, m_rect.b.x, m_rect.b.y, FALSE, p);
}

/////////////////////////////////////////////////////////////////////////////
// the interface function called by the program
/////////////////////////////////////////////////////////////////////////////
void
SelectColor( byte *attrib, char *header )
{
	ColorSelector CS;
	long          options = wBordered | wClear | wUserContents;

	if( CS.InitCustom(57, 6, lz_Colors, options) )
	{
		byte newAttrib;

		CS.EnableExplosions(15);
		CS.wFrame.SetFrameType(lz_FrameType);
		CS.wFrame.AddHeader(header, heTR);
		CS.wFrame.AddShadow(shBR, shSeeThru);
		CS.SetColor(*attrib);
		SetBlink(TRUE);
		CS.Draw();
		CS.Process();
		newAttrib = CS.GetColor();
		CS.Erase();
		CS.Done();

		if( *attrib != newAttrib )
		{
			*attrib = newAttrib;
			lz_Dirty = TRUE;
		}
	}
}
