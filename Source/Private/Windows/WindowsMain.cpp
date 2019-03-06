
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
	LPWSTR *ArgcList;
	int Argv=0;

	ArgcList = CommandLineToArgvW(GetCommandLineW(), &Argv);

	FString CommandLine;
	for (int index = 0; index < Argv; ++index)
	{
		CommandLine.Append(ArgcList[index]);
		CommandLine.Append(TEXT(" "));
	}
	// why FString(GetCommandLineW()) is error?
	// FString CommandLine(GetCommandLineW());

	int resault=RealExecutionMain(*CommandLine);

	return resault;
}
