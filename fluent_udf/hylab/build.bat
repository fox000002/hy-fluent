@echo off 

set VC6_DIR="D:\\GS\\VC6"

call %VC6_DIR%\VC98\\Bin\VCVARS32.bat

rm -vf *.obj *.dll

nmake /f hylab.mak VERSION=2d

@pause
