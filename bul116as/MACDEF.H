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


#if !defined( __MACDEF_H )
#define __MACDEF_H

/*
 ***************************************************************************
              Macro definitions common to all PB-Lib library
 ***************************************************************************
*/

/*
 * far declarations
*/
#if defined(__COMPACT__) || defined( __LARGE__ ) || defined( __HUGE__ )
	#define __FAR   far
	#define _FDATA  1
#else
	#define __FAR
	#define _FDATA  0
#endif

/*
 * some trivial stuff, I don't think anybody would define only one
*/
#if !defined( FALSE ) && !defined( TRUE )
	#define FALSE (0)
	#define TRUE  (1)
#endif

/*
 * this will always be 1 or 0
*/
#define BOOL(x)        ( !(!(x)) )

/*
 * Swap any objects, give their type as third parameter
*/
#define SWAP(a,b,type) do{ type __tswap = a; a = b; b = __tswap; }while(0)

/*
 * Cast any object to any type
*/
#define CAST(a,type)  ( *((type __FAR *)&(a)) )

/*
 * Return min, max of two integral types: be careful with ++ and --
*/
#define MIN(a,b)   ((a)-(b) <= 0 ? (a) : (b))
#define MAX(a,b)   ((a)-(b) >= 0 ? (a) : (b))

/*
 * Extract high and low order bytes from a word
*/
#define HIBYTE(w)  ( ((w) >> 8) & 0x00ff )
#define LOBYTE(w)  ( (w) & 0x00ff        )

/*
 * Nul-terminator for ASCIIZ strings
*/
#define NUL '\0'
#if !defined( EOS )
	#define EOS NUL
#endif

/*
 * Some useful typedefs
*/
typedef unsigned char  byte;
typedef unsigned char  uchar;
typedef unsigned short word;
typedef unsigned int   uint;
typedef unsigned long  dword;
typedef unsigned long  ulong;
typedef unsigned int   size_t;

#if !defined( NULL )
	#if _FDATA
		#define NULL (0L)
	#else
		#define NULL (0)
	#endif
#endif

/*
 * Correct function declarations
*/
#if defined( __cplusplus )
	#define EXTERN        extern "C"
#else
	#define EXTERN
#endif

#endif
