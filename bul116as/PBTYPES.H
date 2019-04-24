/*
 BULETMAN ProBoard PEX Version
 Copyright (C) 1995 by Branislav L. Slantchev

 This file is part of the PB-GNU Bulletin Manager.

 BULETMAN is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2.

 BULETMAN is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with BULETMAN; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*/


#if !defined( __PBTYPES_H )
#define __PBTYPES_H

/*
 ***************************************************************************
                    Data types for the PB-Lib Library
 ***************************************************************************
*/

/*
 * Some new data types for ProBoard, some of them are duplicated in
 * the MACDEF.H PB-Lib standard header, but that's no big deal.
*/
typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned int   uint;
typedef unsigned long  ulong;
typedef unsigned long  dword;
typedef byte bool;
typedef byte DateType[3];
typedef byte TimeType[3];
typedef byte TimeFrame[7][6];
typedef byte combinedboards[125];
typedef word KEY;
typedef dword accessflags;
typedef DateType Date;
typedef TimeType Time;

#endif
