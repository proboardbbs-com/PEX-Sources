/* ProTag II - Version 1.0
   Written by Gregory Gulick
   Enhanced By Tiago Pascoal, so that it would run PEX as editors
   Copyright 1994 - All Rights Reserved

 Note: This is the original coding, i've only added support for PEX editors
changed a few lines, which i thought they would look better, didn't even
looked at the rest of the code, which could probably be optimized as well.
 Also changed the C++ type comments to C comments.

 The color codes could be improved. (maybe in the next version)

                                          Tiago Pascoal
*/

#include "pb_sdk.h"

#define CTRUE 0
#define CFALSE -1

#define LETTER "ABCDEFGHIJKLMNO"

/* Prototypes */

void main(int argc, char **argv);
void no_memory(void);
void header(void);
void press_enter(void);
void read_cmd_line(char command_line[100]);
void get_pex_directory(char command_line[100]);
int get_random(void);
void get_total_tags(void);
void select_tags(void);
void print_quote(int x);

/* Global Variables */

    char ch;                               /* Multipurpose character */

    char argument[100][100];               /* Command line arguments */
    int total_arguments = 0;               /* Total number of arguments */

    int tt;                                /* Total tags */

    char tag[101][101];                    /* Actual tags for selection */
    char PEX_DIRECTORY[100];               /* Location of PEXes */

/* -------------------------------------------------------------------------
  These are the general purpose, PEX utilities I use in every program:

   * No Memory alert
   * Program header
   * Press enter to continue
   * Read command line parsed by commas
   * Read PEX directory
------------------------------------------------------------------------- */

void no_memory(void)
{
	printf("\f");
	SetColor(RED);
	printf("Insufficient memory to run PowerPEX!\n");
	press_enter();
    exit();                                     /* Abort program */
}

void press_enter(void)
{
	SetColor(WHITE);
	printf("\nPress [");
	SetColor(YELLOW);
	printf("Enter");
	SetColor(WHITE);
	printf("] to Continue.");
	ch=WaitKeys("\r");
}

void header(void)
{
		printf("\f");
		SetColor(WHITE);
        printf("ProTag II v1.0 ");
		SetColor(CYAN);
		printf("� ");
		SetColor(BLUE);
        printf("A PowerPEX by Gregory Gulick and Enhanced by Tiago Pascoal  ");
		SetColor(CYAN);
		printf("          (C) Copyright 1994\n");
		SetColor(MAGENTA);
		printf("컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�\n\n");
}

void read_cmd_line(char command_line[100])
{

    /* This function parses the command line and places the results in the
       variable "argument[]" starting at element 1.  This is used to get
       around the 8 or 9 parameter limit on the command line. */

	char *token;

/*    char tokensep[]=",";  this was giving me probles due the way my
      compiler initializes strings anyway it was quite useless TP.
*/

    token = strtok(command_line,",");

	while (token != NULL) {
		++total_arguments;
		strcpy(argument[total_arguments],token);
        token = strtok(NULL, ",");
	}
}

void get_pex_directory(char command_line[100])
{

    /* This function returns the user's PEX directory by parsing command
       line parameter 0 which is the program's full pathname and filename.
       The result is placed in the global variable PEX_DIRECTORY and can
       be used by other funtions. */

	char *location, *locations;

	if ((location = (char *) calloc(1,100)) == NULL)
		no_memory();

	locations = location;

	location = strrchr(command_line,'\\');
	strncpy(PEX_DIRECTORY, command_line,
		strlen(command_line)-strlen(location)+1);

	free(locations);
}

/*-------------------------------------------------------------------------
   Beginning of main program body
  ------------------------------------------------------------------------- */

