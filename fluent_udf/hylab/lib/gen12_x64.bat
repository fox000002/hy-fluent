@echo off

set FLUENT_INC=E:\Progs\ANSYS_Inc\v121\fluent
set VERSION=2d
set RELEASE=12.1.2
set LIB_RELEASE=1212
set FLUENT_ARCH=win64

set VERSION_STR1=2d 2ddp 3d 3ddp

set CWD=%CD%

for %%i in (%VERSION_STR1%) do (
if not exist %CWD%\fluent%RELEASE%\%FLUENT_ARCH%\%%i (
  mkdir %CWD%\fluent%RELEASE%\%FLUENT_ARCH%\%%i
  cd %CWD%\fluent%RELEASE%\%FLUENT_ARCH%\%%i
  pexports %FLUENT_INC%\fluent%RELEASE%\%FLUENT_ARCH%\%%i\fl%LIB_RELEASE%s.exe > fl%LIB_RELEASE%s.def
  dlltool --dllname fl%LIB_RELEASE%s.exe --def fl%LIB_RELEASE%s.def --output-lib libfl%LIB_RELEASE%s.a
 )
 
if not exist %CWD%\fluent%RELEASE%\%FLUENT_ARCH%\%%i_host (
  mkdir %CWD%\fluent%RELEASE%\%FLUENT_ARCH%\%%i_host
  cd %CWD%\fluent%RELEASE%\%FLUENT_ARCH%\%%i_host
  pexports %FLUENT_INC%\fluent%RELEASE%\%FLUENT_ARCH%\%%i_host\fl%LIB_RELEASE%.exe > fl%LIB_RELEASE%.def
  dlltool --dllname fl%LIB_RELEASE%.exe --def fl%LIB_RELEASE%.def --output-lib libfl%LIB_RELEASE%.a
 )
 
if not exist %CWD%\fluent%RELEASE%\%FLUENT_ARCH%\%%i_node (
  mkdir %CWD%\fluent%RELEASE%\%FLUENT_ARCH%\%%i_node
  cd %CWD%\fluent%RELEASE%\%FLUENT_ARCH%\%%i_node
  pexports %FLUENT_INC%\fluent%RELEASE%\%FLUENT_ARCH%\%%i_node\fl_net%LIB_RELEASE%.exe > fl_net%LIB_RELEASE%.def
  dlltool --dllname fl_net%LIB_RELEASE%.exe --def fl_net%LIB_RELEASE%.def --output-lib libfl_net%LIB_RELEASE%.a
  pexports %FLUENT_INC%\fluent%RELEASE%\%FLUENT_ARCH%\%%i_node\fl_mpi%LIB_RELEASE%.exe > fl_mpi%LIB_RELEASE%.def
  dlltool --dllname fl_mpi%LIB_RELEASE%.exe --def fl_mpi%LIB_RELEASE%.def --output-lib libfl_mpi%LIB_RELEASE%.a
 )
)

@pause
