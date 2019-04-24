
/* ------------------------------------------------------------
 * Filename ............... FSEd.Cpp
 *
 * General Purpose ........ TheQuote main source file
 * ------------------------------------------------------------
 * First date ............. 12-28-94 05:57pm
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
#include "index.hpp"

/*-------------------------------------------------------------------------*/

#define  maxLines    15

/*-------------------------------------------------------------------------*/

LCL WrdT    quoteNums[maxLines];
LCL ChrP    quoteText[maxLines];
LCL IndexP  quoteIndx          ;

/*--] Code [---------------------------------------------------------------*/

/*
 * Routine   : centerLine()
 * Purpose   : center the specified line on the screen
 * ------------------------------------------------------------------------
 * Parameters: line number, formatted string
 * Return    : None
 *
 */

LCL void centerLine(IntT lineNo, ChrP fs, ...)
{
   ChrT buffer[128];
   va_list v;

   va_start(v, fs);
   vsprintf(buffer, fs, v);
   va_end(v);

   IntT realLen = strlen(buffer);
   IntT textLen = 0;
   for(IntT cnt = 0; cnt < realLen; cnt++)
   {
      if(BytT(buffer[cnt]) >= 32)
         textLen++;
   }

   GotoXY((80 - textLen) / 2, lineNo);
   printf(buffer);
}

/*
 * Routine   : makeRandoms()
 * Purpose   : create the random numbers
 * ------------------------------------------------------------------------
 * Parameters: The top quote number
 * Return    : None
 *
 */

LCL void makeRandoms(IntT maxVal)
{
   srand(unsigned(clockTicks()));            // we need a real random number

   for(IntT cnt = 0; cnt < maxLines; cnt++)
   {
      IntT x = -1;
      while(x == -1)
      {
         IntT l = (abs(rand()) % 255);       // pick a random quote number
         IntT h = (abs(rand()) % 255);
         x = (abs(l * h) % maxVal);

         if(cnt != 0)                        // check if not already used...
            for(IntT i = 0; i < cnt; i++)
               if(quoteNums[i] == x)
                  x = -1;
      }

      quoteNums[cnt] = x;
   }
}

/*
 * Routine   : readQuotes()
 * Purpose   : read all the required quotes
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : True if OK, False if NOT
 *
 */

LCL FlgT readQuotes()
{
   FlgT ret_val = True;

   if(quoteIndx->open("rb") == True)
   {
      ChrT filename[128];
      sprintf(filename, "%s%s.DAT", ChrP(Config->pexpath), projectName);
      FILE *handle = fopen(filename,"rb");
      if(handle != NULL)
      {
         for(IntT cnt = 0; cnt < maxLines && ret_val == True; cnt++)
         {
            if(quoteText[cnt] != NULL)
            {
               free(quoteText[cnt]);
               quoteText[cnt] = NULL;
            }

            quoteIndx->read(quoteNums[cnt]);

            ChrT buffer[128];
            if(fseek(handle, quoteIndx->offset, SEEK_SET) == 0)
            {
               if(fread(buffer, 1, quoteIndx->length, handle) == quoteIndx->length)
               {
                  for(IntT i = quoteIndx->length - 1; i >= 0; i--)
                     buffer[i] ^= (i == 0 ? xorData : buffer[i - 1]);

                  buffer[quoteIndx->length] = '\0';
                  quoteText[cnt] = strdup(buffer);
               }
            }
         }

         fclose(handle);
      }
      else
         ret_val = False;

      quoteIndx->close();
   }
   else
      ret_val = False;

   return(ret_val);
}

