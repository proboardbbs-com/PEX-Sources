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


#if !defined( __PBINC_H )
#define __PBINC_H

/*
 ***************************************************************************
			This is the only file you need to include in PB PEX/EXEs
 ***************************************************************************
*/

/*
 * Standard macros and defines for all PB-Lib files
*/
#if !defined( __MACDEF_H )
	#include "macdef.h"
#endif

/*
 * ProBoard-specific data types.
*/
#if !defined( __PBTYPES_H )
	#include "pbtypes.h"
#endif

#if defined( PB_SDK )
	#if !defined( PBSDK_VERSION )
		#include <pb_sdk.h>
	#endif
	/*
	 * this lets us use PB's exit(void) function
	*/
	#define EXIT_FAILURE
	#define EXIT_SUCCESS
	#define EOF           -1
	/*
	 * fix stupid bug in the SDK header
	*/
	#undef  MAGENTA
	#undef  BLUE
	#define MAGENTA 0x04
	#define BLUE    0x05
	EXTERN void ResetInactivity( void );
	/*
	 * include some missing structures
	*/
	#if !defined( __PBSTRUCT_H )
		#include "pbstruct.h"
	#endif
#else
	/*
	 * this provides the off-line interface to global PB vars and defines
	*/
	#if !defined( __PBHEADER_H )
		#include "pbheader.h"
	#endif
	#define l_mul(v1,v2)   ( (long)(v1) * (v2) )
	#define l_div(v1,v2)   ( (long)(v1) / (v2) )
	#define l_mod(v1,v2)   ( (long)(v1) % (v2) )
	#define l_shl(v,s)     ( (long)(v) << (uint)(s) )
	#define l_shr(v,s)     ( (long)(v) >> (uint)(s) )
	#define ul_div(v1,v2)  ( (dword)(v1) / (v2) )
	#define ul_mod(v1,v2)  ( (dword)(v1) % (v2) )
	#define ul_shl(v,s)    ( (dword)(v) << (uint)(s) )
	#define ul_shr(v,s)    ( (dword)(v) >> (uint)(s) )
#endif

/*
 * these duplicate Borland's headers: some very useful constants
*/
#define MAXPATH      80   /* maximum length of a complete file path       */
#define MAXDRIVE      3   /* maximum drive specification, includes '\0'   */
#define MAXDIR       66   /* maximum subdirectory, includes '\0'          */
#define MAXFILE       9   /* maximum file name length, acc. for '\0'      */
#define MAXEXT        5   /* maximum extension len, acc. for '\0' and '.' */
#define BUFSIZ      512   /* generic buffer size                          */

/*
 ***************************************************************************
				 These are used by some PB-specific functions
 ***************************************************************************
*/

/*
 * Protocol flags
*/
#define PROT_BATCH       0x01    /* Batch protocol          */
#define PROT_ENABLED     0x02    /* Enabled                 */
#define PROT_BOTH        0x04    /* Full-duplex protocol    */
#define PROT_BIM         0x08    /* Bimodem-type ctl-file   */

/*
 * Message flags for MSGAREA.msgBaseType
*/
#define MSG_HUDSON   0    /* Hudson base     */
#define MSG_SQUISH   1    /* Squish base     */
#define MSG_SDM      2    /* Fido *.MSG base */
#define MSG_JAM      3    /* JAM base        */

/*
 * Masks for EVENTS.daysEnabled, usage: eventrec.daysEnabled & EVT_MONDAY
*/
#define EVT_MONDAY     0x01
#define EVT_TUESDAY    0x02
#define EVT_WEDNESDAY  0x04
#define EVT_THURSDAY   0x08
#define EVT_FRIDAY     0x10
#define EVT_SATURDAY   0x20
#define EVT_SUNDAY     0x40

/*
 * Extra flags for uFlags, see: USERSPB record
*/
#define PBUF_NOIBM         0x00000001L
#define PBUF_NOTOPS        0x00000002L
#define PBUF_AVTPLUS       0x00000004L
#define PBUF_ATTEN         0x00000008L
#define PBUF_NORIP         0x00000010L
#define PBUF_MULTILOGIN    0x00000020L
#define PBUF_FREECHAT      0x00000040L
#define PBUF_LOCALONLY     0x00000080L

/*
 * Other constants
*/
#define USERDOES_LEN    0x16   /* Length of USERDOES line */

#endif

