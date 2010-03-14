@echo off

rm -vf *.obj *.dll *.c *.opt *.exp *.lib *.plg *.ncb *.user *.suo

rm -rvf Debug Release _UpgradeReport_Files hylab_*Debug*

rm -vf res/*.res

rm -vf *.XML *.log *.sdf

rm -rvf x64

@pause
