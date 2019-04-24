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


#if !defined( __INIFILE_H )
	#include "inifile.h"
#endif

#if !defined( __DIRMANIP_H )
	#include "dirmanip.h"
#endif

/***************************************************************************/
/*************   Reading and writing Windows-style INI files   *************/
/***************************************************************************/

enum { _DUPE, _NODUPE };

int _openfile( const char *name, int mode );
int _closefile( void );
int _locateSec( const char *name );
int _locateVar( const char *name );
int _isSecName( const char *s );

static const char *_inipath;
static const char *_tmppath;
static FILE *_inifile;
static FILE *_tmpfile;
static char  _varvalue[MAX_INI_LINE];
static int   _openmode;

/*
 * prepare ini file for reading, temp file for writing
*/
	int
_openfile( const char *name, int mode )
{
	FILE *fp;

	_openmode = mode;

	_inipath = name;
	fp = fopen( name, "r" );
	if( !fp && _DUPE == mode )
		fp = fopen( name, "w" );
	if( !fp ) return -1;
	else _inifile = fp;

	if( mode == _DUPE ){
		_tmppath = tmpname( basedir((char*)name) );
		fp = fopen( _tmppath, "w" );
		if( !fp ) return -1;
		else _tmpfile = fp;
	}
	return 0;
}

/*
 * flush rest of ini file to tmp, unlink ini and rename the tmp;
*/
	int
_closefile( void )
{
	char buf[MAX_INI_LINE];

	if( _NODUPE == _openmode ){
		fclose( _inifile );
		return 0;
	}

	/* write rest of the ini file if necessary */
	for( ;; ){
		if( NULL == fgets( buf, MAX_INI_LINE, _inifile ) ) break;
		fputs( buf, _tmpfile );
	}

	/* cleanup */
	fclose( _inifile );
	fclose( _tmpfile );
	if( -1 == unlink( _inipath ) ) return -1;
	if( -1 == rename( _tmppath, _inipath ) ) return -1;
	return 0;
}

/*
 * True if buffer resembles a section definition
*/
	int
_isSecName( const char *buf )
{
	const char *p = buf;

	if( '[' != *p++ ) return 0;
	if( !strrchr( p, ']' ) ) return 0;
	return 1;
}


/*
 * locates a section and copies as it goes along, copies the match too!
*/
	int
_locateSec( const char *secName )
{
	char buf[MAX_INI_LINE];
	char name[81];
	int  len;

	sprintf( name, "[%s]", secName );
	len = strlen( name );

	for( ;; ){
		if( NULL == fgets( buf, MAX_INI_LINE, _inifile ) ) break;
		if( _openmode == _DUPE ) fputs( buf, _tmpfile );
		if( !memicmp( buf, name, len ) ) return 0;
	}
	return -1;
}


/*
 * locates a variable in current section only, on match doesn't copy
 * but stores in _varvalue, next sec name is not copied
*/
	int
_locateVar( const char *varName )
{
	char buf[MAX_INI_LINE];
	char *p;
	int  len;

	len = strlen( varName );

	for( ;; ){
		long pos = ftell( _inifile );
		if( NULL == fgets(buf, MAX_INI_LINE, _inifile) ) break;
		/* end of section, push section name back on stream */
		if( _isSecName(buf) ){
			fseek( _inifile, pos, SEEK_SET );
			return -1;
		}

		/* eat whitespace at beginning of variable */
		for( p = buf; isspace(*p); ++p )
			;

		/* see if name matches and if yes, look for '=' char */
		if( !memicmp( p, varName, len ) ){
			for( p = &p[len]; isspace(*p); ++p )
				;

			/* if not '=' but at end of string, then it's a match */
			/* if not at end, then it's not a match, continue     */
			if( '=' != *p ){
				if( EOS == *p ){
					_varvalue[0] = EOS;
					return 0;
				}
			}
			/* ok, we have the '=' sign, store the value after it */
			/* eat leading and trailing whitespace                */
			else{
				for( ++p; isspace(*p); ++p )
					;
				strcpy( _varvalue, p );
				for( p = &_varvalue[strlen(_varvalue)-1]; isspace(*p); --p )
					;
				*(p+1) = EOS;
				return 0;
			}
		}
		/* no match, just dump the buffer to temp file */
		if( _openmode == _DUPE ) fputs( buf, _tmpfile );
	}

	return -1;
}


