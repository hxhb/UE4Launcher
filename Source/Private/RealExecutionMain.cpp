
#include "RealExecutionMain.h"
#include "LaunchEngineLoop.h"
#include "StandaloneRenderer.h"
#include "Framework/Application/SlateApplication.h"
#include "DesktopPlatformModule.h"
#include "ISlateReflectorModule.h"
#include "SWebBrowser.h"
#include "IWebBrowserWindow.h"
#include "IWebBrowserPopupFeatures.h"

// project files
#include "SlateWidget/SConfPanel.h"
#include "Data/FLaunchConf.h"
#include "Tools/EngineLaunchTools.h"
#include "Tools/SerializationTools.h"
#include "Tools/CommandLineParase.h"

#define LOCTEXT_NAMESPACE "UE4Launcher"

IMPLEMENT_APPLICATION(UE4Launcher, "UE4Launcher");

int RealExecutionMain(const TCHAR* pCmdLine)
{
	// add .uejson file mapping for administrator user
	EngineLaunchTools::UE4LauncherRegisterWriter();

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

	TMap<FString, FString> CommandArgsMaps = CommandLineParase::GetCommandLineParamsMap(FCommandLine::Get());
	FLaunchConf DefaultConfig;
	FString CommandLaunchArg = CommandArgsMaps.Contains(TEXT("e")) ? TEXT("e") :
							CommandArgsMaps.Contains(TEXT("c")) ? TEXT("c") : TEXT("");

	// has config params?
	if (!CommandLaunchArg.IsEmpty() && CommandArgsMaps.Contains(CommandLaunchArg))
	{	
		FString jsonFile = *CommandArgsMaps.Find(CommandLaunchArg);
		FString jsonValue(TEXT(""));
		FString JsonfullPath = FPaths::ConvertRelativePathToFull(jsonFile);
		if (FFileHelper::LoadFileToString(jsonValue, *JsonfullPath))
		{
			DefaultConfig = SerializationTools::DeSerializationConf(jsonValue);
		}

	}

	// has -c?
	if (!CommandLaunchArg.IsEmpty() && CommandLaunchArg.Equals(TEXT("c")))
	{
		EngineLaunchTools::EngineLauncher(DefaultConfig);
	}
	else
	{
		TSharedPtr < SConfPanel > LauncherPanel;
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
											SAssignNew(LauncherPanel, SConfPanel)
										];

		// show the window
		FSlateApplication::Get().AddWindow(MainWindow.ToSharedRef());

		// use config
		LauncherPanel->UpdateAll(DefaultConfig);

		// main loop
		while (!GIsRequestingExit)
		{
			FTaskGraphInterface::Get().ProcessThreadUntilIdle(ENamedThreads::GameThread);
			FStats::AdvanceFrame(false);
			FTicker::GetCoreTicker().Tick(FApp::GetDeltaTime());
			FSlateApplication::Get().PumpMessages();
			FSlateApplication::Get().Tick();
		}
	}

	FModuleManager::Get().UnloadModulesAtShutdown();
	FSlateApplication::Shutdown();

	return 0;
}


#undef LOCTEXT_NAMESPACE