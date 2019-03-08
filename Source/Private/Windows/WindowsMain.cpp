
// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "RealExecutionMain.h"
#include <windows.h>
#include <shellapi.h>

int WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	return RealExecutionMain(GetCommandLineW());
}
