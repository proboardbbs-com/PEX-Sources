/*
 GRAFFITI ProBoard PEX and Standalone EXE Versions
 Copyright (C) 1995 by Branislav L. Slantchev

 This file is part of the GRAFFITI.

 GRAFFITI is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2.

 GRAFFITI is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with GRAFFITI; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "graffiti.h"

#define VERSION "1.01"

#if !defined( PB_SDK )
	const char __PROMPT__[] =
	"[A] Add   [D] Delete   [R] Redraw   [Q/Enter] Quit :";
	const char ALIAS_PMT[] = "Signature ([Enter] for none): ";
	const char LINE_PMT[] = "Line: ";
	const char TEMPL_PMT[] = "Select a color template: ";
#else
	const char __PROMPT__[] =
	" \007[\003A\007] \004Add your line   \007[\003D\007] \004Delete a line   "
	"\007[\003R\007] \004Redraw screen   \007[\003Q\007/\003Enter\007] \004Quit";
	const char ALIAS_PMT[] = "\007Signature ([\003Enter\007] for none): ";
	const char LINE_PMT[] = "\007Line: ";
	const char TEMPL_PMT[] = "\007Select a color template: ";
#endif

const char ERR_WRITE[] = "File IO error writing record '%d'";
const char ERR_CREAT[] = "Could not create the grafitti wall file";
const char ERR_LOCAT[] = "Could not locate data file '%s'";
const char ERR_READ[]  = "File IO error reading record '%d'";

static char wallPath[MAXPATH];
static char colorPath[MAXPATH];

static COLORDEF colors[MAXCOLORDEF];
static int totalColors = 0;

/* arg1 is the level required for deleting, defaults to 32,000 */

