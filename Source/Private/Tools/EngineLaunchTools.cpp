#include "Tools/EngineLaunchTools.h"
#include <shellapi.h>

bool EngineLaunchTools::EngineLauncher(const FLaunchConf& conf)
{
	FString BinPath = EngineLaunchTools::GetToolBinPath(conf);
	if(!(conf.Engine.Len()>0 && FPaths::FileExists(BinPath)))
		return false;
	FString AllParams = CombineLaunchParams(conf);
	FPlatformProcess::CreateProc(*BinPath, *AllParams, true, false, false, NULL, NULL, NULL, NULL, NULL);
	return true;
}

//FString EngineLaunchTools::GetEngineBinPath(const FLaunchConf& conf)
//{
//#define EXECUTABLE_FORMAT TEXT(".exe")
//	FString resault(FPaths::Combine(*conf.Engine, TEXT("Engine/Binaries/"),*conf.Platfrom,*(conf.Tool + EXECUTABLE_FORMAT)));
//#undef EXECUTABLE_FORMAT
//	return resault;
//}
FString EngineLaunchTools::CombineLaunchParams(const FLaunchConf& conf)
{
	FString resault(TEXT(""));
	if (!conf.ToolPreArgs.IsEmpty())
		resault.Append(conf.ToolPreArgs);
	resault.Append(TEXT("\"")).Append(conf.Project).Append(TEXT("\" "));

	for(const auto& ParamItem:conf.Params)
	{
		resault.Append(" ");
		resault.Append(ParamItem);
	}
	return resault;
}

TMap<FString, FString> EngineLaunchTools::GetAllRegistedEngineMap()
{
	TMap<FString, FString> resault;
	FDesktopPlatformModule::Get()->EnumerateEngineInstallations(resault);

	return resault;
}
TArray<FString> EngineLaunchTools::GetAllRegistedEngineList(const TMap<FString, FString>& pEngineMap)
{
	TArray<FString> resault;
	TArray<FString> EngineAllKey;
	pEngineMap.GetKeys(EngineAllKey);

	for (FString& Key : EngineAllKey)
	{
		if (pEngineMap.Find(Key))
		{
			resault.Add(*pEngineMap.Find(Key));
		}
	}
	return resault;
}


