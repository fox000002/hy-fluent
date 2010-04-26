@echo off

set FLUENT_INC=D:\GS\Fluent.Inc
set VERSION=2d
set RELEASE=6.3.26
set LIB_RELEASE=6326
set FLUENT_ARCH=ntx86

set VERSION_STR1=2d 2ddp 3d 3ddp

set CWD=%CD%

for %%i in (%VERSION_STR1%) do (
if not exist %CWD%\ntx86\%%i (
  mkdir %CWD%\ntx86\%%i
  cd %CWD%\ntx86\%%i
  pexports %FLUENT_INC%\fluent%RELEASE%\%FLUENT_ARCH%\%%i\fl6326s.exe > fl6326s.def
  dlltool --dllname fl6326s.exe --def fl6326s.def --output-lib libfl6326s.a
 )
 
if not exist %CWD%\ntx86\%%i_host (
  mkdir %CWD%\ntx86\%%i_host
  cd %CWD%\ntx86\%%i_host
  pexports %FLUENT_INC%\fluent%RELEASE%\%FLUENT_ARCH%\%%i_host\fl6326.exe > fl6326.def
  dlltool --dllname fl6326.exe --def fl6326.def --output-lib libfl6326.a
 )
 
if not exist %CWD%\ntx86\%%i_node (
  mkdir %CWD%\ntx86\%%i_node
  cd %CWD%\ntx86\%%i_node
  pexports %FLUENT_INC%\fluent%RELEASE%\%FLUENT_ARCH%\%%i_node\fl_net6326.exe > fl_net6326.def
  dlltool --dllname fl_net6326.exe --def fl_net6326.def --output-lib libfl_net6326.a
  pexports %FLUENT_INC%\fluent%RELEASE%\%FLUENT_ARCH%\%%i_node\fl_mpi6326.exe > fl_mpi6326.def
  dlltool --dllname fl_mpi6326.exe --def fl_mpi6326.def --output-lib libfl_mpi6326.a
 )
)

@pause
