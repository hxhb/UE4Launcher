#pragma once

#include "UnrealString.h"
#include "Array.h"
#include "CoreMinimal.h"


struct FLaunchConf
{
	FString Engine;
	FString Project;
	FString Tool;
	FString ToolPreArgs;
	TArray<FString> Params;
};