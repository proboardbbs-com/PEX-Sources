@echo off

:: -----------------------------------------------------------------------
:: this batchfile is created for Borland C++ 4.x  If you are still using
:: Borland C++ 3.x, remove the -x- from the "bcc..." lines
:: -----------------------------------------------------------------------

:: -----------------------------------------------------------------------
:: path to the PB_SDK.OBJ directory
:: -----------------------------------------------------------------------
set sdk=h:\sdk\2.01\dos\p\

:: -----------------------------------------------------------------------
:: path to the BORLAND library directory
:: -----------------------------------------------------------------------
set borlib=f:\bc4\lib\

:: -----------------------------------------------------------------------
:: path to the borland and proboard include directory
:: -----------------------------------------------------------------------
set incdir=f:\bc4\include;h:\sdk\2.10\dev\hpp

:: -----------------------------------------------------------------------
:: directory where to place the object files
:: -----------------------------------------------------------------------
set objdir=..\obj\

:: -----------------------------------------------------------------------
:: directory where to place the executable files
:: -----------------------------------------------------------------------
set reldir=..\rel\

:: -----------------------------------------------------------------------
echo ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
echo ³ Creating FSED.PEX...                                              ³
echo ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
bcc -c -w -x- -ml -I%incdir% -n%objdir% -DPEX fsed.cpp clock.cpp index.cpp
tlink /n/c/x %sdk%pb_sdk.obj %objdir%fsed %objdir%clock %objdir%index,%reldir%fsed.pex

:: -----------------------------------------------------------------------
echo ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
echo ³ Creating COMPILER.EXE                                             ³
echo ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
bcc -c -w -x- -ml -I%incdir% -n%objdir% compiler.cpp clock.cpp index.cpp
tlink /n/c/x -Tde %borlib%c0l %objdir%compiler %objdir%clock %objdir%index,%reldir%compiler.exe,,%borlib%cl
