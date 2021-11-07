// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#ifndef APSTUDIO_INVOKED

#include <windows.h>
#include "../../../Runtime/Launch/Resources/Windows/resource.h"
#include "../../../Runtime/Launch/Resources/Version.h"

#define IDICON_UE4Launcher 2000
#define IDICON_UE4Config 2001
/////////////////////////////////////////////////////////////////////////////
//
// Version
//

VS_VERSION_INFO VERSIONINFO
 FILEVERSION ENGINE_MAJOR_VERSION,ENGINE_MINOR_VERSION,ENGINE_PATCH_VERSION,0
 PRODUCTVERSION ENGINE_MAJOR_VERSION,ENGINE_MINOR_VERSION,ENGINE_PATCH_VERSION,0
 FILEFLAGSMASK 0x17L
#ifdef _DEBUG
 FILEFLAGS 0x1L
#else
 FILEFLAGS 0x0L
#endif
 FILEOS 0x4L
 FILETYPE 0x2L
 FILESUBTYPE 0x0L
BEGIN
	BLOCK "StringFileInfo"
	BEGIN
		BLOCK "040904b0"
		BEGIN
			VALUE "CompanyName", "imzlp.me"
			VALUE "LegalCopyright", EPIC_COPYRIGHT_STRING
			VALUE "ProductName", "UE4 Launcher"
			VALUE "ProductVersion", ENGINE_VERSION_STRING
			VALUE "FileDescription", "UE4 Launcher"
			VALUE "InternalName", EPIC_PRODUCT_IDENTIFIER
			VALUE "OriginalFilename", "UE4Launcher.exe"
			VALUE "ShellIntegrationVersion", "2"
		END
	END
	BLOCK "VarFileInfo"
	BEGIN
		VALUE "Translation", 0x409, 1200
	END
END

#endif

/////////////////////////////////////////////////////////////////////////////
//
// Application Manifest
//
//
//#if _DEBUG
//
//// Debug manifest
//#if _WIN64
//1						RT_MANIFEST				"../../../Runtime/Launch/Resources/Windows/Debug-PCLaunch-Win64.manifest"
//#else
//1						RT_MANIFEST				"../../../Runtime/Launch/Resources/Windows/Debug-PCLaunch-Win32.manifest"
//#endif
//
//#else
//
//// Release manifest
//#if _WIN64
//1						RT_MANIFEST				"../../../Runtime/Launch/Resources/Windows/PCLaunch-Win64.manifest"
//#else
//1						RT_MANIFEST				"../../../Runtime/Launch/Resources/Windows/PCLaunch-Win32.manifest"
//#endif
//
//#endif

// Icon with lowest ID value placed first to ensure application icon
// remains consistent on all systems.
IDICON_UE4Launcher			ICON                    "Icon.ico"
IDICON_UE4Config		ICON                    "Icon_config.ico"