/*
 * writes/modifies a var in section (if they don't exist), they're created
*/
	int
IniWrite( const char *fname, const char *secName, const char *varName,
		  const char *value )
{
	char buf[MAX_INI_LINE];

	if( -1 == _openfile( fname, _DUPE ) ) return -1;

	if( secName && (-1 == _locateSec(secName)) ){
		sprintf( buf, "\n[%s]\n", secName );
		fputs( buf, _tmpfile );
	}

	/* if no match was found and no data, then it's a new toggle */
	if( -1 == _locateVar(varName) && varName ){
		sprintf( buf, "%s\n", varName );
		fputs( buf, _tmpfile );
	}
	/* if match is found and data, create it, otherwise delete it */
	else if( value ){
		sprintf( buf, "%s=%s\n", varName, value );
		fputs( buf, _tmpfile );
	}

	if( -1 == _closefile() ) return -1;
	return 0;
}


/*
 * Reads a string value
*/
	int
IniReadStr( const char *fname, const char *secName, const char *varName,
			char *value, const char *defaultStr )
{

	strcpy( value, defaultStr );

	if( -1 == _openfile(fname, _NODUPE ) ) return -1;

	/* if section was specified and not located, error */
	if( secName && (-1 == _locateSec( secName )) ) return -1;

	/* try to find variable name */
	if( -1 == _locateVar(varName) ) return -1;

	/* ok, value is in _varvalue, so that's it */
	strcpy( value, _varvalue );

	if( -1 == _closefile() ) return -1;
	return 0;
}


/*
 * Reads an integer value
*/
	int
IniReadInt( const char *fname, const char *secName, const char *varName,
			int *value, const int defaultInt )
{
	char buf[80]; /* nice large buffer */

	*value = defaultInt;
	if( -1 == IniReadStr( fname, secName, varName, buf, 0 ) )
		return -1;

	*value = atoi( buf );
	return 0;
}


/*
 * Reads a long value
*/
	int
IniReadLong( const char *fname, const char *secName, const char *varName,
			 long *value, const long defaultLong )
{
	char buf[80];

	*value = defaultLong;
	if( -1 == IniReadStr( fname, secName, varName, buf, 0 ) )
		return -1;

	*value = atol( buf );
	return 0;
}


/*
 * Reads a boolean value
*/
	int
IniReadBool( const char *fname, const char *secName, const char *varName,
			 bool *value, const bool defaultBool )
{
	char buf[80];

	*value = defaultBool;
	if( -1 == IniReadStr( fname, secName, varName, buf, 0 ) )
		return -1;

	if( !memicmp( buf, "True", 4 ) || '1' == buf[0] ){
		*value = TRUE;
		return 0;
	}

	if( !memicmp( buf, "False", 5 ) || '0' == buf[0] ){
		*value = FALSE;
		return 0;
	}

	return -1;
}


/***************************************************************************/
/******************** ProBoard's SDK Get/SetIniVar *************************/
/***************************************************************************/
#if !defined( PB_SDK )
	bool
GetIniVar( char *fname, char *var, char *value, int len )
{
	char path[81], buf[MAX_INI_LINE];

#if 0
	char *p;

	p = strrchr( strcpy( path, fname ), '.' );
	if( !p ) strcat( path, ".INI" );
	else strcpy( p, ".INI" );
#endif

	chext( path, fname, ".INI" );

	if( -1 == IniReadStr(path, NULL, var, buf, 0 ) ) return FALSE;
	sprintf( value, "%-*s", len, buf );
	return TRUE;
}

	bool
SetIniVar( char *fname, char *var, char *value )
{
	char path[81];

#if 0
	char *p;

	p = strrchr( strcpy( path, fname ), '.' );
	if( !p ) strcat( path, ".INI" );
	else strcpy( p, ".INI" );
#endif

	chext( path, fname, ".INI" );

	if( -1 == IniWrite(path, NULL, var, value) ) return FALSE;
	return TRUE;
}
#endif


