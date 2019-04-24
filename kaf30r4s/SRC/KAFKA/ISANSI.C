/*
 * This file is part of the KaFKa Kompiler
 * Copyright (C) 1995 by Branislav L. Slantchev
 *
 * Distributed under the GNU General Public License
 * See the file 'copying' for more information about your license.
 *
*/
#include <stdio.h>
#include <dos.h>

int waitChar(void){
	union REGS regs;
	regs.h.ah = 0x0b;
    int86(0x21, &regs, &regs);
    return regs.h.al ? 1 : 0;
}

int getChar(void){
	union REGS regs;
    regs.h.ah = 0x07;
    int86(0x21, &regs, &regs);
    return regs.h.al;
}

int isansi(void)
{
    fflush(stdin);
	fprintf(stdout, "\r\x1b[6n\r   \r");
    if( waitChar() && 0x1b == getChar() ){
		if( waitChar() && '[' == getChar() ){
        	while( waitChar() ) getChar();
            return 1;
		}
	}
    return 0;
}