#pragma argsused
void main( int argc, char *argv[] )
{
	FILE *fp, *fc;
	int  numLines, num;
	LINE wall[MAXLINES];
	unsigned int sysopLevel = 32000U;
	char buf[MAXTEXT];
#if defined( PB_SDK )
	int noLog = 0, noUserDoes = 0;
#endif

	/* only in ANSI mode */
#if defined( PB_SDK )
	if( !CurUser->uFlags & UFLAG_ANSI ){
		printf( "Sorry, Physical Graffiti is available in ANSI only.\t" );
		exit();
	}
	CurUser->uFlags |= UFLAG_CLEAR;
#endif

	/* get sysop level if specified on command line */
	if( --argc ) sysopLevel = (unsigned)atoi( *(argv+1) );

#if defined( PB_SDK )
	/* check for the /NU and /NL */
	while( --argc ){
		switch( toupper(argv[argc + 1][2]) ){
			case 'U': noUserDoes = 1; break;
			case 'L': noLog = 1; break;
		}
	}
#endif

	/* figure out the data files path */
	/* the SDK has a bug: argv[0] is expanded to pathname/no extension! */
#if !defined( PB_SDK )
	strcpy( strrchr( strcpy(wallPath, *argv), '.' ), ".DAT" );
	strcpy( strrchr( strcpy(colorPath, *argv), '.' ), ".DEF" );
#else
	strcpy( wallPath, *argv );
	strcat( wallPath, ".DAT" );
	strcpy( colorPath, *argv );
	strcat( colorPath, ".DEF" );
#endif

	if( !(fp = fopen( wallPath, "a+b" )) ){
		cprintf( ERR_LOCAT, wallPath );
		exit( EXIT_FAILURE );
	}

	if( !(fc = fopen( colorPath, "rb" ) ) ){
		cprintf( ERR_LOCAT, colorPath );
		exit( EXIT_FAILURE );
	}

	/* load color definitions, up to MAXLINES */
	for( totalColors = 0; ; totalColors++){
		int stat;

		if( totalColors == MAXCOLORDEF ) break;
		stat = fread( &colors[totalColors], sizeof(COLORDEF), 1, fc );
		if( feof(fc) ) break;
		if( 1 != stat ){
			cprintf( ERR_READ, totalColors );
			fclose( fc );
			exit( EXIT_FAILURE );
		}
	}
	fclose( fc );

	for( numLines = 0; numLines < MAXLINES; ++numLines ){
		int stat;

		stat = fread( &wall[numLines], sizeof(LINE), 1, fp );

		if( feof(fp) ){
			/* brand new file, put a default one line */
			if( !numLines ){
				strcpy( wall[0].text, DEF_TEXT );
				strcpy( wall[0].name, DEF_NAME );
				wall[0].date = time( NULL );
				fwrite( &wall[0], 1, sizeof(LINE), fp );
				numLines++;
			}
			fclose( fp );
			break;
		}

		/* well, an error */
		if( 1 != stat ){
			cprintf( ERR_READ, numLines );
			fclose( fp );
			exit( EXIT_FAILURE );
		}
	}

	/* we don't check for a file longer than MAXLINES */

	/* here we have at least 1 line, the default and numLines says # */
	/* the lines are stored in reverse order in the wall array       */
	/* chronologically, the last one is the first one to be shown    */

#if defined( PB_SDK )
	if( !noLog ) Log( LOG_FRIEND, "%s entered Physical Graffiti", CurUser->name );
	if( !noUserDoes ) userd_creat( "Physical Graffiti v1.01" );
#endif

	clrscr();
	printWall( wall, numLines );

	/* main program loop */
	for( ;; ){
		switch( WaitKeys( "ADRQ\r" ) ){

			/* adds a graffiti */
			case 'A':

				if( numLines == MAXLINES ){
					memmove( (char *)&wall[0], (char *)&wall[1],
						sizeof(LINE) * (MAXLINES - 1) );
					num = numLines - 1;
				}
				else num = numLines;

				gotoxy( 1, 24 );
				clreol();
				cprintf( LINE_PMT );
				Input( buf, MAXTEXT - 1, INPUT_ALL );
				if( '\0' == buf[0] ){
					printFooter( __PROMPT__ );
					break;
				}
				strcpy( wall[num].text, buf );
				gotoxy( 1, 24 );
				clreol();
				cprintf( ALIAS_PMT );
				Input( buf, MAXNAME, INPUT_ALL );
				if( '\0' == buf[0] )
					strcpy( wall[num].name, "Anonymous" );
				else
					strcpy( wall[num].name, buf );
				wall[num].date = time( NULL );
				if( numLines < MAXLINES ) numLines++;
				colorSelection();
				Input( buf, 3, INPUT_DIGITS );
				wall[num].color = atoi( buf ) - 1;
				if( wall[num].color < 0 || wall[num].color >= totalColors )
					wall[num].color = 0;
				writeWall( wall, numLines );
				printWall( wall, numLines );
			break;

			/* deletes a graffiti (Sysop only) */
			case 'D':
				if( sysopLevel <=
#if defined( PB_SDK )
				CurUser->level
#else
				32000U
#endif
				&& numLines
				){
					gotoxy( 1, 24 );
					clreol();
					textattr( WHITE );
					cprintf( "Which one do you want to delete: " );
					Input( buf, 3, INPUT_DIGITS );
					num = atoi(buf);
					if( num > 0 && num <= numLines ){
						/* obtain actual index for wall array */
						num = numLines - num;
						memmove( (char *)&wall[num], (char *)&wall[num+1],
							(numLines - num) * sizeof(LINE) );
						numLines--;
						printWall( wall, numLines );
						writeWall( wall, numLines );
					}
					else{
						printFooter( __PROMPT__ );
					}
				}
			break;

			/* redraws the screen */
			case 'R':
				printWall( wall, numLines );
			break;

			/* quit or simply [Enter] quits the program */
			case 'Q' :
			case '\r':
#if defined( PB_SDK )
				if( !noLog )
					Log( LOG_FRIEND, "%s left Physical Graffiti",
						CurUser->name );
				if( !noUserDoes ) userd_unlink();
#endif
			clrscr();
			_delayFlash = 5;
			_delayFade = 145;
			fade( "Thank you for using Physical Graffiti v1.01", 19, 12 );
#if defined( PB_SDK )
			if( BaudRate > 96000 )
		#endif
			flash( "Physical Graffiti is Copyright (C) 1995 by Branislav L. Slantchev", 8, 23 );
			return;
		}
	}
}


#if !defined( PB_SDK )
/*
 * Returns command from a list of hotkeys, -1 on error
*/
char getCommand( char *keylist )
{
	char *temp;

	temp = strupr( strdup(keylist) );
	if( !temp ) return -1;

	for( ;; ){
		int key = getch();
		if( strchr( temp, toupper(key) ) ){
			free( temp );
			return toupper((char)key);
		}
	}
}
#endif