/*
 * Routine   : displayQuotes()
 * Purpose   : display the quotes on the screen
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

LCL void displayQuotes()
{
   CurUser->uFlags |= UFLAG_CLEAR;     // for those not having CLS enabled...
   printf("\7\f");                     // clear screen and add two blank lines

   // due to a bug in the PB_SDK.H file, MAGENTA and BLUE are reversed.
   // where you see BLUE, ProBoard will use MAGENTA...
   centerLine(2, "%c%s %cversion %c%s.%c (C) %s %s",
                  BLUE , projectName,
                  WHITE,
                  BLUE , versionText,
                  WHITE,
                  copyrightYear,
                  companyName
             );

   centerLine(3, "%c컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴", WHITE);

   for(IntT cnt = 0; cnt < maxLines; cnt++)
   {
      GotoXY(1, 5 + cnt);

      printf(" %c[%c%c%c] %c%s",
               WHITE , YELLOW, cnt + 'A', WHITE,
               CYAN  , (quoteText[cnt] != NULL
                          ? quoteText[cnt]
                          : "-= ERROR! =-"
                       )
            );
   }

   centerLine(21, "%c[%cA%c..%c%c%c]...: %cSelect quote   %c[%c*%c]...: %cMore quotes   %c[%cENTER%c]...: %cDon't quote message",
                  WHITE, YELLOW, WHITE, YELLOW, 'A' + maxLines - 1, WHITE, CYAN ,
                  WHITE, YELLOW, WHITE, CYAN  ,
                  WHITE, YELLOW, WHITE, CYAN
             );
}

/*
 * Routine   : handle()
 * Purpose   : handle the quotes
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : text to add at the bottom of the message (NULL if none)
 *
 */

LCL ChrP handle()
{
   ChrP ret_val = NULL;

   IntT maxQuotes = quoteIndx->recCount();
   if(maxQuotes > 0)
   {
      while(ret_val == NULL)
      {
         makeRandoms(maxQuotes);
         if(readQuotes() == True)
         {
            displayQuotes();

            ChrT choice = WaitKeys("ABCDEFGHIJKLMNO\x1B\r*");

            // --- ENTER or ESCAPE -------------------------------------
            if((choice == 0x1B) || (choice == '\r'))
               break;

            // --- A .. O ----------------------------------------------
            if((choice >= 'A') && (choice <= 'O'))
            {
               IntT quote = choice - 'A';
               if(quoteText[quote] != NULL)
               {
                  GotoXY(0, 5 + quote);
                  printf("%c>", BLINK|RED);
                  GotoXY(1, 21);

                  ret_val = quoteText[quote];
               }
            }
         }
         else
            break;
      }
   }
   return(ret_val);
}

/*
 * Routine   : init()
 * Purpose   : initialize the PEX
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

LCL void init()
{
   quoteIndx = new IndexT;

   for(IntT cnt = 0; cnt < maxLines; cnt++)
      quoteText[cnt] = NULL;
}

/*
 * Routine   : kill()
 * Purpose   : kill the PEX
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

LCL void kill()
{
   for(IntT cnt = 0; cnt < maxLines; cnt++)
      if(quoteText[cnt] != NULL)
         free(quoteText[cnt]);

   delete quoteIndx;
}

/*
 * Routine   : main()
 * Purpose   : the main function
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void main()
{
   ChrT buffer[128];

   // This is the piece of code to determine if the editor is a PEX
   // message editor.
   if(Config->editorname[0] != '@')    // EXE editor
   {
      sprintf(buffer,"*N*Q%s *P *B *T *I", Config->editorname);
      MenuFunction(MENU_SHELL, buffer);
   }
   else                                // PEX editor
   {
      strcpy(buffer, Config->editorname + 1);
      MenuFunction(MENU_RUN_SDKFILE, buffer);
   }

   // Ok, we are back, check if the message isn't aborted...
   struct find_t find;
   if(dos_findfirst("MSGTMP", 0, &find) == 0)
   {
      init();
      ChrP quote = handle();
      if(quote != NULL)
      {
         ChrT appendText[256];

         sprintf(buffer,"%s.INI", projectName);
         if(GetIniVar(buffer, "QuoteHeader", buffer, 80) == True)
            sprintf(appendText, "\n%s\n%s\n\n", buffer, quote);
         else
            sprintf(appendText, "\n... And now, some words of wisdom:\n%s\n\n", quote);

         AppendMSGTMP(appendText);
      }

      kill();
   }
}

/*---------------------------] END OF THE CODE [---------------------------*/
