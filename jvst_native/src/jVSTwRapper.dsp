# Microsoft Developer Studio Project File - Name="jVSTwRapper" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **
 
# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=jVSTwRapper - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "jVSTwRapper.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "jVSTwRapper.mak" CFG="jVSTwRapper - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "jVSTwRapper - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "jVSTwRapper - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "jVSTwRapper - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JVSTWRAPPER_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JVSTWRAPPER_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 odbc32.lib odbccp32.lib kernel32.lib comdlg32.lib shell32.lib user32.lib gdi32.lib winspool.lib advapi32.lib ole32.lib oleaut32.lib uuid.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "jVSTwRapper - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JVSTWRAPPER_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JVSTWRAPPER_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"C:\Program Files\Tracktion\plugins\3rd Party\jVSTwRapper\jVSTwRapper.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "jVSTwRapper - Win32 Release"
# Name "jVSTwRapper - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\ConfigFileReader.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ConfigFileReader.h
# End Source File
# Begin Source File

SOURCE=.\src\JNIUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\src\JNIUtils.h
# End Source File
# Begin Source File

SOURCE=.\src\jVSTwRapperMAIN.cpp
# End Source File
# Begin Source File

SOURCE=.\src\jVSTwRapperMAIN.h
# End Source File
# Begin Source File

SOURCE=.\src\VSTGUIWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VSTGUIWrapper.h
# End Source File
# Begin Source File

SOURCE=.\src\VSTV10ToHost.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VSTV10ToHost.h
# End Source File
# Begin Source File

SOURCE=.\src\VSTV10ToPlug.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VSTV10ToPlug.h
# End Source File
# Begin Source File

SOURCE=.\src\VSTV20ToHost.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VSTV20ToHost.h
# End Source File
# Begin Source File

SOURCE=.\src\VSTV20ToPlug.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VSTV20ToPlug.h
# End Source File
# Begin Source File

SOURCE=.\src\VSTV21ToHost.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VSTV21ToHost.h
# End Source File
# Begin Source File

SOURCE=.\src\VSTV21ToPlug.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VSTV21ToPlug.h
# End Source File
# Begin Source File

SOURCE=.\src\VSTV22ToHost.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VSTV22ToHost.h
# End Source File
# Begin Source File

SOURCE=.\src\VSTV22ToPlug.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VSTV22ToPlug.h
# End Source File
# Begin Source File

SOURCE=.\src\VSTV23ToHost.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VSTV23ToHost.h
# End Source File
# Begin Source File

SOURCE=.\src\VSTV23ToPlug.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VSTV23ToPlug.h
# End Source File
# End Group
# Begin Group "Config Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\jVSTwRapper.def
# End Source File
# Begin Source File

SOURCE=.\src\conf\jVSTwRapper.ini
# End Source File
# End Group
# Begin Group "VST Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\vst\AEffect.h
# End Source File
# Begin Source File

SOURCE=.\src\vst\aeffectx.h
# End Source File
# Begin Source File

SOURCE=.\src\vst\AudioEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\src\vst\audioeffectx.cpp
# End Source File
# Begin Source File

SOURCE=.\src\vst\audioeffectx.h
# End Source File
# Begin Source File

SOURCE=.\src\vst\vstplugsmacho.h
# End Source File
# End Group
# Begin Group "VSTGUI Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\vst\aeffguieditor.cpp
# End Source File
# Begin Source File

SOURCE=.\src\vst\aeffguieditor.h
# End Source File
# Begin Source File

SOURCE=.\src\vst\vstcontrols.cpp
# End Source File
# Begin Source File

SOURCE=.\src\vst\vstcontrols.h
# End Source File
# Begin Source File

SOURCE=.\src\vst\vstgui.cpp
# End Source File
# Begin Source File

SOURCE=.\src\vst\vstgui.h
# End Source File
# End Group
# End Target
# End Project
