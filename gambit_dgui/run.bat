@echo off

set FLUENT_INC=D:\GS\Fluent.Inc
set FLUENT_ARCH=ntx86
set GAMBIT=D:\GS\Fluent.Inc\gambit2.3.16
set GAMBIT_ROOT=D:\GS\Fluent.Inc\gambit2.3.16
set FLUENT_HELP=D:\GS\Fluent.Inc\gambit2.3.16\nthelp\main.hlp
set GAMBIT_INTEROP_ROOT=D:\GS\Fluent.Inc\gambit2.3.16
set GAMBIT_WRAPPER_SCR=D:\GS\Fluent.Inc/ntbin/ntx86/interop.exe
set GAMBIT_BITMAP_PATH=D:\GS\Fluent.Inc\gambit2.3.16\bitmaps
set GAMBIT_HELP_PATH=D:\GS\Fluent.Inc\gambit2.3.16\help
set P_SCHEMA=D:\GS\Fluent.Inc\gambit2.3.16\parasolid\schema
set DISPLAY=hu-7dc9623579ca:0.0
set XKEYSYMDB=C:\Documents and Settings\Administrator\Application Data\Hummingbird\Connectivity\13.00\Exceed\\XKeysymDB
set XERRORDB=C:\Documents and Settings\Administrator\Application Data\Hummingbird\Connectivity\13.00\Exceed\\XErrorDB
set XLOCALEDIR=C:\Documents and Settings\Administrator\Application Data\Hummingbird\Connectivity\13.00\Exceed\\locale
set XAPPLRESDIR=C:\Documents and Settings\Administrator\Application Data\Hummingbird\Connectivity\13.00\Exceed\
set FLUENT_PROD=gambit
set FLUENT_PROD_DIR=D:\GS\Fluent.Inc\gambit2.3.16
set PATH=C:\Program Files (x86)\Hummingbird\Connectivity\13.00\Exceed\;D:\GS\Fluent.Inc\gambit2.3.16\ntx86\xdll;D:\GS\Fluent.Inc\gambit2.3.16\ntx86\xdll\lib3dx\intel_a\code\bin;D:\GS\Fluent.Inc\gambit2.3.16\ntx86;D:\GS\Fluent.Inc\ntbin\ntx86;%PATH%

start D:\Progs\Hummingbird\Connectivity\13.00\Exceed\exceed.exe

D:\GS\Fluent.Inc\gambit2.3.16\ntx86\gambit.exe -device open -id 1111 -new -initfile hello.dg
