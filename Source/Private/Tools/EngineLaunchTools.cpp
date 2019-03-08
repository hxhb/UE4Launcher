#include "Tools/EngineLaunchTools.h"


bool EngineLaunchTools::EngineLauncher(const FLaunchConf& conf)
{
	if(!(conf.Engine.Len()>0 && FPaths::FileExists(GetEngineBinPath(conf))))
		return false;
	FPlatformProcess::CreateProc(*GetEngineBinPath(conf), *CombineLaunchParams(conf), true, false, false, NULL, NULL, NULL, NULL, NULL);
	return true;
}

FString EngineLaunchTools::GetEngineBinPath(const FLaunchConf& conf)
{
#define EXECUTABLE_FORMAT TEXT(".exe")
	FString EngineProgramName = conf.bUseCmdEngine ? TEXT("UE4Editor-cmd") : TEXT("UE4Editor");
	FString resault(FPaths::Combine(*conf.Engine, TEXT("Engine/Binaries/"),*conf.Platfrom,*(EngineProgramName + EXECUTABLE_FORMAT)));
#undef EXECUTABLE_FORMAT
	return resault;
}
FString EngineLaunchTools::CombineLaunchParams(const FLaunchConf& conf)
{
	FString resault;

	resault.Append(conf.Project);
	resault.Append(TEXT(""));

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
	TArray<FValueRegister> WaitWrite{
		FValueRegister{ HKEY_CLASSES_ROOT ,REG_SZ,TEXT(".uejson"),TEXT(""),TEXT("UE4Launcher.ProjectFile"),true },
		FValueRegister{ HKEY_CLASSES_ROOT ,REG_SZ,TEXT("UE4Launcher.ProjectFile\\DefaultIcon"),TEXT(""),TEXT("\"C:\\Program Files (x86)\\UE4Launcher\\Engine\\Binaries\\Win64\\UE4Launcher.exe\",1"),true },
		FValueRegister{ HKEY_CLASSES_ROOT ,REG_SZ,TEXT("UE4Launcher.ProjectFile\\shell\\edit"),TEXT(""),TEXT("Edit Config") ,true},
		FValueRegister{ HKEY_CLASSES_ROOT ,REG_SZ,TEXT("UE4Launcher.ProjectFile\\shell\\edit"),TEXT("Icon"),TEXT("\"C:\\Program Files (x86)\\UE4Launcher\\Engine\\Binaries\\Win64\\UE4Launcher.exe\",1")},
		FValueRegister{ HKEY_CLASSES_ROOT ,REG_SZ,TEXT("UE4Launcher.ProjectFile\\shell\\edit\\command"),TEXT(""),TEXT("\"C:\\Program Files (x86)\\UE4Launcher\\Engine\\Binaries\\Win64\\UE4Launcher.exe\" -e \"%1\"") ,true},
		FValueRegister{ HKEY_CLASSES_ROOT ,REG_SZ,TEXT("UE4Launcher.ProjectFile\\shell\\run"),TEXT(""),TEXT("Launch Config") ,true},
		FValueRegister{ HKEY_CLASSES_ROOT ,REG_SZ,TEXT("UE4Launcher.ProjectFile\\shell\\run"),TEXT("Icon"),TEXT("\"C:\\Program Files (x86)\\UE4Launcher\\Engine\\Binaries\\Win64\\UE4Launcher.exe\"")},
		FValueRegister{ HKEY_CLASSES_ROOT ,REG_SZ,TEXT("UE4Launcher.ProjectFile\\shell\\run\\command"),TEXT(""),TEXT("\"C:\\Program Files (x86)\\UE4Launcher\\Engine\\Binaries\\Win64\\UE4Launcher.exe\" -c \"%1\""), true}
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


FString EngineLaunchTools::GetCurrentWorkDirectory()
{
#define MAX_NUM 1024
	WCHAR CurrentDir[MAX_NUM];
	GetCurrentDirectory(MAX_NUM - 1, CurrentDir);
#undef  MAX_NUM

	return FString(CurrentDir);
}