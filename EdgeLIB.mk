# makefile
# date 2017-11-16
# Copyright 2017 Mamoru kaminaga
CC = C:\"Program Files (x86)"\"Microsoft Visual Studio"\2017\Community\VC\Tools\MSVC\14.10.25017\bin\HostX86\x86\cl.exe
LIB = C:\"Program Files (x86)"\"Microsoft Visual Studio"\2017\Community\VC\Tools\MSVC\14.10.25017\bin\HostX86\x86\lib.exe
OBJDIR = build
TARGET = EdgeLIB.lib
SRC =\
	EdgeCompress.cpp\
	EdgeFile.cpp\
	EdgeLayer.cpp\
	StdAfx.cpp
OBJS =\
	$(OBJDIR)/EdgeCompress.obj\
 	$(OBJDIR)/EdgeFile.obj\
 	$(OBJDIR)/EdgeLayer.obj\
 	$(OBJDIR)/StdAfx.obj
CCFLAGS = /W4 /Zi /O2 /MT /EHsc /D"WIN32" /D"NODEBUG" /D"_LIB" /D"_UNICODE"\
	/D"UNICODE" /D"DIRECTINPUT_VERSION=0x0800" /Fo"$(OBJDIR)\\"

$(TARGET): $(OBJS)
	$(LIB) /OUT:$(TARGET) $(OBJS)

.cpp{$(OBJDIR)}.obj:
	@[ -d $(OBJDIR) ] || mkdir $(OBJDIR)
	$(CC) $(CPPFLAGS) /Fo"$(OBJDIR)\\" /c $<
