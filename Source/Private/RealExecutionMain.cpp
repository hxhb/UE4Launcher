
#include "RealExecutionMain.h"
#include "DesktopPlatformModule.h"
#include "ISlateReflectorModule.h"
#include "SWebBrowser.h"
#include "Framework/Application/SlateApplication.h"
#include "IWebBrowserWindow.h"
#include "IWebBrowserPopupFeatures.h"
#include "UI/WidgetUELauncher.h"
#include "Data/FUELaunchConf.h"
#include "Tools/EngineLaunchTools.h"
#include "Tools/SerializationTools.h"
#include <iostream>
#include <direct.h> 

#define LOCTEXT_NAMESPACE "UE4Launcher"

IMPLEMENT_APPLICATION(UE4Launcher, "UE4Launcher");

#define MAX_PATH_BUFFER 512
TSharedRef<SDockTab> SpawnUELauncherTab(const FSpawnTabArgs& Args)
{
	TSharedRef<SDockTab> UELauncherTab = SNew(SDockTab)
		.Label(LOCTEXT("UE4LauncherTab", "UE4 Launcher"))
		.ToolTipText(LOCTEXT("UE4LauncherTabToolTip", "Launch Engine or Project."))
		.TabRole(ETabRole::NomadTab)
		.Clipping(EWidgetClipping::ClipToBounds);
	UELauncherTab->SetContent
	(
		MakeWidgetUELauncher()
	);
	return UELauncherTab;
}

int RealExecutionMain(const TCHAR* pCmdLine)
{
	FCommandLine::Set(pCmdLine);

	// start up the main loop
	GEngineLoop.PreInit(pCmdLine);

	// Make sure all UObject classes are registered and default properties have been initialized
	ProcessNewlyLoadedUObjects();

	// Tell the module manager it may now process newly-loaded UObjects when new C++ modules are loaded
	FModuleManager::Get().StartProcessingNewlyLoadedObjects();

	// crank up a normal Slate application using the platform's standalone renderer
	FSlateApplication::InitializeAsStandaloneApplication(GetStandardStandaloneRenderer());

	// set the application name
	FGlobalTabmanager::Get()->SetApplicationTitle(LOCTEXT("AppTitle", "UE4 Launcher"));

	FModuleManager::LoadModuleChecked<ISlateReflectorModule>("SlateReflector").RegisterTabSpawner(FWorkspaceItem::NewGroup(LOCTEXT("DeveloperMenu", "Developer")));

	//// Registe WebBrowserTab
	// RegisterNomadTabSpawner is just one tab on window
	// RegisterTabSpawner can have mulit table on window
	//FGlobalTabmanager::Get()->RegisterTabSpawner("UE4LauncherTab", FOnSpawnTab::CreateStatic(&SpawnUELauncherTab))
	//	.SetDisplayName(LOCTEXT("UE4LauncherTab", "UE4 Launcher"));

	//TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("SlateUE4Launcher_Layout")
	//	->AddArea
	//	(
	//		FTabManager::NewArea(500, 500)
	//		->SetWindow(FVector2D(500, 500), false)
	//		->Split
	//		(
	//			FTabManager::NewStack()
	//			->AddTab("UE4LauncherTab", ETabState::OpenedTab)
	//		)
	//	);

	//FGlobalTabmanager::Get()->RestoreFrom(Layout, TSharedPtr<SWindow>());
	FString jsonFile;
	bool bUseCmdLine = false;
	{
		if (FParse::Value(FCommandLine::Get(), TEXT("-c"), jsonFile) && !jsonFile.IsEmpty())
		{
			TCHAR PathBuffer[MAX_PATH_BUFFER];
			_wgetcwd(PathBuffer, MAX_PATH_BUFFER - 1);
			FString runTimePath(PathBuffer);
			//FString JsonfullPath=FPaths::Combine(_wgetcwd(PathBuffer,MAX_PATH_BUFFER-1), FPaths::ConvertRelativePathToFull(jsonFile));
			FString jsonValue;
			FString JsonfullPath = FPaths::ConvertRelativePathToFull(jsonFile);
			if (FFileHelper::LoadFileToString(jsonValue, *JsonfullPath))
			{
				FUELaunchConf conf = SerializationTools::DeSerializationConf(jsonValue);
				EngineLaunchTools::EngineLauncher(conf);
				bUseCmdLine = true;
			}
			std::wcout << *jsonFile << std::endl;
			std::wcout<<*JsonfullPath<<std::endl;
			std::wcout << *runTimePath << std::endl;
		}
	}
	TSharedPtr<SWidgetUELauncher> pLauncherWidget;
	{	
		if (!bUseCmdLine)
		{
			TSharedPtr<SWindow> MainWindow = SNew(SWindow)
				.Title(LOCTEXT("MainWindow_Title", "UE4 Launcher"))
				.ScreenPosition(FVector2D(520, 550))
				.ClientSize(FVector2D(520, 550))
				.SupportsMaximize(false)
				.AutoCenter(EAutoCenter::PrimaryWorkArea)
				.MaxHeight(800)
				.MaxWidth(650)
				.MinHeight(580)
				.MinWidth(520)
				.IsTopmostWindow(false)
				[
					//MakeWidgetUELauncher()
					SAssignNew(pLauncherWidget, SWidgetUELauncher)
				];

			FSlateApplication::Get().AddWindow(MainWindow.ToSharedRef());
		}
		if (FParse::Value(FCommandLine::Get(), TEXT("-e"), jsonFile) && !jsonFile.IsEmpty())
		{
			FString jsonValue;
			FString JsonfullPath = FPaths::ConvertRelativePathToFull(jsonFile);
			std::wcout << *jsonFile << std::endl;
			std::wcout << *JsonfullPath << std::endl;
			if (FFileHelper::LoadFileToString(jsonValue, *JsonfullPath))
			{
				FUELaunchConf conf = SerializationTools::DeSerializationConf(jsonValue);
				pLauncherWidget->UpdateAll(conf);
			}	
		}
	}
	while (!GIsRequestingExit)
	{
		FTaskGraphInterface::Get().ProcessThreadUntilIdle(ENamedThreads::GameThread);
		FStats::AdvanceFrame(false);
		FTicker::GetCoreTicker().Tick(FApp::GetDeltaTime());
		FSlateApplication::Get().PumpMessages();
		FSlateApplication::Get().Tick();
	}
	FModuleManager::Get().UnloadModulesAtShutdown();
	FSlateApplication::Shutdown();

	return 0;
}


#undef LOCTEXT_NAMESPACE