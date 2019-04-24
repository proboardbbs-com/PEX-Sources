// ==========================================================================
// SLR E-Mail v1.0
// Asks new users for an e-mail address
// --------------------------------------------------------------------------
// Orginally known as QCS E-mail v1.0
// Written by Alan Wagstaff, QCS Software
// Released to SLASHER BBS - 19th May 2003
// Now supported and updated by John Riley, Primary SysOp of SLASHER BBS!
// SLASHER BBS  http://slasherbbs.dtdns.net/
// ==========================================================================

#include <pb_sdk.h>

void main()	// Start of PEX
	{

	char  	slr_mailing_list[200],
		slr_textfile[9],
		slr_prompt_x[4],
		slr_prompt_y[4],
		slr_prompt_text[81],
		slr_input_length[4],
		slr_input_x[4],
		slr_input_y[4],
		inifile[200],
		email[81],
		email_string[81];

	int px, py, ix, iy, il;

	FILE *fp;

	strcpy(inifile, Config->pexpath);
	strcat(inifile, "\SLREMAIL.INI");

	GetIniVar(inifile, "SLR_INPUT_LENGTH", slr_input_length, 4);
	GetIniVar(inifile, "SLR_INPUT_X", slr_input_x, 4);
	GetIniVar(inifile, "SLR_INPUT_Y", slr_input_y, 4);
	GetIniVar(inifile, "SLR_MAILING_LIST", slr_mailing_list, 199);
	GetIniVar(inifile, "SLR_TEXTFILE", slr_textfile, 80);
	GetIniVar(inifile, "SLR_PROMPT_X", slr_prompt_x, 4);
	GetIniVar(inifile, "SLR_PROMPT_Y", slr_prompt_y, 4);
	GetIniVar(inifile, "SLR_PROMPT_TEXT", slr_prompt_text, 80);

	// Check to see if we should use an ANS/AVT file
	if(strlen(slr_textfile)<1)
		{
		if(strlen(slr_input_length)<1)
			{
			strcpy(slr_input_length, "60");
			}
		if(strlen(slr_prompt_text)<1)
			{
			strcpy(slr_prompt_text, "Please enter your e-mail address below:");
			}

		il=atoi(slr_input_length);

		while(1)
			{
			printf("\n\n\007%s\n",slr_prompt_text);
			Input(email, il, INPUT_ALL);
			printf("\n\n\007You entered - \005%s\007 - Is this correct? ", email);
			if((Ask(TRUE))==TRUE)
				{
				break;
				}
			}

		if((fp=fopen(slr_mailing_list, "at"))==NULL)
			{
			Log(LOG_FRIEND, "SLR E-Mail ERROR: Could not open / create %s for writing", slr_mailing_list);
			Log(LOG_FRIEND, "SLR E-Mail ERROR: The e-mail address (%s) has not been saved", email);
			exit();
			}
		fseek(fp, 0L, SEEK_END);

		strcpy(email_string, email);
		strcat(email_string, " <");
		strcat(email_string, CurUser->name);
		strcat(email_string, ">\n");

		fputs(email_string, fp);

		fclose(fp);

		exit();
		}
	else
		{
		if((px=atoi(slr_prompt_x))<1)
			{
			Log(LOG_FRIEND, "SLR E-Mail ERROR: SLR_PROMPT_X is either less than 1, or is not a number.");
			Log(LOG_FRIEND, "SLR E-Mail ERROR: Exit (silent)");
			exit();
			}
		if((py=atoi(slr_prompt_y))<1)
			{
			Log(LOG_FRIEND, "SLR E-Mail ERROR: SLR_PROMPT_Y is either less than 1, or is not a number.");
			Log(LOG_FRIEND, "SLR E-Mail ERROR: Exit (silent)");
			exit();
			}
		if((ix=atoi(slr_input_x))<1)
			{
			Log(LOG_FRIEND, "SLR E-Mail ERROR: SLR_INPUT_X is either less than 1, or is not a number.");
			Log(LOG_FRIEND, "SLR E-Mail ERROR: Exit (silent)");
			exit();
			}
		if((iy=atoi(slr_input_y))<1)
			{
			Log(LOG_FRIEND, "SLR E-Mail ERROR: SLR_INPUT_Y is either less than 1, or is not a number.");
			Log(LOG_FRIEND, "SLR E-Mail ERROR: Exit (silent)");
			exit();
			}

		MenuFunction(MENU_SHOWANSASC, slr_textfile);

		if((strlen(slr_prompt_text)>1))
			{
			GotoXY(px,py);
			printf("\n\n\007%s\n",slr_prompt_text);
			}

		if(strlen(slr_input_length)<1)
			{
			strcpy(slr_input_length, "60");
			}

		il=atoi(slr_input_length);

		GotoXY(ix,iy);
		printf("\007");
		Input(email, il, INPUT_ALL);

		if((fp=fopen(slr_mailing_list, "at"))==NULL)
			{
			Log(LOG_FRIEND, "SLR E-Mail ERROR: Could not open / create %s for writing", slr_mailing_list);
			Log(LOG_FRIEND, "SLR E-Mail ERROR: The e-mail address (%s) has not been saved", email);
			exit();
			}
		fseek(fp, 0L, SEEK_END);

		strcpy(email_string, email);
		strcat(email_string, " <");
		strcat(email_string, CurUser->name);
		strcat(email_string, ">\n");

		fputs(email_string, fp);

		fclose(fp);

		exit();
		}

	exit();
	printf("  *** STOP TRYING TO HACK MY PEX ***  QCS E-Mail v1.0 was written by Alan Wagstaff, QCS Software.    SLR E-mail v1.0 is an updated version of QCS E-mail by SLASHER BBS, Primary SysOp, John Riley         COPYRIGHT (C) 2006 SLASHER BBS - ALL RIGHTS RESERVED ");
	}
