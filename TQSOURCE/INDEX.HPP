
/* ------------------------------------------------------------
 * Filename ............... Index.Hpp
 *
 * General Purpose ........ Index class
 * ------------------------------------------------------------
 * First date ............. 12-28-94 06:17pm
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

#ifndef __INDEX_HPP
#  define __INDEX_HPP

/*--] Structures [-------------------------------] index_data [------------*/

   struct index_data
   {
      LngT        offset;
      BytT        length;
   };

/*--] Structures [-------------------------------] Index [-----------------*/

   _StructType(Index);
   struct Index : public index_data
   {
                  Index          (                                      );
                  ~Index         (                                      );

      FlgT        open           (  ChrP mode                           );
      void        close          (                                      );
      FlgT        seek           (  IntT recno                          );
      FlgT        read           (  IntT recno                          );
      FlgT        write          (  IntT recno                          );
      IntT        recCount       (                                      );
   };

/*-------------------------------------------------------------------------*/

#endif

/*---------------------------] END OF THE CODE [---------------------------*/
