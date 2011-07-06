@echo off

rm -vf *.obj *.o *.dll *.c *.opt *.exp *.lib *.plg *.ncb *.user *.suo *.so

rm -rvf Debug Release _UpgradeReport_Files hylab_*Debug* Debug_*

rm -vf res/*.res

rm -vf *.XML *.log *.sdf

rm -rvf x64

rm -rvf libhylab

rm -vf err

@pause
