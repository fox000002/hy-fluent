# Microsoft Developer Studio Project File - Name="hylab" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=hylab - Win32 Debug_2DDP
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "hylab.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "hylab.mak" CFG="hylab - Win32 Debug_2DDP"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "hylab - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "hylab - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "hylab - Win32 Debug_3D" (based on "Win32 (x86) External Target")
!MESSAGE "hylab - Win32 Debug_2D" (based on "Win32 (x86) External Target")
!MESSAGE "hylab - Win32 Debug_2DDP" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "hylab - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f hylab.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "hylab.dll"
# PROP BASE Bsc_Name "hylab.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "nmake /f "hylab.mak""
# PROP Rebuild_Opt "/a"
# PROP Target_File "libhylab.dll"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "hylab - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f hylab.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "hylab.dll"
# PROP BASE Bsc_Name "hylab.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "nmake /f "hylab.mak"  VERSION=3ddp"
# PROP Rebuild_Opt "/a"
# PROP Target_File "libhylab.dll"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "hylab - Win32 Debug_3D"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "hylab___Win32_Debug_3D"
# PROP BASE Intermediate_Dir "hylab___Win32_Debug_3D"
# PROP BASE Cmd_Line "nmake /f "hylab.mak""
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "libhylab.dll"
# PROP BASE Bsc_Name ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "hylab___Win32_Debug_3D"
# PROP Intermediate_Dir "hylab___Win32_Debug_3D"
# PROP Cmd_Line "nmake /f "hylab.mak" VERSION=3d"
# PROP Rebuild_Opt "/a"
# PROP Target_File "libhylab.dll"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "hylab - Win32 Debug_2D"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "hylab___Win32_Debug_2D"
# PROP BASE Intermediate_Dir "hylab___Win32_Debug_2D"
# PROP BASE Cmd_Line "nmake /f "hylab.mak""
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "libhylab.dll"
# PROP BASE Bsc_Name ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "hylab___Win32_Debug_2D"
# PROP Intermediate_Dir "hylab___Win32_Debug_2D"
# PROP Cmd_Line "nmake /f "hylab.mak" VERSION=2d"
# PROP Rebuild_Opt "/a"
# PROP Target_File "libhylab.dll"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "hylab - Win32 Debug_2DDP"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "hylab___Win32_Debug_2DDP"
# PROP BASE Intermediate_Dir "hylab___Win32_Debug_2DDP"
# PROP BASE Cmd_Line "nmake /f "hylab.mak""
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "libhylab.dll"
# PROP BASE Bsc_Name ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "hylab___Win32_Debug_2DDP"
# PROP Intermediate_Dir "hylab___Win32_Debug_2DDP"
# PROP Cmd_Line "nmake /f "hylab.mak" VERSION=2ddp"
# PROP Rebuild_Opt "clean"
# PROP Target_File "libhylab.dll"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "hylab - Win32 Release"
# Name "hylab - Win32 Debug"
# Name "hylab - Win32 Debug_3D"
# Name "hylab - Win32 Debug_2D"
# Name "hylab - Win32 Debug_2DDP"

!IF  "$(CFG)" == "hylab - Win32 Release"

!ELSEIF  "$(CFG)" == "hylab - Win32 Debug"

!ELSEIF  "$(CFG)" == "hylab - Win32 Debug_3D"

!ELSEIF  "$(CFG)" == "hylab - Win32 Debug_2D"

!ELSEIF  "$(CFG)" == "hylab - Win32 Debug_2DDP"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\mydialog.c
# End Source File
# Begin Source File

SOURCE=.\src\myext.cpp
# End Source File
# Begin Source File

SOURCE=.\src\mymodels.c
# End Source File
# Begin Source File

SOURCE=.\src\myscheme.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\mydialog.h
# End Source File
# Begin Source File

SOURCE=.\src\myext.h
# End Source File
# Begin Source File

SOURCE=.\src\myscheme.h
# End Source File
# Begin Source File

SOURCE=.\src\myudf.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "UDF Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\mybc.c
# End Source File
# Begin Source File

SOURCE=.\src\mydmesh.c
# End Source File
# Begin Source File

SOURCE=.\src\mydpm.c
# End Source File
# Begin Source File

SOURCE=.\src\myhack.c
# End Source File
# Begin Source File

SOURCE=.\src\mymisc.c
# End Source File
# Begin Source File

SOURCE=.\src\mymphase.c
# End Source File
# Begin Source File

SOURCE=.\src\myprop.c
# End Source File
# Begin Source File

SOURCE=.\src\mysolver.c
# End Source File
# Begin Source File

SOURCE=.\src\mysource.c
# End Source File
# Begin Source File

SOURCE=.\src\myturb.c
# End Source File
# Begin Source File

SOURCE=.\src\myudf.c
# End Source File
# Begin Source File

SOURCE=.\src\myudm.c
# End Source File
# Begin Source File

SOURCE=.\src\myuns.c
# End Source File
# Begin Source File

SOURCE=.\src\myvector.c
# End Source File
# End Group
# End Target
# End Project
