#pragma once

#include "Array.h"
#include "Data/FLaunchConf.h"


class EngineLaunchTools
{
public:
	static bool EngineLauncher(const FLaunchConf& conf);
	static FString CombineLaunchParams(const FLaunchConf& conf);
	static FString GetEngineBinPath(const FLaunchConf& conf);
	static TMap<FString, FString> GetAllRegistedEngineMap();
	static TArray<FString> GetAllRegistedEngineList(const TMap<FString, FString>& pEngineMap);
	static FString GetCurrentWorkDirectory();
	static void RegisterValueWriter(HKEY hKey, DWORD dwType, const FString& lpSubKey, const FString& lpValueName, const FString& lpData, bool SetAsDefaultValue);
	static void UE4LauncherRegisterWriter();
};