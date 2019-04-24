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

#include "main.h"

char iniFile[80];
char oldPassWord[16];
word curDay, curMon, curYear;

static const char *defaultMesg = "\n\n"
"\001ProBoard Security Message: \007your password expires in \x13%d\007 days!"
"\n\n";


#pragma warn -ofp
   void
main( argc, argv )
   int  argc;
   char *argv[];
{
   char  buf[80];
   word  minLevel = 32000;
   dword minFlags = 0L;
   dword minFlagsNot = 0L;
   word  maxDays;
   byte  inputFlags = INPUT_ALL | INPUT_PWD;
   int   i;

   if( --argc )
      strcpy( iniFile, argv[1] );
   else{
      char *ptr;

      ptr = strrchr( argv[0], '\\' );
      if( ptr ) *ptr = '\0';
      else argv[0] = '\0';

      strcpy( iniFile, argv[0] );
      strcat( iniFile, "\\PASSWD" );
   }

   getVar( "NoCopyInfo", buf, 2, TRUE );

   if( '0' == buf[0] ){
      printf( "\003\nPassword v1.0\007 Copyright (C) 1995 by Silicon Creations Inc." );
      getVar( "RegNum", buf, 11, TRUE );
      if( !strcmp( buf, "0000000000" ) )
         puts( " \003[\x11 UNREGISTERED \003]" );
      else
         puts( " \003[\007 Registered \003]" );

      getVar( "Delay", buf, 5, FALSE );
      delay( atoi( buf ) );
   }

   getVar( "Force", buf, 2, TRUE );

   if( '0' == buf[0] ){

      if( FALSE != getVar( "SecLevel", buf, 6, FALSE ) ){
         minLevel = atoi( buf );
      }

      if( FALSE != getVar( "SecFlags", buf, 33, FALSE ) ){
         strupr( buf );
         for( i = strlen( buf ) - 1; i >= 0; --i ){
            if( buf[i] <= '9')
               buf[i] += 43;
            minFlags |= ul_shl(1, 32 - (buf[i] - '@'));
         }
      }

      if( FALSE != getVar( "SecFlagsNot", buf, 33, FALSE ) ){
         strupr( buf );
         for( i = strlen( buf ) - 1; i >= 0; --i ){
            if( buf[i] <= '9' )
               buf[i] += 43;
            minFlagsNot |= ul_shl(1, 32 - (buf[i] - '@'));
         }
      }

#if defined DEBUG
   printf( "minFlags: %lX  minFlagsNot: %lX\n"
           "usrFlags: %lX\n\t",
            minFlags, minFlagsNot, CurUser->aFlags);
#endif

      if( TRUE == CheckAccess( minLevel, minFlags ) ){
         if( !(CurUser->aFlags & minFlagsNot ) )
            exit();
      }

      getVar( "MaxDays", buf, 4, TRUE );
      maxDays = atoi( buf );

      if( FALSE == isChangeNeeded( maxDays ) )
         return;
   }

   getVar( "NoBackground", buf, 2, TRUE );
   if( '1' == buf[0] ) inputFlags |= INPUT_NOFIELD;

   strcpy( oldPassWord, CurUser->passWord );

   do{
      char secBuf[16];

      printf( "\n\007New password:\003 " );
      Input( buf, 15, inputFlags );
      if( TRUE == isValidPwd( buf ) ){
         printf( "\n\007Retype password:\003 " );
         Input( secBuf, 15, inputFlags );
         if( !strcmp( buf, secBuf ) ){
            strcpy( CurUser->passWord, buf );
            CurUser->lastPwdChange[0] = curDay;
            CurUser->lastPwdChange[1] = curMon;
            CurUser->lastPwdChange[2] = curYear;
            WriteUser( CurUser );
            puts( "\n\n\007Password successfully changed. "
                  "Press [\003Enter\007] to continue...\t" );
            Log( LOG_FRIEND, "PASSWD: %s - password changed!", CurUser->name );
            exit();
         }
         puts( "\n\003Error: \001password mismatch! Please, try again." );
      }
   }while( 1 );
}


   bool