/*
 * Prints the whole graffiti wall.
*/
void printWall( LINE wall[], int num )
{
	char dateBuf[21], bigBuf[100];
	 int i;

	clrscr();
	printHeader();

	if( !num ){
		gotoxy( 29, 11 );
		cprintf( "Nothing on the wall!" );
		printFooter( __PROMPT__ );
		return;
	}

	for( i = num - 1; 0 <= i; --i ){

		int ofs = STARTY + (num - i ) * 2;

		/* ok, print that entry */
		gotoxy( STARTX, ofs );
		strftime( dateBuf, 21, "at %H:%M on %m/%d/%y", localtime(&wall[i].date) );
		textattr( LIGHTRED );
		cprintf( "%2d. ", num - i );
		printColors( wall[i].text, wall[i].color );
		sprintf( bigBuf, "%56s%21s", wall[i].name, dateBuf );
		textattr( DARKGRAY );
		gotoxy( 1, ofs + 1 );
		cprintf( "%s", bigBuf );
	}

	printFooter( __PROMPT__ );
}


/*
 * Offline dummy for Input()
*/
#if !defined( PB_SDK )
#pragma argsused
void getStr( char *s, int len, int dummy )
{
	s[0] = len;
	cgets( s );
	memmove( s, &s[2], strlen(&s[2]) + 1 );
}
#endif


/*
 * Rewrites the wall file.
*/
void writeWall( LINE wall[], int numLines )
{
	FILE *fp;
	int  i;

	fp = fopen( wallPath, "wb" );
	if( !fp ){
		cprintf( ERR_CREAT );
		return;
	}

	for( i = 0; i < numLines; ++i ){
		if( 1 != fwrite( &wall[i], sizeof(LINE), 1, fp ) ){
			cprintf( ERR_WRITE, numLines );
			fclose( fp );
			return;
		}
	}

	fclose( fp );
}


/*
 * Prints program header
*/
void printHeader( void )
{
	gotoxy( 1, 1 );
#if defined( PB_SDK )
	printf( "\003%s \001v%s \007 Copyright (C) 1995 by Silicon Creations\001",
		__PROGNAME__, __PROGVER__ );
#else
	textcolor( YELLOW );
	cprintf( "%s", __PROGNAME__ );
	textcolor( LIGHTRED );
	cprintf( " v%s", __PROGVER__ );
	textcolor( WHITE );
	cprintf( "  Copyright (C) 1995 by Silicon Creations" );
	textcolor( LIGHTRED );
#endif
	gotoxy( 1, 2 );
	cprintf("컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�"
			"컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴" );
}

/*
 * Prints program footer.
*/
void printFooter( const char *text )
{
	gotoxy( 1, 23 );
#if defined( PB_SDK )
	printf( "\001컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�"
			"컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴" );
#else
	textcolor( LIGHTRED );
	cprintf( "컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�"
			"컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴" );
#endif
	gotoxy( 1, 24 );
	clreol();
	cprintf( "%s", text );
}


/*
 * Prints line as per color definition.
*/
void printColors( char *text, int clrIndex )
{
	char *p;
	int  i, inword;

	if( clrIndex > totalColors - 1 ) clrIndex = totalColors - 1;
	if( clrIndex < 0 ) clrIndex = 0;

	/* see if it's one color entry */
	if( colors[clrIndex].numColors == 1 ){
		textattr( colors[clrIndex].table[0] );
		cprintf( text );
		return;
	}

	/* ok, now the funny stuff: every character is in different     */
	/* color, if restart on words, indexing starts from 0 for every */
	/* new word or wraps around when exceeded. Spaces are not in    */
	/* colors (use color for last character)                        */
	for( p = text, i = 0; '\0' != *p; ++p ){

		if( isspace(*p) || strchr("`~!@#$%^&*()_+|\\=-[]{};':\"/?.,<>", *p) ){
			inword = 0;
			if( '\t' == *p ) cprintf( "    " );
			else putch( *p );
			continue;
		}

		/* if restart on word, reset index */
		if( !inword ){
			if( colors[clrIndex].onWords ) i = 0;
			inword = 1;
		}

		/* set color, print character and increment color index */
		textattr( colors[clrIndex].table[i] );
		putch( *p );

		/* test for normal condition, both onWords and !onWords */
		/* else we're pointing at the last color in the table   */
		if( i < colors[clrIndex].numColors - 1 ) i++;
		else if( !colors[clrIndex].onWords ) i++;

		/* see if we need a wrap-around */
		if( !colors[clrIndex].onWords && i == colors[clrIndex].numColors )
			i = 0;
	}
}


/*
 * Prints a table of available colors.
*/
void colorSelection( void )
{
	int i, sy = STARTY + 2;

	clrscr();
	printHeader();

	for( i = 0; i < totalColors; ++i ){
		gotoxy( STARTX, sy + i );
		textattr( LIGHTRED );
		cprintf( "%2d. ", i + 1 );
		printColors( "This is how the just-entered-text will look like...", i );
	}

	printFooter( TEMPL_PMT );
}
