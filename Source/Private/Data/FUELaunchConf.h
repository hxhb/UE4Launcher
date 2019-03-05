#pragma once

#include "UnrealString.h"
#include "Array.h"
#include "CoreMinimal.h"


struct FUELaunchConf
{

	FString LaunchEngine;
	FString LaunchPlatfrom;
	FString LaunchProject;
	TArray<FString> LaunchParams;
	bool bUseCmdEngine;
};