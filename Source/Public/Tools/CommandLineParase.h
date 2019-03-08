#pragma once
#include "UnrealString.h"
#include "Array.h"
#include "Map.h"


class CommandLineParase
{
public:
	static TMap<FString,FString> GetCommandLineParamsMap(const FString& CommandLine);
};