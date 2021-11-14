#include "Tools/EngineLaunchTools.h"
#include "Tools/SerializationTools.h"
#include <shellapi.h>
#include "Misc/FileHelper.h"
#include <winreg.h>
#include "DesktopPlatformModule.h"
#include <windows.h>
#include "Templates/SharedPointer.h"

TArray<FString> EngineLaunchTools::OpenFileDialog()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

	TArray<FString> OpenFilenames;
	if (DesktopPlatform)
	{
		const bool bOpened = DesktopPlatform->OpenFileDialog(
			nullptr,
			TEXT("Open Files"),
			FString(TEXT("")),
			TEXT(""),
			TEXT("*.*"),
			EFileDialogFlags::None,
			OpenFilenames
		);
	}
	return OpenFilenames;
}

EngineBuildVersion EngineLaunchTools::GetBuildVersion(const FString& EngineRoot)
{
	EngineBuildVersion BuildVersion{};

	FString BuildVersionFile = FPaths::Combine(EngineRoot,TEXT("Engine/Build/Build.version"));
	FString jsonValue;
	bool flag = FFileHelper::LoadFileToString(jsonValue, *BuildVersionFile);
	if(flag)
	{
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(jsonValue);
		TSharedPtr<FJsonObject> JsonObject;
		if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
		{
			BuildVersion.MajorVersion = JsonObject->GetIntegerField(TEXT("MajorVersion"));
			BuildVersion.MinorVersion = JsonObject->GetIntegerField(TEXT("MinorVersion"));
			BuildVersion.PatchVersion = JsonObject->GetIntegerField(TEXT("PatchVersion"));
		}
	}
	return BuildVersion;
}

FString EngineLaunchTools::GetUnrealBuildToolBin(const FString& EngineRoot, const EngineBuildVersion& Version)
{
	FString result;
	if(Version.MajorVersion > 4)
	{
		result = FPaths::Combine(EngineRoot,TEXT("Engine/Binaries/DotNET/UnrealBuildTool/UnrealBuildTool.exe"));
	}
	if(Version.MajorVersion == 4)
	{
		result = FPaths::Combine(EngineRoot,TEXT("Engine/Binaries/DotNET/UnrealBuildTool.exe"));
	}

	return result;
}

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
		if (!ParamItem.IsEmpty())
		{
			resault.Append(" ");
			resault.Append(ParamItem);
		}
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

