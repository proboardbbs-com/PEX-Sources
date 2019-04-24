
/* ------------------------------------------------------------
 * Filename ............... Clock.Cpp
 *
 * General Purpose ........ Clock ticks functions
 * ------------------------------------------------------------
 * First date ............. 12-28-94 06:02pm
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

#include "main.hpp"
#include "clock.hpp"

/*--] Code [---------------------------------------------------------------*/

/*
 * Routine   : clockTicks()
 * Purpose   : the current number of clockticks
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : the current number of clockticks
 *
 */

DwdT clockTicks()
{
   return *(DwdFP(0x46C));
}

/*
 * Routine   : clockDiff()
 * Purpose   : the difference in clockticks
 * ------------------------------------------------------------------------
 * Parameters: value to compare with
 * Return    : the difference
 *
 */

DwdT clockDiff(DwdT prev)
{
   return(clockTicks() - prev);
}

/*---------------------------] END OF THE CODE [---------------------------*/
