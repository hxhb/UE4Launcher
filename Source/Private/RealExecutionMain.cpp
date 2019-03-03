
#include "RealExecutionMain.h"
#include "DesktopPlatformModule.h"
#include "ISlateReflectorModule.h"
#include "SWebBrowser.h"
#include "Framework/Application/SlateApplication.h"
#include "IWebBrowserWindow.h"
#include "IWebBrowserPopupFeatures.h"
#include "UI/WidgetUELauncher.h"

#define LOCTEXT_NAMESPACE "UE4Launcher"

IMPLEMENT_APPLICATION(UE4Launcher, "UE4Launcher");

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
	FCommandLine::Set(TEXT(""));

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

	// RegisterNomadTabSpawner is just one tab on window
	// RegisterTabSpawner can have mulit table on window
	FModuleManager::LoadModuleChecked<ISlateReflectorModule>("SlateReflector").RegisterTabSpawner(FWorkspaceItem::NewGroup(LOCTEXT("DeveloperMenu", "Developer")));

	//// Registe WebBrowserTab
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

	TSharedPtr<SWindow> MainWindow = SNew(SWindow)
		.Title(LOCTEXT("MainWindow_Title", "UE4 Launcher"))
		.ScreenPosition(FVector2D(520, 550))
		.ClientSize(FVector2D(520, 550))
		.SupportsMaximize(false)
		.AutoCenter(EAutoCenter::PrimaryWorkArea)
		.MaxHeight(800)
		.MaxWidth(800)
		.MinHeight(580)
		.MinWidth(520)
		[
			MakeWidgetUELauncher()
		];

	FSlateApplication::Get().AddWindow(MainWindow.ToSharedRef());


	while (!GIsRequestingExit)
	{
		FTaskGraphInterface::Get().ProcessThreadUntilIdle(ENamedThreads::GameThread);
		FStats::AdvanceFrame(false);
		FTicker::GetCoreTicker().Tick(FApp::GetDeltaTime());
		FSlateApplication::Get().PumpMessages();
		FSlateApplication::Get().Tick();
		FPlatformProcess::Sleep(0);
	}
	FModuleManager::Get().UnloadModulesAtShutdown();
	FSlateApplication::Shutdown();

	return 0;
}


#undef LOCTEXT_NAMESPACE