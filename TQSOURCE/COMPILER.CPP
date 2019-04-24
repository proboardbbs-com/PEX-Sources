
/* ------------------------------------------------------------
 * Filename ............... Compiler.Cpp
 *
 * General Purpose ........ Compiler functions
 * ------------------------------------------------------------
 * First date ............. 12-28-94 09:07pm
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
#include "index.hpp"

/*--] Code [---------------------------------------------------------------*/

/*
 * Routine   : centerLine()
 * Purpose   : center the specified line on the screen
 * ------------------------------------------------------------------------
 * Parameters: formatted string
 * Return    : None
 *
 */

LCL void centerLine(ChrP fs, ...)
{
   ChrT buffer[128];
   va_list v;

   va_start(v, fs);
   vsprintf(buffer, fs, v);
   va_end(v);

   IntT len   = strlen(buffer);
   IntT space = (80 - len) / 2;
   printf("%*.*s%s\n", space, space, " ", buffer);
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
   printf("\n\n");
   centerLine("%s compiler v%s. (C) %s %s",
               projectName,
               versionText,
               copyrightYear,
               companyName
             );

   centerLine("ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ");

   printf("\n");

   // --- try to open the index file -----------------------------------
   Index index;
   if(index.open("wb") == False)
   {
      printf("Cannot open the index file for writing!\n\a");
      return;
   }

   ChrT buffer[129];

   // --- try to open the database file --------------------------------
   sprintf(buffer,"%s.DAT", projectName);
   FILE *output = fopen(buffer, "wb");
   if(output == NULL)
   {
      printf("Cannot open the database file for writing!\n\a");
      return;
   }

   // --- try to find quote files --------------------------------------
   sprintf(buffer,"%s.T??", projectName);
   struct find_t find;
   IntT status = _dos_findfirst(buffer, 0, &find);
   if(status != 0)
   {
      printf("No datafiles found in the current directory!\n\a");
      return;
   }

   // --- compile each datafile ----------------------------------------
   IntT quotes = 0;
   while(status == 0)
   {
      FILE *input = fopen(find.name, "rt");
      if(input == NULL)
      {
         printf("Cannot open %s for reading!\n\a", find.name);
      }
      else
      {
         printf("\nş Processing %s :", find.name);
         while(!feof(input))
         {
            if(fgets(buffer, 128, input) + NULL)
            {
               IntT len = strlen(buffer);
               if(buffer[len - 1] == '\n')
                  len--;

               ChrT mask = xorData;
               for(IntT i = 0; i < len; i++)
               {
                  buffer[i] = buffer[i] ^ mask;
                  mask = buffer[i];
               }

               index.offset = ftell(output);
               index.length = len;

               fwrite(buffer, 1, len, output);
               index.write(quotes++);
            }
         }

         fclose(input);
      }

      status = _dos_findnext(&find);
   }

   printf("\n\nş %d quotes compiled!\n\a", quotes);

   index.close();
   fclose(output);
}

/*---------------------------] END OF THE CODE [---------------------------*/
