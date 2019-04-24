/*
 BULETMAN ProBoard PEX Version
 Copyright (C) 1995 by Branislav L. Slantchev

 This file is part of the PB-GNU BULETMAN.

 PB-GNU BULETMAN is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2.

 PB-GNU BULETMAN is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with PB-GNU BULETMAN; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "buletman.h"

// program info
const char PROG_NAME[] = "Bulletin Manager";
const char PROG_VERSION[] = "1.16";

// Error messages
const char E_NOFILE[] = "\001File not found: '\003%s\001'";
const char E_OVR[] = "\001Bulletin number exceeds max ( \003%d\001 ): '\007%s\001'";
const char E_NOBUL[] = "\n\001Bulletin \003%d\001 does not exist. \007Press [\003Enter\007] to continue...\t";
const char E_NOPROMPT[] = "Prompt not defined: '%s'";
const char STR_NOFILE[] = "NOFILE";
char _bulRoot[MAXPATH] = "BLT-";  // root name for the bulletins BLT-xxx.A*

// prompt strings array and indices (default to ASCII)
enum { PR_MENU, PR_MENUNX, PR_AUTO, PR_AUTONX };
char *_promptArray[4] = { NULL, NULL, NULL, NULL };

char *_listNumClr = NULL;            // color for numbers (white)
char *_listTxtClr = NULL;            // color for string (yellow)

// global pointer to needed prompt
char *_promptStr;
bool _newOnly = FALSE;        // show new only
bool _autoRead = TRUE;        // show without menu
bool _noXfers = FALSE;        // allow download of bulletins
bool _listNew = FALSE;        // list new bulletins before prompt
bool _exitNow = FALSE;        // exit immediately w/o copyright string
bool _noCopyr = FALSE;        // suppress closing screen copyright text
char _scrFile[MAXFILE];       // screen file to display (root name)
char _bulArray[MAXBUL];       // array with bulletin info
char _iniFileName[MAXPATH];   // alternative INI file name

int main( int argc, char *argv[] )
{
   struct find_t rec;
   char filePath[MAXPATH];
   int startChar, opt;

   // get default configuration file name
   chext( _iniFileName, *argv, ".INI" );

   // see if /FORCE parameter is specified on command line
   // and get alternative INI file name.
   opterr = 0; // suppress error messages
   while( EOF != (opt = getopt(argc,argv,"f:F:mMnN")) ){
	  switch( toupper(opt) ){
		 case 'M': _autoRead = FALSE; break;
		 case 'F': strcpy( _iniFileName, optarg ); break;
		 case 'N': _noCopyr = TRUE; break;
		 default :
			fail( "Option not recognized: '%c'\n", opt );
	  }
   }

   // get parameters from INI file
   setup( _iniFileName );

   // initialize array and prompts
   memset( _bulArray, BUL_NOENTRY, MAXBUL );

   // calculate offset of number to check
   char *p = strrchr( _bulRoot, '\\' );
   if( p ) startChar = strlen(p+1);
   else startChar = strlen( _bulRoot );

   // build new and existing bulletins list
   sprintf( filePath, "%s%s*.A*", Config->txtpath, _bulRoot );
   int done = dos_findfirst( filePath, FA_NORMAL, &rec );
   while( !done ){
	  int num = atoi( &rec.name[startChar] );
	  if( num >= MAXBUL ) fail( E_OVR, MAXBUL, rec.name );
	  _bulArray[num] = isUpdated( rec.date, rec.time );
	  done = dos_findnext( &rec );
   }

   // the INI file initialization takes case of the strings
   _promptStr = _promptArray[ PR_MENU ];

   // autoread mode
   if( TRUE == _autoRead ){
	  if( !memchr( _bulArray, BUL_UPDATED, MAXBUL ) ){
		 _exitNow = TRUE;
		 fail( NULL );
	  }
	  autoRead();
	  fail( NULL );
   }

   // menu mode, see if screen file exists
   if( !stricmp( _scrFile, STR_NOFILE ) )
      fail( E_NOFILE, _scrFile );

   if( TRUE == _noXfers ) _promptStr = _promptArray[ PR_MENUNX ];
   else _promptStr = _promptArray[ PR_MENU ];

   puts( "\f" );
   MenuFunction( MENU_SHOWANSASC, _scrFile );
   do{
      int sel = prompt();

      switch( sel ){

         case cmQuit: // aborts program and returns to BBS
         fail( NULL );

         case cmDownload:
            if( FALSE == _noXfers ) downloadBul( NULL );
         break;

         case cmNew: // all new
            puts( "\f" );
            _newOnly = TRUE;  // force for autoRead function
            autoRead();
			if( TRUE == _noXfers ) _promptStr = _promptArray[ PR_MENUNX ];
            else _promptStr = _promptArray[ PR_MENU ];
         break;

		 case cmRedraw:
		 case cmEnter:
         case 0: // just nothing! Not an error, though...
         break;

         default:
            if( (sel >= MAXBUL) || (BUL_NOENTRY == _bulArray[sel]) ){
               printf( E_NOBUL, sel );
            }
            else{
               sprintf( filePath, "%s%d", _bulRoot, sel );
               puts( "\f" );
               MenuFunction( MENU_SHOWANSASC, filePath );
               printf( "\007Press [\003Enter\007] to continue...\t" );
            }
      } // switch

      puts( "\f" );
      MenuFunction( MENU_SHOWANSASC, _scrFile );

   }while( 1 );  // do
}


void setup( char *progName )
{
   char buf[512];
   char path[MAXPATH];
   char *SecName;
   struct find_t dummy;

   // the name of the config file passed to PB SDK's GetIniVar
   // is *argv. GetIniVar strips the extension and adds .INI to
   // it. Thus, if you rename the program, the config file will
   // have to be <newname>.ini

// rename the progFile
   IniReadBool( progName, "General", "NewOnly", &_newOnly, FALSE );
   IniReadBool( progName, "General", "ListNew", &_listNew, TRUE );
   IniReadBool( progName, "General", "NoXfer", &_noXfers, FALSE );
   IniReadStr( progName, "General", "RootName", _bulRoot, "BLT-" );
   IniReadStr( progName, "General", "ShowFile", buf, "\0" );
   strcat( strcpy( path, Config->txtpath ), buf );
   strcat( path, ".A*" );

   // if not overriden by command-line parameter, check for
   // the AutoRead setting

   if( TRUE == _autoRead )
	  IniReadBool(progName, "General", "AutoRead", &_autoRead, FALSE);

   // abort program only if not AutoRead and file not found!
   if( 0 != dos_findfirst( path, 0, &dummy ) ){
	  if( FALSE == _autoRead )  fail( E_NOFILE, path );
	  else strcpy( _scrFile, STR_NOFILE );
   }
   else strcpy( _scrFile, buf );

   // load prompts, see which ones we need
   if( CurUser->uFlags & UFLAG_ANSI ){
	  SecName = "PromptColor";
	  if( -1 != IniReadStr( progName, "ListColor", "Text", buf, EOS ) )
		 _listTxtClr = strdup( buf );
	  if( -1 != IniReadStr( progName, "ListColor", "Numbers", buf, EOS ) )
		 _listNumClr = strdup( buf );
   }
   else SecName = "PromptMono";

   if( -1 == IniReadStr(progName, SecName, "Menu", buf, EOS) )
	  fail( E_NOPROMPT, "Menu" );
   _promptArray[ PR_MENU ] = strdup( buf );

   if( -1 == IniReadStr(progName, SecName, "MenuNX", buf, EOS) )
	  fail( E_NOPROMPT, "MenuNX" );
   _promptArray[ PR_MENUNX ] = strdup( buf );

   if( -1 == IniReadStr(progName, SecName, "Auto", buf, EOS) )
	  fail( E_NOPROMPT, "Auto" );
   _promptArray[ PR_AUTO ] = strdup( buf );

   if( -1 == IniReadStr(progName, SecName, "AutoNX", buf, EOS) )
	  fail( E_NOPROMPT, "AutoNX" );
   _promptArray[ PR_AUTONX ] = strdup( buf );
}

// Function checks file date and time against current user's last login
// expected format for date and time is from the find_t structures.
// Returns: BUL_OLD if bulletin exists, but is old
//          BUL_UPDATED, if bulletin is newer
int isUpdated( int loginDate, int loginTime )
{
   int year = 1980 + (loginDate >> 9);
   int month = (loginDate >> 5) & 0x000f;
   int day = loginDate & 0x001f;
   int hour = (loginTime >> 11) & 0x001f;
   int mins = (loginTime >> 5) & 0x003f;

   if( year > (CurUser->lastDate[2] + 1900) ) return BUL_UPDATED;
   else if( year < (CurUser->lastDate[2] + 1900) ) return BUL_OLD;

   if( month > CurUser->lastDate[1] ) return BUL_UPDATED;
   else if( month < CurUser->lastDate[1] ) return BUL_OLD;

   if( day > CurUser->lastDate[0] ) return BUL_UPDATED;
   else if( day < CurUser->lastDate[0] ) return BUL_OLD;

   if( hour > CurUser->lastTime[0] ) return BUL_UPDATED;
   else if( hour < CurUser->lastTime[0] ) return BUL_OLD;

   if( mins > CurUser->lastTime[1] ) return BUL_UPDATED;
   return BUL_OLD;
}


// Downloads a bulletin.
// Note: ANSI users can download .ANS bulletin only!
//       ASCII users can download .ASC bulletin only!
void downloadBul( char *rootName )
{
      char data[MAXPATH];
      char buf[MAXFILE];
       int num, asciiVersion = 0, ansiVersion = 0;

   // if no rootName, obtain bulletin number to download
   if( NULL == rootName ){
      printf( "\n\007Enter number of bulletin to download: " );
      Input( buf, 4, INPUT_DIGITS | INPUT_NOFIELD );
      num = atoi( buf );
      if( (num >= MAXBUL) || (BUL_NOENTRY == _bulArray[num]) ){
         printf( E_NOBUL, num );
         return;
      }
	  sprintf( buf, "%s%d", _bulRoot, num );
   }
   else strcpy( buf, rootName );

   // select ANS or ASC file
   sprintf( data, "%s%s.ANS", Config->txtpath, buf );
   if( !access(data, 0) ) ansiVersion = 1;
   sprintf( data, "%s%s.ASC", Config->txtpath, buf );
   if( !access(data, 0) ) asciiVersion = 1;

   // check if file exists
   if( !ansiVersion && !asciiVersion ){
	  printf( "\001Bulletin file for '\003%d\001' not found! "
              "\007Press [\003Enter\007] to continue...\t", num );
      return;
   }

   // if both exist, prompt user which one they want
   if( ansiVersion && asciiVersion ){
      printf( "\rThis bulletin is available in \003ANSI\007 and \003ASCII\007"
	  ". Download which one? (\003A\007nsi/a\003S\007cii) " );
      int key = WaitKeys( "AS" );
	  if( 'A' == key ) asciiVersion = 0;
      else ansiVersion = 0;
   }

   if( asciiVersion ) sprintf( data, "/F=%s%s.ASC", Config->txtpath, buf );
   else sprintf( data, "/F=%s%s.ANS", Config->txtpath, buf );

   MenuFunction( MENU_DOWNLOAD, data );
}


// Prompts for user input. Returns command or bulletin number.
// Does not check for invalid input.
int prompt( void )
{
   char buf[5];
   int  anyUpdated = 0;


   putchar( '\n' );

   if( TRUE == _listNew ){
	  printf( "%sBulletins updated: ", _listTxtClr ? _listTxtClr : "" );
	  for( int i = 1; i < MAXBUL; ++i ){
		 if( BUL_UPDATED == _bulArray[i] ){
			if( !anyUpdated ){       // first bulletin updated found
			   printf( "%s%d", _listNumClr ? _listNumClr : "", i );
			   anyUpdated = 1;
			}
			else printf( "%s, %s%d", _listTxtClr ? _listTxtClr : "",
				_listNumClr ? _listNumClr : "", i );
		 }
	  }
	  if( !anyUpdated ) printf( "%sNone.\n", _listNumClr ? _listNumClr : "");
	  else putchar( '\n' );
   }

   printf( _promptStr );
   Input( buf, 4, INPUT_ALL | INPUT_NOFIELD );
   switch( toupper( *buf ) ){
	  case 'Q':
	  return cmQuit;

	  case 'R':
	  return cmRedraw;

	  case 'D':
	  return cmDownload;

	  case 'N':
	  return cmNew;

	  // [Enter] redraws
	  case '\0':
		 return cmEnter;

	  default:
		 return atoi( buf );
   }
}


// Aborts program with optional error message.
void fail( const char *fmt, ... )
{
   va_list aPtr;

   // see which prompt strings we need to free ( from strdup() )
   // it is permissible to pass NULL pointers to free()
   for( int i = 0; i < 4; ++i ) free( _promptArray[i] );
   if( _listTxtClr ) free( _listTxtClr );
   if( _listNumClr ) free( _listNumClr );

   // see if we need to print an error message
   if( NULL != fmt ){
      va_start( aPtr, fmt );
      vprintf( fmt, aPtr );
      va_end( aPtr );
      printf( "\n%cPress [%cEnter%c] to continue...\t",
               WHITE, YELLOW, WHITE );
   }

   if( TRUE == _exitNow ) exit();

   // Copyright string here
   if( (FALSE == _noCopyr) &&
	   (CurUser->uFlags & UFLAG_ANSI) &&
	   (BaudRate == 0 || BaudRate >= 9600) )
   {
	  char buf[100];

	  sprintf( buf, "%s v.%s Copyright (C) 1995 Silicon Creations",
		 PROG_NAME, PROG_VERSION );
	  puts( "\f" );
	  _delayFade = 145;
	  fade( buf, (80 - strlen(buf)) / 2, 10 );

	  sprintf( buf, "Registered to %s", Config->bbsname );
	  _delayFlash = 5;
	  flash( buf, (80 - strlen(buf)) / 2, 23 );
   }
   exit();
}


// Displays bulletins in AutoMode.
void autoRead( void )
{
   char filePath[MAXPATH];

   if( TRUE == _noXfers ) _promptStr = _promptArray[ PR_AUTONX ];
   else _promptStr = _promptArray[ PR_AUTO ];

   for( int i = 1; i < MAXBUL; ++i ){

	  // no bulletin, skip
	  if( BUL_NOENTRY == _bulArray[i] ) continue;

	  // if NewOnly and bulletin is old, skip
	  if( (TRUE == _newOnly) && (BUL_OLD == _bulArray[i]) )
		 continue;

	  // display bulletin
	  sprintf( filePath, "%s%d", _bulRoot, i );
	  MenuFunction( MENU_SHOWANSASC, filePath );

	  switch( prompt() ){
		 case cmRedraw:
			--i;  // decrement counter for next pass, same bulletin
		 break;

		 case cmQuit:
		 return;

		 case cmDownload:
			if( FALSE == _noXfers ) downloadBul( filePath );
		 break;

		 default: break;
	  } // switch
   } // for
}
