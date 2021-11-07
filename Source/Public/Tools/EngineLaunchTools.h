#pragma once

#include "Containers/Array.h"
#include "Data/FLaunchConf.h"
#include <winreg.h>
#include "UnrealString.h"
struct FToolInfo
{
	FToolInfo() = default;
	inline FToolInfo(const FString& pToolName, const FString& pPreArgs, const FString& pBinPath)
		:ToolName(pToolName), PreArgs(pPreArgs), BinPath(pBinPath) {}
	FString ToolName;
	FString PreArgs;
	FString BinPath;
};

class EngineLaunchTools
{
public:
	static bool EngineLauncher(const FLaunchConf& conf);
	static FString CombineLaunchParams(const FLaunchConf& conf);
	//static FString GetEngineBinPath(const FLaunchConf& conf);
	static TMap<FString, FString> GetAllRegistedEngineMap();
	static TArray<FString> GetAllRegistedEngineList(const TMap<FString, FString>& pEngineMap);
	static void RegisterValueWriter(HKEY hKey, DWORD dwType, const FString& lpSubKey, const FString& lpValueName, const FString& lpData, bool SetAsDefaultValue);
	static void UE4LauncherRegisterWriter();
	static FString GetEnginePathFromIdentifier(const FString& EngineIdentifier);
	// Tool Info
	static TArray<FToolInfo> GetToolsInfoList();
	static TArray<FString> GetToolList();
	static FToolInfo GetToolInfo(const FString& ToolName);
	static FString GetToolBinPath(const FLaunchConf& Tool);

	static FString GetCurrentProgramFullName();
	static FString GetCurrentWorkDirectory();

	static FString GetUEProjectEnginePath(const FString& upeojctFile);
	static FString GetProjectDir(const FString& Project);
	static FString GetFileNameByFullDir(const FString& FullDir);
	
};