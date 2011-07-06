@echo off 

REM set VC6_DIR="D:\\GS\\VC6"

REM call %VC6_DIR%\VC98\\Bin\VCVARS32.bat

set VS8_DIR="D:\\Progs\\VS8"

call %VS8_DIR%\VC\bin\amd64\vcvarsamd64.bat

nmake /f hylab.mak VERSION=2d FLUENT_ARCH=Win64

@pause
