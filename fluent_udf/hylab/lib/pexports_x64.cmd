@echo off
if "%1"=="" goto :USAGE
:CHECK

dumpbin.exe >nul
if %ERRORLEVEL% EQU 1 goto :FAILURE_1
dumpbin.exe /EXPORTS %1 > tmp 2>nul
echo LIBRARY %~n1 >%~n1.def
echo. >>%~n1.def
echo EXPORTS >> %~n1.def
SET no=1
SETLOCAL ENABLEDELAYEDEXPANSION
for /F "skip=19 tokens=3*" %%i in (tmp) do (
echo %%j @!no! >>%~n1.def
SET /A no=!no!+1
)
endlocal 

if "%2"=="" goto :END
where lib.exe >nul
if %ERRORLEVEL% EQU 1 goto :FAILURE_2
lib /def:%~n1.def /machine:%2
goto :END

:USAGE
echo pexport <dll file> [x86|x64]
echo create import library from Dll  --- Ray Linn
goto :END

:FAILURE_1
echo Can not find the dumpbin.exe in your path.
goto :END

:FAILURE_2
echo Can not find the lib.exe in your path.
goto :END

:END
del tmp