/***************************************************************************/
#if defined( DEBUG_INIFILE )
void main( void )
{
	char buf[MAX_INI_LINE];
	char *fn = "SAMPLE.INI";
	int st, i;
	long l;
	bool b;

	IniReadStr( fn, "Section", "FirstVar", buf, "default first var" );
	puts( buf );
	st = IniReadStr( fn, "Another", "Toggle", buf, 0 );
	if( -1 == st ) puts( "Toggle is off" );
	else puts( "Toggle is on" );
	st = IniReadStr( fn, 0, "NoSection", buf, 0 );
	if( -1 == st ) puts( "can't find NoSection" );
	else puts( buf );

	st = IniReadInt( fn, "Another", "IntVar", &i, 0 );
	if( -1 == st ) puts( "IntVar: error" );
	else printf( "IntVar = %d\n", i );
	st = IniReadInt( fn, "Another", "Int2Var", &i, 0 );
	if( -1 == st ) puts( "Int2Var: error" );
	else printf( "Int2Var = %d\n", i );
	st = IniReadInt( fn, "Another", "Int3Var", &i, 0 );
	if( -1 == st ) puts( "Int3Var: error" );
	else printf( "Int3Var = %d\n", i );

	st = IniReadLong( fn, "Last", "Long", &l, 0 );
	if( -1 == st ) puts( "Long: error" );
	else printf( "Long = %ld\n", l );
	st = IniReadLong( fn, "Last", "Long2", &l, 0 );
	if( -1 == st ) puts( "Long2: error" );
	else printf( "Long2 = %ld\n", l );
	st = IniReadLong( fn, "Last", "Long3", &l, 0 );
	if( -1 == st ) puts( "Long3: error" );
	else printf( "Long3 = %ld\n", l );

	st = IniReadBool( fn, "OkOneMore", "Bool", &b, TRUE );
	if( -1 == st ) puts( "Bool: error" );
	else printf( "Bool: %s\n", b == TRUE ? "True" : "False" );
	st = IniReadBool( fn, "OkOneMore", "Bool2", &b, TRUE );
	if( -1 == st ) puts( "Bool2: error" );
	else printf( "Bool2: %s\n", b == TRUE ? "True" : "False" );
	st = IniReadBool( fn, "OkOneMore", "Bool3", &b, TRUE );
	if( -1 == st ) puts( "Bool3: error" );
	else printf( "Bool3: %s\n", b == TRUE ? "True" : "False" );
	st = IniReadBool( fn, "OkOneMore", "Bool4", &b, TRUE );
	if( -1 == st ) puts( "Bool4: error" );
	else printf( "Bool4: %s\n", b == TRUE ? "True" : "False" );

	puts( "Removing MoreToggle..." );
	IniWrite( fn, "OkOneMore", "MoreToggle", 0 );
	st = IniReadStr( fn, "OkOneMore", "MoreToggle", buf, 0 );
	if( -1 == st ) puts( "More toggle is off" );
	else puts( "More toggle on" );
	puts( "Modifying Int3Var to -55..." );
	sprintf( buf, "%d", -55 );
	IniWrite( fn, "Another", "Int3Var", buf );
	st = IniReadInt( fn, "Another", "Int3Var", &i, 0 );
	if( -1 == st ) puts( "Int3Var: error" );
	else printf( "Int3Var = %d\n", i );
	puts( "Adding a new empty section NewSection" );
	IniWrite( fn, "NewSection", 0, 0 );
	puts( "Adding a section Added with variable Bool=True" );
	IniWrite( fn, "Added", "Bool", "True" );
	st = IniReadBool( fn, "Added", "Bool", &b, FALSE );
	if( -1 == st ) puts( "failed" );
	else printf( "Bool: %s\n", b == TRUE ? "True" : "False" );
	puts( "Adding to Last: LongToggle" );
	IniWrite( fn, "Last", "LongToggle", 0 );

	if( FALSE != GetIniVar( "PBSDK.INI", "NonExistant", buf, 80 ) )
		puts( "Ops, the file is not supposed to exist." );
	if( FALSE != SetIniVar( "PROFILE.DAT", "Entry", "Value" ) )
		puts( "Ok!" );
	else puts( "Darn!" );
	if( TRUE == GetIniVar( "PROFILE.DEF", "Entry", buf, 80 ) )
		printf( "Ok: %s\n", buf );
	else puts( "Darn!" );
}
#endif
