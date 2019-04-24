// PowerPEX Key Generator
// Version 1.0 - By Gregory Gulick

#include <pb_sdk.h>

void main(void);
void press_enter(void);

void press_enter(void)
{
	char ch;

	SetColor(WHITE);
	printf("\nPress [");
	SetColor(YELLOW);
	printf("Enter");
	SetColor(WHITE);
	printf("] to Continue.");
	ch = WaitKeys("\r");
}

void main(void)
{
	FILE *dfp, *fp;
	char board_name[100], rev_board[100];
	char sysop_name[100], rev_name[100];
	int x,y,z;
	time_t ct;

    dfp = fopen("C:\\TPS\\KEY\\POWERPEX.KEY","w");

	printf("\f");
	MenuFunction(MENU_SHOWANSASC,"PP-KEY");
	SetColor(WHITE);
	printf("\nEnter SysOp Name : ");Input(sysop_name,50,INPUT_ALL);
		if (strlen(sysop_name) == 0)
			exit();
	printf("\nEnter BBS Name   : ");Input(board_name,50,INPUT_ALL);
		if (strlen(board_name) == 0)
			exit();
	strcpy(rev_name,sysop_name);
	strcpy(rev_board,board_name);
	strrev(rev_name);
	strrev(rev_board);

	fprintf(dfp,"%d\n",strlen(rev_name));
	fprintf(dfp,"%d\n",strlen(rev_board));

	for (x=0;x!=strlen(rev_name);++x) {         // Start encryption
		fprintf(dfp,"%c",rev_name[x]);
		z = 48;
		for (y=1;y<31;++y) {                   // Print garbage loop.
			if ((z<48) || (z>254))             // Valid garbage?
			   z = 48;
			fputc(z,dfp);
			z = z+x;
		}
	}
	for (x=0;x!=strlen(rev_board);++x) {
		fprintf(dfp,"%c",rev_board[x]);
		z=48;
		for (y=1;y<31;++y) {
			if ((z<48) || (z>254))
			   z = 48;
			fputc(z,dfp);
			z=z+x;
		}
	}
	fclose(dfp);
	SetColor(BLUE);
	printf("\n\nThanks for using PowerPEXes!\n\n");
	SetColor(WHITE);
	printf("You will now be able to download your PowerPEX Key!\n");
	press_enter();
	MenuFunction(MENU_DOWNLOAD,"/F=C:\\TPS\\KEY\\POWERPEX.KEY /T");
	time(&ct);
	if (access("PP-KEY.LOG",00) != 0) {
		fp = fopen("PP-KEY.LOG","w");
		fprintf(fp,"---------------------------------------------------------------------------\n");
	}
	else
		fp = fopen("PP-KEY.LOG","a");
	fprintf(fp,"%s downloaded a PowerPEX key on %s",CurUser->name, ctime(&ct));
	fprintf(fp,"---------------------------------------------------------------------------\n");
	fclose(fp);
	printf("\f");
	SetColor(RED);
	printf(":: NOTE ::\n");
	SetColor(WHITE);
	printf("  Your POWERPEX.KEY should be placed in your ProBoard directory.\n");
	press_enter();
}
