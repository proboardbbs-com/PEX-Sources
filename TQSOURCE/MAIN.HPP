
/* ------------------------------------------------------------
 * Filename ............... Main.Hpp
 *
 * General Purpose ........ Main include file
 * ------------------------------------------------------------
 * First date ............. 12-28-94 05:55pm
 *
 * First in version ....... 2.00
 *
 * Written by ............. Alain Schellinck
 *
 * Copyright .............. Nephilim software development
 * ------------------------------------------------------------
 *
 *       See the GNU.DOC file before using this source!
 *
 * ------------------------------------------------------------
 * Revisions:
 * ----------
 *
 *   Date   | By |                  Purpose                   |
 * ---------+----+--------------------------------------------+
 *          |    |                                            |
 *          |    |                                            |
 *          |    |                                            |
 */

/*-------------------------------------------------------------------------*/

#ifndef __MAIN_HPP
#  define __MAIN_HPP

/*-------------------------------------------------------------------------*/

#  ifdef PEX
#     include <pb_sdk.h>
#  else
#     include <stdarg.h>
#     include <stdio.h>
#     include <stdlib.h>
#     include <string.h>
#     include <dos.h>
#     include <dir.h>
#  endif

/*-------------------------------------------------------------------------*/

#  include "typedefs.hpp"
#  include "version.hpp"

/*-------------------------------------------------------------------------*/

#  ifdef PEX
#     define TRAP   printf("\7%s(%u) ", __FILE__, __LINE__)
#  else
#     define TRAP   printf("%s(%u) ", __FILE__, __LINE__)
#  endif

/*-------------------------------------------------------------------------*/

#endif

/*---------------------------] END OF THE CODE [---------------------------*/
