@echo off

set FLUENT_INC=D:\GS\Fluent.Inc

set FLUENT_ARCH=ntx86

set FLUENT_REVISION=26

set USER_LOCAL=u:

set SEEMPATH=.;%FLUENT_INC%\fluent6.3.26\lib;%FLUENT_INC%\fluent6.3.26\client\lib;%FLUENT_INC%\fluent6.3.26\cortex\lib

set FLUENT_HELP=%FLUENT_INC%\fluent6.3.26\nthelp\main.hlp

set FLUENT_GTIHOME=%FLUENT_INC%\fluent6.3.26\gtpower

set FLUENT_WAVELIB=%FLUENT_INC%\fluent6.3.26\wave

set FLUENT_PROD=fluent

set FLUENT_PROD_DIR=%FLUENT_INC%\fluent6.3.26

set FLUENT_ADDONS=%FLUENT_INC%\addons

set PATH=%FLUENT_INC%\ntbin\ntx86;%FLUENT_INC%\ntbin\ntx86\opengl;%FLUENT_INC%\fluent6.3.26\vki\ntx86;%FLUENT_INC%\license\ntx86;%PATH%


set LM_LICENSE_FILE=%FLUENT_INC%\license\license.dat

start %FLUENT_INC%\fluent6.3.26\ntx86\2d\fl6326s.exe -i load.scm
