###########################################################
# @(#)makefile_nt.udf	1.26 Copyright 06/28/00 Fluent Inc.
#
#	All Rights Reserved
#
#	This is unpublished proprietary source code of Fluent Inc.
#	It is protected by U.S. copyright law as an unpublished work
#	and is furnished pursuant to a written license agreement.  It
#	is considered by Fluent Inc. to be confidential and may not be
#	used, copied, or disclosed to others except in accordance with
#	the terms and conditions of the license agreement.
#
###########################################################
# 
# purpose:
#    This file is the makefile for NT UDF builds.
#
# usage:
#    Change file name 'makefile_nt.udf' to 'makefile'.
#    Place the makefile file in the target dir.
#    Place the helper file 'user_nt.udf' in same directory.
#    Edit user_nt.udf file.
#    Execute an 'nmake'  (no arguments needed).
#
# notes:
#    The user should not need to change this file.
#    The following macros are expected to be in the user_nt.udf file:
#      SOURCES, VERSION, PARALLEL_NODE and USER_OBJECTS.
#    The macro FLUENT_LIB is automatically defined here based on VERSION
#      and PARALLEL_NODE.
#    Only this makefile and the helper file 'user_nt.udf' needs to be 
#      the target dir, all other files are automatically generated, 
#      and removed in an 'nmake clean'.
#    The source file is expected in ..\..\src directory.
#    Make sure the VC compiler is set up for command line mode, use
#      the file Vcvars32.bat from Visual Studio to assist this. 
#    This makefile uses the public domain program 'sed', make sure
#      sed is in your executable path.
#    The '^' char in the SRC literal is a quote for the '\' char,
#      otherwise interpretted as a line continuation char.
#    Use the 'nmake clean' command to restart builds.
###########################################################

!INCLUDE user_nt.udf

# These need to be updated for each release.
LIB_RELEASE = 6326
RELEASE = 6.3.26

# it is assumed $(FLUENT_INC)is already set.
!IF !DEFINED(FLUENT_INC)
!  ERROR You need to define the environmental variable FLUENT_INC.
!ENDIF

!IF ("$(PROCESSOR_ARCHITECTURE)" == "ALPHA")
FLUENT_ARCH = ntalpha
!ENDIF

!IF ("$(PROCESSOR_ARCHITECTURE)" == "x86")
FLUENT_ARCH = ntx86
!ENDIF

!IF ("$(PROCESSOR_ARCHITECTURE)" == "AMD64")
FLUENT_ARCH = win64 
!ENDIF

# set FLUENT_LIB based on VERSION macro

!IF (("$(VERSION)" == "2d") || ("$(VERSION)" == "3d") || \
     ("$(VERSION)" == "2ddp") || ("$(VERSION)" == "3ddp"))
FLUENT_LIB = fl$(LIB_RELEASE)s.lib
!ENDIF

!IF (("$(VERSION)" == "2d_host") || ("$(VERSION)" == "3d_host") || \
     ("$(VERSION)" == "2ddp_host") || ("$(VERSION)" == "3ddp_host"))
FLUENT_LIB = fl$(LIB_RELEASE).lib
!ENDIF

!IF (("$(VERSION)" == "2d_node") || ("$(VERSION)" == "3d_node") || \
     ("$(VERSION)" == "2ddp_node") || ("$(VERSION)" == "3ddp_node"))
!IF (("$(PARALLEL_NODE)" != "none") && ("$(PARALLEL_NODE)" != "net"))
PARALLEL_NODE = mpi
!ENDIF
FLUENT_LIB = fl_$(PARALLEL_NODE)$(LIB_RELEASE).lib
!ENDIF


###########################################################

SRC = .\src^\

CC = cl

SED_CMD = $(FLUENT_INC)\ntbin\$(FLUENT_ARCH)\sed.exe
FLUENT_CMD = $(FLUENT_INC)\ntbin\$(FLUENT_ARCH)\fluent 2d

# compile flags 
#  /c for compile only,  /Za for ansi c and __STDC__ is set
CFLAGS = /c /Za /DUDF_EXPORTING