int get_random(void)
{
	time_t tnow;
	char current_time[100], *seconds, *secondss;
	int secs, x;

    /* This procedure reads the clock and sets the random number seed
       using the current second plus the user's login day. */

	if ((seconds = (char *) calloc(1,100)) == NULL)
		no_memory();

	secondss = seconds;

	time(&tnow);
	sprintf(current_time,"%s",ctime(&tnow));

	seconds = strrchr(current_time,':');
    secs = atoi(++seconds);                /* Skip : and make integer */

    srand(secs + rand()); ; /* Set random number seed */

	free(secondss);

	do {
		x = rand();
	} while (x > tt-15);
	return(x);
}

void get_total_tags(void)
{
	FILE *fp;
	char data[100], filename[150];

	if (strlen(PEX_DIRECTORY) + strlen("PROTAG.TAG") > 100) {
		printf("\f\001Filename and path too long.\n");
		press_enter();
		exit();
	}

	strcpy(filename,PEX_DIRECTORY);
	strcat(filename,"PROTAG.TAG\x0");

	if ((fp = fopen(filename,"r")) == NULL) {
		printf("\f\001%s not found.\n",filename);
		press_enter();
		exit();
	}

	tt = 0;

	while ((fgets(data,90,fp) != NULL)
		|| (tt > 30000))
			++tt;

	if (tt < 15) {
		printf("\f\001Not enough tag lines in %s.\n",filename);
		press_enter();
		exit();
	}

	fclose(fp);
}

void show_tags(void)
{
	int loop, x;
	FILE *fp;
	char data[105];
	char filename[150];

	header();

	x = get_random();

	strcpy(filename,PEX_DIRECTORY);
	strcat(filename,"PROTAG.TAG\x0");

	if ((fp = fopen(filename,"r")) == NULL) {
		printf("\f\001%s not found.\n",filename);
		press_enter();
		exit();
	}

	for (loop = 1; loop < x; ++loop)
		fgets(data,100,fp);

	for (loop = 1; loop <= 15; ++loop) {
		fgets(data,100,fp);
		strncpy(tag[loop],data,70);
		if (strstr(tag[loop],"\n") == NULL)
			strcat(tag[loop],"\n");
	}

	for (loop = 1; loop <= 15; ++loop)
		printf("\006[\004%c\006] \007%s",LETTER[loop-1],tag[loop]);

	fclose(fp);
}

void print_quote(int x)
{
	FILE *fp;
	char quote_string[70], config_file[100];

	strcpy(config_file,PEX_DIRECTORY);
	strcat(config_file,"PROTAG.CFG\x0");

	if ((fp = fopen(config_file,"r")) == NULL)
        strcpy(quote_string,"... ProTag II - Version 1.0\n");
	else
		fgets(quote_string,60,fp);

	fclose(fp);

	if ((fp = fopen("MSGTMP","a")) == NULL) {
		printf("\f\001Cannot find MSGTMP.\n");
		press_enter();
		fclose(fp);
		exit();
	}

	fputs(quote_string,fp);
	fputs(tag[x],fp);
	fclose(fp);
}

void main(int argc, char **argv)
{
	char buf, shell_string[100];
	int tag_wanted;
	bool done;

    ++argc;                                   /* To suppress warning */


    if(Config->editorname[0]=='@')
      MenuFunction(MENU_RUN_SDKFILE,&Config->editorname[1]);
    else{ /* the editor is an exe file. Keeping authors coding TP*/
     sprintf(shell_string,"%s *Q",Config->editorname);
     MenuFunction(MENU_SHELL,shell_string);
        }

	if (access("MSGTMP",00) == -1)
		exit();

	get_pex_directory(argv[0]);

	get_total_tags();

	done = FALSE;

	do {
		show_tags();
		printf("\n\007Enter tag line ([\003*\007] for more, [\003Enter\007] for none) : ");
		buf = WaitKeys("ABCDEFGHIJKLMNO*\r");
		switch (buf) {
/* more efficient the previous method was getting on my nerves TP */
            case '*' : done = FALSE; break;
            case '\r': printf("\007None.");
                       exit();
            default  : tag_wanted = buf-64; done = TRUE; break;

                     }
	printf("%c",buf);
	} while (!done);
	print_quote(tag_wanted);
}

