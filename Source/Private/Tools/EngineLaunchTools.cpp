#include "Tools/EngineLaunchTools.h"


bool EngineLaunchTools::EngineLauncher(const FUELaunchConf& conf)
{
	if(!(conf.LaunchEngine.Len()>0 && FPaths::FileExists(GetEngineBinPath(conf))))
		return false;
	FPlatformProcess::CreateProc(*GetEngineBinPath(conf), *CombineLaunchParams(conf), true, false, false, NULL, NULL, NULL, NULL, NULL);
	return true;
}

FString EngineLaunchTools::GetEngineBinPath(const FUELaunchConf& conf)
{
#define EXECUTABLE_FORMAT TEXT(".exe")
	FString EngineProgramName = conf.bUseCmdEngine ? TEXT("UE4Editor-cmd") : TEXT("UE4Editor");
	FString resault(FPaths::Combine(*conf.LaunchEngine, TEXT("Engine/Binaries/"),*conf.LaunchPlatfrom,*(EngineProgramName + EXECUTABLE_FORMAT)));
#undef EXECUTABLE_FORMAT
	return resault;
}
FString EngineLaunchTools::CombineLaunchParams(const FUELaunchConf& conf)
{
	FString resault;

	resault.Append(conf.LaunchProject);
	resault.Append(TEXT(""));

	for(const auto& ParamItem:conf.LaunchParams)
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


void EngineLaunchTools::RegisterLaunchFileMaping()
{
	TCHAR* RegisterPath = TEXT("UE4Launcher.ProjectFile");
	FString ProgramName(TEXT("UE4Launcher.exe"));
	HKEY hRootKey;
	if (RegCreateKeyEx(HKEY_CLASSES_ROOT, RegisterPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hRootKey, NULL) == ERROR_SUCCESS)
	{
		FString NewIdentifier = FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensInBraces);
		LRESULT SetResult = RegSetValueEx(hRootKey, NULL, 0, REG_SZ, (const BYTE*)*ProgramName, (ProgramName.Len() + 1) * sizeof(TCHAR));
		RegCloseKey(hRootKey);
	}
}