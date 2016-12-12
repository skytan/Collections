# Microsoft Developer Studio Project File - Name="Demo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Demo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Demo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Demo.mak" CFG="Demo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Demo - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Demo - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Demo - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "Demo - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /Za /W4 /GX /Z7 /Od /I "..\..\..\include" /D "_DEBUG" /D "FT_DEBUG_LEVEL_ERROR" /D "FT_DEBUG_LEVEL_TRACE" /D "WIN32" /D "_MBCS" /D "_LIB" /D "FT2_BUILD_LIBRARY" /FR"..\..\..\objs\debug_st/" /Fo"..\..\..\objs\debug_st/" /Fd"..\..\..\objs\debug_st/" /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Demo - Win32 Release"
# Name "Demo - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\src\autofit\autofit.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\bdf\bdf.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\cff\cff.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\demo\ft_bitmap.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\base\ftbase.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\base\ftbbox.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\base\ftbdf.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\base\ftbitmap.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\cache\ftcache.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\base\ftdebug.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\base\ftfntfmt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\base\ftfstype.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\base\ftgasp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\base\ftglyph.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\base\ftgxval.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gzip\ftgzip.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\base\ftinit.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\lzw\ftlzw.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\base\ftmm.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\base\ftotval.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\base\ftpfr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\base\ftstroke.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\base\ftsynth.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\base\ftsystem.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\base\fttype1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\base\ftwinfnt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\pcf\pcf.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\pfr\pfr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\psaux\psaux.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\pshinter\pshinter.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\psnames\psmodule.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\raster\raster.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\sfnt\sfnt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\smooth\smooth.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\truetype\truetype.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\type1\type1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\cid\type1cid.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\type42\type42.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\winfonts\winfnt.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