TARGET = libhylab.dll
UDFDATA = udf_names.c

# generate object names from source names
SOURCES2 = $(SOURCES:.\src\=)
SRC_OBJECT = $(SOURCES2:.c=.obj)
UDF_OBJECT = $(UDFDATA:.c=.obj)

MISC_SRCS = $(SRC)mydialog.c
MISC_SRCS2 = $(MISC_SRCS:.\src\=)
MISC_OBJS = $(MISC_SRCS2:.c=.obj)

RCC = rc /V /X /I"D:\GS\VC6\VC98\Include"

RC_FILE = .\res\mydialog.rc
#RC_FILE2 = $(RC_FILE:.\src\=)
RES_FILE = $(RC_FILE:.rc=.res)

OBJECTS = $(UDF_OBJECT) $(SRC_OBJECT) $(USER_OBJECTS) $(MISC_OBJS)

INCLUDES= -I$(FLUENT_INC)\fluent$(RELEASE)\$(FLUENT_ARCH)\$(VERSION) \
          -I$(FLUENT_INC)\fluent$(RELEASE)\src \
          -I$(FLUENT_INC)\fluent$(RELEASE)\cortex\src \
          -I$(FLUENT_INC)\fluent$(RELEASE)\client\src \
          -I$(FLUENT_INC)\fluent$(RELEASE)\tgrid\src \
          -I$(FLUENT_INC)\fluent$(RELEASE)\multiport\src

LIBS = /Libpath:$(FLUENT_INC)\fluent$(RELEASE)\$(FLUENT_ARCH)\$(VERSION) user32.lib


default:   $(TARGET)

$(UDF_OBJECT): $(UDFDATA)
	$(CC) $(CFLAGS) $(INCLUDES)  $**
	
$(SRC_OBJECT): $(SOURCES)
	$(CC) $(CFLAGS) $(INCLUDES)  $**
    
$(MISC_OBJS) : $(MISC_SRCS)
	$(CC) /c $**
    
$(RES_FILE) : $(RC_FILE)
	$(RCC) $**
	
$(TARGET): hylab.mak user_nt.udf $(UDF_OBJECT) $(SRC_OBJECT) $(MISC_OBJS) $(RES_FILE)
	@echo # Linking $@ because of $?
	link  $(LIBS) /dll   /out:$(TARGET) \
	      $(OBJECTS)  $(FLUENT_LIB) $(RES_FILE)
	
$(UDFDATA): hylab.mak $(SRC_OBJECT)
	@echo # Generating $@ because of $?
	@echo /* This file generated automatically. */ > $@
	@echo /*          Do not modify.            */ >> $@
	@echo #include "udf.h" >> $@
	@echo #include "prop.h" >> $@
	@echo #include "dpm.h" >> $@
		
	@$(SED_CMD) -n "s/^\(DEFINE_[_A-Z]*(.*)\)/extern \1;/p" $(SOURCES) >> $@
	
	@echo __declspec(dllexport) UDF_Data udf_data[] = { >> $@
	
	@$(SED_CMD) -n "s/^DEFINE_\([_A-Z]*\)( *\([_a-zA-Z0-9]*\).*)/\{\"\2\", (void (*)())\2, UDF_TYPE_\1\},/p" $(SOURCES) >> $@
	
	@echo }; >> $@
	
	@echo __declspec(dllexport) int n_udf_data = sizeof(udf_data)/sizeof(UDF_Data); >> $@
	@echo #include "version.h" >> $@
	@echo __declspec(dllexport) void UDF_Inquire_Release(int *major, int *minor, int *revision) >> $@
	@echo { >> $@
	@echo   *major = RampantReleaseMajor; >> $@
	@echo   *minor = RampantReleaseMinor; >> $@
	@echo   *revision = RampantReleaseRevision; >> $@
	@echo } >> $@
	

clean:
	@del  *.dll *.obj *.ilk *.exp *.lib *.pdb *.c
    
run:
	@$(FLUENT_CMD)
