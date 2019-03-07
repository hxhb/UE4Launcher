#pragma once

#include "Array.h"

#include "Data/FUELaunchConf.h"


class EngineLaunchTools
{
public:
	static bool EngineLauncher(const FUELaunchConf& conf);
	static FString CombineLaunchParams(const FUELaunchConf& conf);
	static FString GetEngineBinPath(const FUELaunchConf& conf);
	static TMap<FString, FString> GetAllRegistedEngineMap();
	static TArray<FString> GetAllRegistedEngineList(const TMap<FString, FString>& pEngineMap);
	static void RegisterLaunchFileMaping();
};