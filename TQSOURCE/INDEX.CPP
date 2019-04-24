
/* ------------------------------------------------------------
 * Filename ............... Index.Cpp
 *
 * General Purpose ........ Index class
 * ------------------------------------------------------------
 * First date ............. 12-28-94 06:20pm
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

/*-------------------------------------------------------------------------*/

LCL FILE *handle;

/*--] Code [-------------------------------------] Index [-----------------*/

/*
 * Routine   : Index()
 * Purpose   : the constructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Index::Index()
{
   handle = NULL;
   offset = 0;
   length = 0;
}

/*
 * Routine   : ~Index()
 * Purpose   : the destructor
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

Index::~Index()
{
   close();
}

/*
 * Routine   : open()
 * Purpose   : open the index file in the specified mode
 * ------------------------------------------------------------------------
 * Parameters: mode string
 * Return    : True if OK, False if NOT
 *
 */

FlgT Index::open(ChrP mode)
{
   close();

   ChrT fname[129];

#ifdef PEX
   sprintf(fname,"%s%s.IDX", ChrP(Config->pexpath), projectName);
#else
   sprintf(fname,"%s.IDX", projectName);
#endif

   handle = fopen(fname, mode);
   return(FlgT(handle != NULL));
}

/*
 * Routine   : close()
 * Purpose   : close the index file
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : None
 *
 */

void Index::close()
{
   if(handle != NULL)
   {
      fclose(handle);
      handle = NULL;
   }
}

/*
 * Routine   : seek()
 * Purpose   : seek to the specified record number
 * ------------------------------------------------------------------------
 * Parameters: record number
 * Return    : True if OK, False if NOT
 *
 */

FlgT Index::seek(IntT recno)
{
   FlgT ret_val = False;

   if(handle != NULL)
   {
#ifdef PEX
      LngT pos = l_mul(LngT(recno), LngT(sizeof(index_data)));
#else
      LngT pos = LngT(recno) * LngT(sizeof(index_data));
#endif
      if(fseek(handle, pos, SEEK_SET) == 0)
         ret_val = True;
   }

   return(ret_val);
}

/*
 * Routine   : read()
 * Purpose   : read the specified record number
 * ------------------------------------------------------------------------
 * Parameters: record number
 * Return    : True if OK, False if NOT
 *
 */

FlgT Index::read(IntT recno)
{
   FlgT ret_val = False;

   if(handle != NULL)
   {
      if(seek(recno) == True)
         if(fread(this, 1, sizeof(index_data), handle) == sizeof(index_data))
            ret_val = True;
   }

   return(ret_val);
}

/*
 * Routine   : write()
 * Purpose   : write the specified record
 * ------------------------------------------------------------------------
 * Parameters: record number
 * Return    : True if OK, False if NOT
 *
 */

FlgT Index::write(IntT recno)
{
   FlgT ret_val = False;

   if(handle != NULL)
   {
      if(seek(recno) == True)
         if(fwrite(this, 1, sizeof(index_data), handle) == sizeof(index_data))
            ret_val = True;
   }

   return(ret_val);
}

/*
 * Routine   : recCount()
 * Purpose   : count the number of record in the file
 * ------------------------------------------------------------------------
 * Parameters: None
 * Return    : -1 if failed, otherwise number of record
 *
 */

IntT Index::recCount()
{
   IntT ret_val = -1;

   FlgT wasOpen = True;
   if(handle == NULL)
   {
      open("rb");
      wasOpen = False;
   }

   if(handle != NULL)
   {
      if(fseek(handle, 0, SEEK_END) == 0)
      {
         LngT fileSize = ftell(handle);
#ifdef PEX
         ret_val = IntT(l_div(fileSize, LngT(sizeof(index_data))));
#else
         ret_val = IntT(fileSize / LngT(sizeof(index_data)));
#endif
      }
   }

   if(!wasOpen)
      close();

   return(ret_val);
}

/*---------------------------] END OF THE CODE [---------------------------*/