void EngineLaunchTools::RegisterValueWriter(HKEY hKey, DWORD dwType,const FString& lpSubKey, const FString& lpValueName,const FString& lpData,bool SetAsDefaultValue)
{
	HKEY hRootKey;
	if (RegCreateKeyEx(hKey, *lpSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hRootKey, NULL) == ERROR_SUCCESS)
	{
		if (SetAsDefaultValue)
			RegSetValue(hKey, *lpSubKey, dwType, *lpData, (lpData.Len() + 1) * sizeof(TCHAR));
		else
			RegSetValueEx(hRootKey, *lpValueName, 0, dwType, (const BYTE*)*lpData, (lpData.Len() + 1) * sizeof(TCHAR));
		RegCloseKey(hRootKey);
	}
}
void EngineLaunchTools::UE4LauncherRegisterWriter()
{

	struct FValueRegister
	{
		FValueRegister(HKEY Key, DWORD pdwType,const FString& pSubKey, const FString& pValueName, const FString& pData,bool pIsDefaultValue=false):
		hKey(Key), dwType(pdwType),lpSubKey(pSubKey),lpValueName(pValueName),lpData(pData), lpIsDefaultValue(pIsDefaultValue){}
		HKEY hKey;
		DWORD dwType;
		FString lpSubKey;
		FString lpValueName;
		FString lpData;
		bool lpIsDefaultValue;
	};
	FString ExecutableProgramPath = TEXT("\"")+EngineLaunchTools::GetCurrentProgramFullName()+ TEXT("\"");
	TArray<FValueRegister> WaitWrite{
		FValueRegister{ HKEY_CLASSES_ROOT ,REG_SZ,TEXT(".uejson"),TEXT(""),TEXT("UE4Launcher.ProjectFile"),true },
		FValueRegister{ HKEY_CLASSES_ROOT ,REG_SZ,TEXT("UE4Launcher.ProjectFile\\DefaultIcon"),TEXT(""),ExecutableProgramPath+TEXT(",1"),true },
		FValueRegister{ HKEY_CLASSES_ROOT ,REG_SZ,TEXT("UE4Launcher.ProjectFile\\shell\\edit"),TEXT(""),TEXT("Edit Config") ,true},
		FValueRegister{ HKEY_CLASSES_ROOT ,REG_SZ,TEXT("UE4Launcher.ProjectFile\\shell\\edit"),TEXT("Icon"),ExecutableProgramPath + TEXT(",1")},
		FValueRegister{ HKEY_CLASSES_ROOT ,REG_SZ,TEXT("UE4Launcher.ProjectFile\\shell\\edit\\command"),TEXT(""),ExecutableProgramPath + TEXT(" -e \"%1\"") ,true},
		FValueRegister{ HKEY_CLASSES_ROOT ,REG_SZ,TEXT("UE4Launcher.ProjectFile\\shell\\run"),TEXT(""),TEXT("Launch Config") ,true},
		FValueRegister{ HKEY_CLASSES_ROOT ,REG_SZ,TEXT("UE4Launcher.ProjectFile\\shell\\run"),TEXT("Icon"),ExecutableProgramPath},
		FValueRegister{ HKEY_CLASSES_ROOT ,REG_SZ,TEXT("UE4Launcher.ProjectFile\\shell\\run\\command"),TEXT(""),ExecutableProgramPath+TEXT(" -c \"%1\""), true}
	};
	for (const auto& RegisterItem : WaitWrite)
	{
		EngineLaunchTools::RegisterValueWriter(RegisterItem.hKey,
											   RegisterItem.dwType,
											   RegisterItem.lpSubKey,
											   RegisterItem.lpValueName,
											   RegisterItem.lpData,
											   RegisterItem.lpIsDefaultValue);
	}
}

FString EngineLaunchTools::GetCurrentProgramFullName()
{
	LPWSTR *ArgcList;
	int Argv = 0;
	ArgcList = CommandLineToArgvW(GetCommandLineW(), &Argv);
	return FString(ArgcList[0]);
}

FString EngineLaunchTools::GetCurrentWorkDirectory()
{
#define MAX_NUM 1024
	WCHAR CurrentDir[MAX_NUM];
	GetCurrentDirectory(MAX_NUM - 1, CurrentDir);
#undef  MAX_NUM

	return FString(CurrentDir);
}


TArray<FToolInfo> EngineLaunchTools::GetToolsInfoList()
{
	return TArray<FToolInfo>{
		{TEXT("UE4Editor"), TEXT(""), TEXT("Engine/Binaries/Win64")},
		{ TEXT("UE4Editor-cmd"),TEXT(""),TEXT("Engine/Binaries/Win64") }
	};
}

TArray<FString> EngineLaunchTools::GetToolList()
{
	TArray<FString> resault;
	for (const auto& ToolItem : EngineLaunchTools::GetToolsInfoList())
	{
		resault.Add(ToolItem.ToolName);
	}
	return resault;
}

FToolInfo EngineLaunchTools::GetToolInfo(const FString& ToolName)
{
	for (const auto& ToolItem : EngineLaunchTools::GetToolsInfoList())
	{
		if (ToolItem.ToolName.Equals(ToolName))
		{
			return ToolItem;
		}
	}
	return FToolInfo{};
}

FString EngineLaunchTools::GetToolBinPath(const FLaunchConf& conf)
{
#define PLATFROM_EXECUTABLE_FORMAT TEXT(".exe")
	FString resault(TEXT(""));
	FToolInfo ToolInfo = EngineLaunchTools::GetToolInfo(conf.Tool);
	resault = FPaths::Combine(conf.Engine, ToolInfo.BinPath, ToolInfo.ToolName + PLATFROM_EXECUTABLE_FORMAT);
#undef PLATFROM_EXECUTABLE_FORMAT
	return resault;
}