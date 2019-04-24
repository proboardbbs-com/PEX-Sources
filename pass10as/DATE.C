/*
 PB-GNU PASSWD ProBoard PEX Version
 Copyright (C) 1995 by Branislav L. Slantchev

 This file is part of the PB-GNU PASSWD.

 PB-GNU PASSWD is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2.

 PB-GNU PASSWD is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with PB-GNU PASSWD; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*/


#include "date.h"

   bool
isLeapYear( year )
   word year;
{
   year += 1900;

   if( !(year % 4) && (year % 100 || !(year % 400)) )
      return TRUE;
   else
      return FALSE;
}

   word
daysDiff( day, mon, year )
   word day, mon, year;
{
   word daysToday, daysChange;
   word passDay, passMon, passYear;
   word daysAdjust = 0;
   int  i;

   passDay  = CurUser->lastPwdChange[0];
   passMon  = CurUser->lastPwdChange[1];
   passYear = CurUser->lastPwdChange[2];

#if defined DEBUG
 printf( "Last password change: %d/%d/%d\n", passMon, passDay, passYear );
 printf( "Current date: %d/%d/%d\n\t", mon, day, year );
#endif

   /* force password change */
   if( !passDay || !passMon || !passYear )
      return 32000;

   daysToday  = dayOfYear( day, mon, year );
   daysChange = dayOfYear( passDay, passMon, passYear );

   if( year != passYear ){
      for( i = passYear; i < year; ++i ){
         daysAdjust += ( TRUE == isLeapYear( i ) ? 366 : 365 );
      }
   }

#if defined DEBUG
printf( "Today: %d,  Since last change: %d,  Adjust %d\n\t",
         daysToday, daysChange, daysAdjust );
#endif

   return daysToday - daysChange + daysAdjust;
}

   word
dayOfYear( day, month, year )
   word day, month, year;
{
   static word daysMon[14] = {0, 0, 31, 59, 90, 120, 151, 181,
                              212, 243, 273, 304, 334, 365 };
   word        numDays;

   numDays = daysMon[month] + day;
   if( (TRUE == isLeapYear(year)) && (month > 2) )
      numDays++;
   return numDays;
}

