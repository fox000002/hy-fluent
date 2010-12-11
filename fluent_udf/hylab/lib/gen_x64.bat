@echo off

set FLUENT_INC=D:\GS\Fluent.Inc
set VERSION=2d
set RELEASE=6.3.26
set LIB_RELEASE=6326
set FLUENT_ARCH=win64

REM set VERSION_STR1=2d 2ddp 3d 3ddp
set VERSION_STR1=2d

set CWD=%CD%

for %%i in (%VERSION_STR1%) do (
if not exist %CWD%\%FLUENT_ARCH%\%%i (
  mkdir %CWD%\%FLUENT_ARCH%\%%i
  cd %CWD%\%FLUENT_ARCH%\%%i
  pexports_x64 %FLUENT_INC%\fluent%RELEASE%\%FLUENT_ARCH%\%%i\fl%LIB_RELEASE%s.exe
  dlltool --dllname fl%LIB_RELEASE%s.exe --def fl%LIB_RELEASE%s.def --output-lib libfl%LIB_RELEASE%s.a
 )
 
if not exist %CWD%\%FLUENT_ARCH%\%%i_host (
  mkdir %CWD%\%FLUENT_ARCH%\%%i_host
  cd %CWD%\%FLUENT_ARCH%\%%i_host
  pexports_x64 %FLUENT_INC%\fluent%RELEASE%\%FLUENT_ARCH%\%%i_host\fl%LIB_RELEASE%.exe
  dlltool --dllname fl%LIB_RELEASE%.exe --def fl%LIB_RELEASE%.def --output-lib libfl%LIB_RELEASE%.a
 )
 
if not exist %CWD%\%FLUENT_ARCH%\%%i_node (
  mkdir %CWD%\%FLUENT_ARCH%\%%i_node
  cd %CWD%\%FLUENT_ARCH%\%%i_node
  pexports_x64 %FLUENT_INC%\fluent%RELEASE%\%FLUENT_ARCH%\%%i_node\fl_net%LIB_RELEASE%.exe
  dlltool --dllname fl_net%LIB_RELEASE%.exe --def fl_net%LIB_RELEASE%.def --output-lib libfl_net%LIB_RELEASE%.a
  pexports_x64 %FLUENT_INC%\fluent%RELEASE%\%FLUENT_ARCH%\%%i_node\fl_mpi%LIB_RELEASE%.exe
  dlltool --dllname fl_mpi%LIB_RELEASE%.exe --def fl_mpi%LIB_RELEASE%.def --output-lib libfl_mpi%LIB_RELEASE%.a
 )
)

@pause
