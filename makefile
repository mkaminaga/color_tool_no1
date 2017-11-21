# makefile
# date 2017-11-16
# Copyright 2017 Mamoru kaminaga
CC = C:\"Program Files (x86)"\"Microsoft Visual Studio\2017"\Community\VC\Tools\MSVC\14.10.25017\bin\HostX86\x86\cl.exe
LINK = C:\"Program Files (x86)"\"Microsoft Visual Studio\2017"\Community\VC\Tools\MSVC\14.10.25017\bin\HostX86\x86\link.exe

OBJDIR = build
TARGET = color01.exe
PDB = color01.pdb
MAP = color01.map
RES = resource.res
SRC =\
	canvas.cc\
	main.cc\
	palette.cc\
	range.cc\
	utility.cc
OBJ =\
	$(OBJDIR)/canvas.obj\
	$(OBJDIR)/main.obj\
	$(OBJDIR)/palette.obj\
	$(OBJDIR)/range.obj\
	$(OBJDIR)/utility.obj
LIBS = "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib"\
"advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib"\
"odbc32.lib" "odbccp32.lib" "Gdiplus.lib"

# Release build
CPPFLAGS = /nologo /W4 /Zi /O2 /MT /D"UNICODE" /D"_UNICODE" /EHsc /Fd"$(OBJDIR)/"
LFLAGS = $(LIBS) /NOLOGO /SUBSYSTEM:WINDOWS /PDB:"$(PDB)" /MAP:"$(MAP)"

# Debug build
#CPPFLAGS = /nologo /W4 /Zi /O2 /MT /D"UNICODE" /D"_UNICODE" /D"DEBUG" /EHsc /Fd"$(OBJDIR)/"
#LFLAGS = $(LIBS) /NOLOGO /SUBSYSTEM:WINDOWS /DEBUG /PDB:"$(PDB)" /MAP:"$(MAP)"

ALL: $(TARGET)

$(TARGET): $(OBJ) $(RES)
	$(LINK) $(LFLAGS) /OUT:$(TARGET) $(OBJ) $(RES)

.cc{$(OBJDIR)}.obj:
	@[ -d $(OBJDIR) ] || mkdir $(OBJDIR)
	$(CC) $(CPPFLAGS) /Fo"$(OBJDIR)\\" /c $<