FString EngineLaunchTools::GetEnginePathFromIdentifier(const FString& EngineIdentifier)
{
	TMap<FString, FString> EngineMaps;
	EngineMaps = EngineLaunchTools::GetAllRegistedEngineMap();
	if (EngineMaps.Contains(EngineIdentifier))
		return *EngineMaps.Find(EngineIdentifier);
	else
		return TEXT("");
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
		FValueRegister{ HKEY_CLASSES_ROOT ,REG_SZ,TEXT("UE4Launcher.ProjectFile\\shell\\run\\command"),TEXT(""),ExecutableProgramPath + TEXT(" -c \"%1\""), true},
		// Unreal.ProjectFile
		FValueRegister{ HKEY_CLASSES_ROOT ,REG_SZ,TEXT("Unreal.ProjectFile\\shell\\genconf"),TEXT(""),TEXT("Genreate UE4Launcher Config") ,true},
		FValueRegister{ HKEY_CLASSES_ROOT ,REG_SZ,TEXT("Unreal.ProjectFile\\shell\\genconf"),TEXT("Icon"),ExecutableProgramPath},
		FValueRegister{ HKEY_CLASSES_ROOT ,REG_SZ,TEXT("Unreal.ProjectFile\\shell\\genconf\\command"),TEXT(""),ExecutableProgramPath + TEXT(" -g \"%1\""), true}
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

FString EngineLaunchTools::GetCurrentProgramDir()
{
	FString BinPATH = GetCurrentProgramFullName();
	FPaths::MakeStandardFilename(BinPATH);

	FString Path;
	{
		FString Filename;
		FString Extern;
		FPaths::Split(BinPATH, Path, Filename, Extern);
	}
	return Path;
}
TArray<FToolInfo> EngineLaunchTools::GetToolsInfoList()
{
	TArray<FToolInfo> Result;
	
	FString LaunchToolsConfPath = FPaths::Combine(EngineLaunchTools::GetCurrentProgramDir(), TEXT("LaunchTools.json"));

	// printf("%s", TCHAR_TO_ANSI(*LaunchToolsConfPath));
	if (FPaths::FileExists(LaunchToolsConfPath))
	{
		FString ConfigContent;
		FFileHelper::LoadFileToString(ConfigContent, *LaunchToolsConfPath);

		Result = SerializationTools::DeSerializeToolsByString(ConfigContent);
	}
	else
	{
		TArray<FToolInfo> DefaultTools{
			{ TEXT("Editor"), TEXT(""), TEXT("Engine/Binaries/Win64") },
			{ TEXT("Editor-cmd"),TEXT(""),TEXT("Engine/Binaries/Win64") },
			{ TEXT("UnrealFrontend"),TEXT(""),TEXT("Engine/Binaries/Win64") },
			{ TEXT("IPhonePackager"),TEXT(""),TEXT("Engine/Binaries/DotNET/IOS") },
			{ TEXT("NetworkProfiler"),TEXT(""),TEXT("Engine/Binaries/DotNET") }
		};
		FString DefaultToolsContent = SerializationTools::SerializeToolsAsString(DefaultTools);
		FFileHelper::SaveStringToFile(DefaultToolsContent, *LaunchToolsConfPath);

		for (const auto& Tool : DefaultTools)
		{
			Result.Add(Tool);
		}
	}
	return Result;	
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
	if (ToolInfo.ToolName.Equals(TEXT("Editor"),ESearchCase::IgnoreCase) || ToolInfo.ToolName.Equals(TEXT("Editor-cmd"), ESearchCase::IgnoreCase))
	{
		resault = FPaths::Combine(conf.Engine, ToolInfo.BinPath, TEXT("UE4")+ToolInfo.ToolName + PLATFROM_EXECUTABLE_FORMAT);
		if (!FPaths::FileExists(resault))
		{
			resault = FPaths::Combine(conf.Engine, ToolInfo.BinPath, TEXT("Unreal") + ToolInfo.ToolName + PLATFROM_EXECUTABLE_FORMAT);
		}
	}
	else
	{
		resault = FPaths::Combine(conf.Engine, ToolInfo.BinPath, ToolInfo.ToolName + PLATFROM_EXECUTABLE_FORMAT);
	}
	
#undef PLATFROM_EXECUTABLE_FORMAT
	return resault;
}


FString EngineLaunchTools::GetUEProjectEnginePath(const FString& upeojctFile)
{
	FString EngineVersionId;
	FDesktopPlatformModule::Get()->GetEngineIdentifierForProject(upeojctFile, EngineVersionId);

	return EngineLaunchTools::GetEnginePathFromIdentifier(EngineVersionId);
}

FString EngineLaunchTools::GetProjectDir(const FString& Project)
{
	FString ProjectPath;
	TArray<FString> OutArray;
	Project.ParseIntoArray(OutArray, TEXT("\\"));
	if (OutArray.Num() == 1 && OutArray[0] == Project)
	{
		Project.ParseIntoArray(OutArray, TEXT("/"));
	}
	for (const auto& item : OutArray)
	{
		if (FPaths::DirectoryExists(ProjectPath + item))
		{
			ProjectPath.Append(item);
			ProjectPath.Append(TEXT("\\"));
		}
	}
	return ProjectPath;
}


FString EngineLaunchTools::GetFileNameByFullDir(const FString& FullDir)
{
	FString resault;
	TArray<FString> OutArray;
	{
		FullDir.ParseIntoArray(OutArray, TEXT("/"));
		if (OutArray.Num() == 1 && OutArray.Last() == FullDir)
		{
			OutArray.Empty();
			FullDir.ParseIntoArray(OutArray, TEXT("\\"));
		}
		resault = OutArray.Last();
		OutArray.Empty();
		resault.ParseIntoArray(OutArray, TEXT("."));
		resault = OutArray[0];
	}
	return resault;
}