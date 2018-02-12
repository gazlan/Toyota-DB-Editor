# Microsoft Developer Studio Project File - Name="ToyotaDB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ToyotaDB - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ToyotaDB.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ToyotaDB.mak" CFG="ToyotaDB - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ToyotaDB - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ToyotaDB - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ToyotaDB - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /I "..\XListCtrl" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS. XLISTCTRLLIB_STATIC" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "ToyotaDB - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\XListCtrl" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ToyotaDB - Win32 Release"
# Name "ToyotaDB - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ATL\build_.h
# End Source File
# Begin Source File

SOURCE=..\Shared\db_dbx.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\file.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\file_walker.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\mmf.cpp
# End Source File
# Begin Source File

SOURCE=..\shared\prng_int.cpp
# End Source File
# Begin Source File

SOURCE=.\ATL\resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ATL\stdafx.h
# End Source File
# Begin Source File

SOURCE=..\Shared\text.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\timestamp.cpp
# End Source File
# Begin Source File

SOURCE=.\ToyotaDB.cpp
# End Source File
# Begin Source File

SOURCE=.\ToyotaDB.rc
# End Source File
# Begin Source File

SOURCE=.\ToyotaDBDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\tray.cpp
# End Source File
# Begin Source File

SOURCE=.\Twins.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\vector.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\vector_sorted.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\xlat.cpp
# End Source File
# Begin Source File

SOURCE=..\Shared\xlat_tables.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Shared\db_dbx.h
# End Source File
# Begin Source File

SOURCE=..\Shared\file.h
# End Source File
# Begin Source File

SOURCE=..\Shared\file_walker.h
# End Source File
# Begin Source File

SOURCE=..\Shared\mmf.h
# End Source File
# Begin Source File

SOURCE=..\shared\prng_int.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Shared\text.h
# End Source File
# Begin Source File

SOURCE=..\Shared\timestamp.h
# End Source File
# Begin Source File

SOURCE=.\ToyotaDB.h
# End Source File
# Begin Source File

SOURCE=.\ToyotaDBDlg.h
# End Source File
# Begin Source File

SOURCE=..\Shared\tray.h
# End Source File
# Begin Source File

SOURCE=.\Twins.h
# End Source File
# Begin Source File

SOURCE=..\Shared\vector.h
# End Source File
# Begin Source File

SOURCE=..\Shared\vector_sorted.h
# End Source File
# Begin Source File

SOURCE=..\Shared\xlat.h
# End Source File
# Begin Source File

SOURCE=..\Shared\xlat_tables.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\dn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\empty.ico
# End Source File
# Begin Source File

SOURCE=.\res\lt.bmp
# End Source File
# Begin Source File

SOURCE=.\res\lt.ico
# End Source File
# Begin Source File

SOURCE=.\res\rt.bmp
# End Source File
# Begin Source File

SOURCE=.\res\rt.ico
# End Source File
# Begin Source File

SOURCE=.\res\ToyotaDB.ico
# End Source File
# Begin Source File

SOURCE=.\res\ToyotaDB.rc2
# End Source File
# Begin Source File

SOURCE=.\res\up.bmp
# End Source File
# End Group
# End Target
# End Project
