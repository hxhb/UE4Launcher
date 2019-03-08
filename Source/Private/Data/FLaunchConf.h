#pragma once

#include "UnrealString.h"
#include "Array.h"
#include "CoreMinimal.h"


struct FLaunchConf
{
	FString Engine;
	FString Platfrom;
	FString Project;
	FString Tool;
	TArray<FString> Params;
	bool bUseCmdEngine=false;
};