getVar( varName, buf, maxLen, exitOnError )
   char *varName;
   char *buf;
   int  maxLen;
   bool exitOnError;
{
   if( FALSE == GetIniVar( iniFile, varName, buf, maxLen ) ){

#if defined DEBUG
   printf( "\nFailed: %s with (%d)\t\n", varName, maxLen );
#endif

      if( TRUE == exitOnError ){
         Log( LOG_FRIEND, "PASSWD: error in INI file for entry '%s'", varName );
         exit();
      }
      else
         return FALSE;
   }

#if defined DEBUG
   printf( "\nSuccess: %s=%s (%d)\t\n", varName, buf, maxLen );
#endif

   return TRUE;
}

   bool
isChangeNeeded( maxDays )
   word maxDays;
{
   time_t      timer = time( NULL );
   struct tm   *tinfo;
   word        diff, warnDays, daysLeft;
   static char buf[80];

   tinfo = localtime( &timer );

   curDay  = tinfo->tm_mday;
   curMon  = tinfo->tm_mon + 1;
   curYear = tinfo->tm_year;

   diff = daysDiff( curDay, curMon, curYear );

   if( diff >= maxDays )
      return TRUE;

   if( FALSE == getVar( "WarnDays", buf, 4, FALSE ) )
      return FALSE;

   warnDays = atoi( buf );
   daysLeft = maxDays - diff;

#if defined DEBUG
   printf( "\nDays left: %d, diff: %d\n\t", daysLeft, diff );
#endif

   if( warnDays >= daysLeft ){
      if( FALSE == getVar( "WarnMesg", buf, 80, FALSE ) )
         strcpy( buf, defaultMesg );

      if( strstr( buf, "%d" ) )
         printf( buf, daysLeft );
      else
         printf( buf );

      printf( "\n\007Press [\003Enter\007] to continue...\t" );
      Log( LOG_FRIEND, "PASSWD: %s warned (%d days to password expiration)",
                       CurUser->name, daysLeft );
   }
   return FALSE;
}


   bool
isValidPwd( passWord )
   char *passWord;
{
   static char *ControlChars = "`~!@#$%^&*()_+|\\=-\"';:/?.,>< \t";
   static byte alphaNum = 255;
   static byte ctlSymbol, minDiff = 0;
   static byte maxRepeats = 0;
   byte        diffs, repeats, alphas, symbols;
   int         i;
   char        buf[3];

   diffs = repeats = alphas = symbols = 0;

   if( 255 == alphaNum ){
      if( TRUE == getVar( "AlphaNum", buf, 3, FALSE ) )
         alphaNum = (byte)atoi( buf );
      if( TRUE == getVar( "Symbols", buf, 3, FALSE ) )
         ctlSymbol = (byte)atoi( buf );
      if( TRUE == getVar( "MinDiff", buf, 3, FALSE ) )
         minDiff = (byte)atoi( buf );
      if( TRUE == getVar( "MaxRepeats", buf, 3, FALSE ) )
         maxRepeats = (byte)atoi( buf );
      if( alphaNum + ctlSymbol > 15 ){
         Log( LOG_FRIEND, "PASSWD: config error - password length exceeded!");
         Log( LOG_FRIEND, "PASSWD: forced values: AlphaNum = 6, Symbols = 2");
         alphaNum = 6; ctlSymbol = 2;
      }
   }

   for( i = strlen( passWord ) - 1; i >= 0; --i ){

      if( isalnum( passWord[i] ) )
         alphas++;
      else if( strchr( ControlChars, passWord[i] )
               || 0x7E < (byte)passWord[i] )
         symbols++;

      if( strchr( oldPassWord, passWord[i] ) )
         repeats++;
      else
         diffs++;
   }

   if( alphas < alphaNum ){
      printf( "\003Error: \001at least \007%d \001alphanumeric characters "
              "are required!\n", alphaNum );
      return FALSE;
   }

   if( symbols < ctlSymbol ){
      printf( "\003Error: \001at least \007%d \001special symbols "
              "are required!\n", ctlSymbol );
      return FALSE;
   }

   if( diffs < minDiff){
      printf( "\003Error: \001new password must have at least \007%d "
              "\001symbols not duplicated in old!\n", minDiff );
      return FALSE;
   }

   if( repeats > maxRepeats ){
      printf( "\003Error: \001not more than \007%d \001"
              "symbols from old password can be used!\n",
               maxRepeats );
      return FALSE;
   }

   return TRUE;
}
