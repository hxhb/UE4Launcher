//// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
//
#include "SlateWidget/SConfPanel.h"

#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SBoxPanel.h"
#include "SlateOptMacros.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SHeader.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Text/SMultiLineEditableText.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Input/SHyperlink.h"
#include "Paths.h"
#include "IDesktopPlatform.h"
#include "DesktopPlatformModule.h"

// project files
#include "SlateWidget/SEditableBoxWraper.h"
#include "SlateWidget/SCombBoxWarper.h"

#include "Tools/SerializationTools.h"
#include "Tools/EngineLaunchTools.h"
#include "Tools/SlateWidgetTools.h"

#define LOCTEXT_NAMESPACE "WidgetUELauncher"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SConfPanel::Construct(const FArguments& InArgs)
{
	OnOpenedFileEvent = InArgs._OnOpenedFileEvent;
	ChildSlot
	[
			SNew(SBorder)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				[
					SAssignNew(SrbWidgetMain, SScrollBox)
					+ SScrollBox::Slot()
					.Padding(5.0f)
					[
						SNew(SGridPanel)
						.FillColumn(0, 1.0f)
						+ SGridPanel::Slot(0, 0)
							.HAlign(HAlign_Fill)
							.Padding(0.0f, 1.0f, 0.0f, 1.0f)
							[
								SNew(SVerticalBox)
								+SVerticalBox::Slot()
								.AutoHeight()
								[
									SNew(SHorizontalBox)
									+ SHorizontalBox::Slot()
									[
										SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
											.AutoWidth()
											[
												SNew(SButton)
												.Text(LOCTEXT("LoadConfig", "Load"))
												.HAlign(HAlign_Center)
												.OnClicked(this, &SConfPanel::BtnClickEventLoadConfig)
											]
										+ SHorizontalBox::Slot()
											.AutoWidth()
											[
												SNew(SButton)
												.Text(LOCTEXT("SaveConfig", "Save"))
											.HAlign(HAlign_Center)
											.OnClicked(this, &SConfPanel::BtnClickEventSaveConfig)
											]
											+ SHorizontalBox::Slot()
											.AutoWidth()
											[
												SNew(SButton)
												.Text(LOCTEXT("ResetConfig","Reset"))
												.HAlign(HAlign_Center)
												.VAlign(VAlign_Center)
												.OnClicked(this, &SConfPanel::BtnClickEventClearConfig)
											]
											+SHorizontalBox::Slot()
											.FillWidth(1.0)
											[
												SNew(SOverlay)
											]
											//+SHorizontalBox::Slot()
											//.AutoWidth()
											//.HAlign(HAlign_Center)
											//[
											//	SNew(SHyperlink)
											//		.Text(LOCTEXT("Developer", "Developed by lipengzha"))
											//		.OnNavigate(this, &SConfPanel::HyLinkClickEventOpenDeveloperWebsite)
											//]
									]
								]
								+SVerticalBox::Slot()
								.Padding(1.5)
									[
										SNew(SHeader)
										.HAlign(HAlign_Center)
										.Content()
										[
											SNew(STextBlock)
											.Text(LOCTEXT("ConfigurationText", "Configuration"))
										]
									]
							]
						+ SGridPanel::Slot(0, 2)
							.HAlign(HAlign_Fill)
							.Padding(0.0f, 2.0f,0.0f,0.0f)
							[
								SNew(SVerticalBox)
								+ SVerticalBox::Slot()
								[
									SNew(SHorizontalBox)
									+ SHorizontalBox::Slot()
									.HAlign(HAlign_Left)
									[
										SNew(STextBlock)
										.Text(LOCTEXT("SelectEngine", "Select Engine Version:"))
									]

								]
								+ SVerticalBox::Slot()
									.AutoHeight()
									.Padding(5.0f)
									[
										SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										.AutoWidth()
										.HAlign(HAlign_Left)
										[
									
											SAssignNew(CmbEngineSelector,SCombBoxWarper)
											.OnSelectorItemChanged(this,&SConfPanel::HandleEngineSelectorChanged)
										]
										+ SHorizontalBox::Slot()
										.AutoWidth()
										.HAlign(HAlign_Left)
										[
											SAssignNew(CmbToolSelector, SCombBoxWarper)
										]
										+ SHorizontalBox::Slot()
										.HAlign(HAlign_Left)
										.AutoWidth()
										[
											SAssignNew(BtnLaunchEngine, SButton)
											.Text(LOCTEXT("LaunchEngine", "Launch"))
											.Text(this, &SConfPanel::GetLaunchEngineBtnText)
											.OnClicked(this, &SConfPanel::BtnClickEventLaunchEngine)
										]
										+ SHorizontalBox::Slot()
											.HAlign(HAlign_Left)
											.AutoWidth()
											[
												SAssignNew(BtnOpenVS, SButton)
												.Text(LOCTEXT("OpenSln", "OpenSln"))
												.OnClicked(this, &SConfPanel::BtnClickEventOpenVS)
											]
											+ SHorizontalBox::Slot()
											.HAlign(HAlign_Left)
											.AutoWidth()
											[
												SNew(SButton)
												.Text(LOCTEXT("OpenEngineDir", "OpenDir"))
												.OnClicked(this, &SConfPanel::BtnClickEventOpenEngineDir)
											]
									]
							]
					// open project location
					+ SGridPanel::Slot(0, 4)
						.HAlign(HAlign_Left)
						.Padding(0.0f, 2.0f, 0.0f, 0.0f)
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot()
								[
									SNew(STextBlock)
									.Text(LOCTEXT("UE Project File", "Select your .uproject file:"))
								]
							+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(5.0f)
								[
									// show uproject file path
									SNew(SHorizontalBox)
									+ SHorizontalBox::Slot()
										//.AutoWidth()
										.FillWidth(1.0f)
										[
											SNew(SEditableTextBox)
											.Text(this, &SConfPanel::GetProjectFileText)
											.HintText(LOCTEXT("SEditableTextBoxHint", "Please select you want launch .uproject file."))
											.OnTextChanged(this, &SConfPanel::OnProjectFileTextBoxChanged)

										]
									// open uproject file button
									+ SHorizontalBox::Slot()
										//.FillWidth(0.3f)
										.AutoWidth()
										[
											SNew(SButton)
											.Text(LOCTEXT("SelectProjectFile", "Select"))
											.HAlign(HAlign_Center)
											.VAlign(VAlign_Center)
											.OnClicked(this, &SConfPanel::BtnClickEventOpenProjectFile)
										]
									+ SHorizontalBox::Slot()
										//.FillWidth(0.3f)
										.AutoWidth()
										[
											SNew(SButton)
											.Text(LOCTEXT("OpenInExplorer", "OpenDir"))
											.HAlign(HAlign_Center)
											.VAlign(VAlign_Center)
											.OnClicked(this, &SConfPanel::BtnClickEventOpenProjectFileDir)
										]
									+ SHorizontalBox::Slot()
										//.FillWidth(0.3f)
										.AutoWidth()
										[
											SAssignNew(BtnGenerateSln, SButton)
											.Text(LOCTEXT("GenerateSln", "GenerateSln"))
											.HAlign(HAlign_Center)
											.VAlign(VAlign_Center)
											.Visibility_Raw(this,&SConfPanel::BtnGenerateSlnVisibility)
											.OnClicked(this, &SConfPanel::BtnClickEventGenerateProjectSln)
										]
									+ SHorizontalBox::Slot()
										//.FillWidth(0.3f)
										.AutoWidth()
										[
											SAssignNew(BtnOpenProjectSln,SButton)
											.Text(LOCTEXT("OpenProjestSln", "OpenSln"))
										.HAlign(HAlign_Center)
										.VAlign(VAlign_Center)
										.OnClicked(this, &SConfPanel::BtnClickEventOpenProjectSln)
										]
								]

						]

					// input launch params
					+ SGridPanel::Slot(0, 5)
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot()
								.AutoHeight()
								[
									SNew(STextBlock)
									.Text(LOCTEXT("LauncherParams", "Launch Parameters:"))
								]
							+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(3.0f)
								[
									// init create a input box
									SAssignNew(SrbWidgetLaunchParams, SScrollBox)
									+ SScrollBox::Slot()
									.Padding(0.0f, 3.0f)
									[
										CreateEditableTextBox(TEXT(""))
									]
								]
								// add a new input param box
								+ SVerticalBox::Slot()
									.AutoHeight()
									[
										SNew(SHorizontalBox)
										+SHorizontalBox::Slot()
										.FillWidth(1.0)
										[
											SNew(SOverlay)
										]
										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											SNew(SButton)
											.Text(LOCTEXT("ClearAllParams", "Clear All"))
											.HAlign(HAlign_Center)
											.OnClicked(this, &SConfPanel::BtnClickEventClearAllLaunchParamsButton)
										]
										+ SHorizontalBox::Slot()
										.AutoWidth()
											[
												SNew(SButton)
												.Text(LOCTEXT("AddParameter", "Add Parameter"))
												.HAlign(HAlign_Center)
												.OnClicked(this, &SConfPanel::BtnClickEventAddLaunchParamButton)
											]
									]
							]

					// SHeader
					+ SGridPanel::Slot(0, 6)
						// .Padding(0.0f, 3.0f, 0.0f, 3.0f)
						.Padding(3.0)
						[
							SNew(SHeader)
							.HAlign(HAlign_Center)
							.Content()
							[
								SNew(STextBlock)
								.Text(LOCTEXT("ManageConfig", "Configuration Manager"))
							]
						]

					// Launch Button
					+ SGridPanel::Slot(0, 7)
						[
							SNew(SHorizontalBox)
							+SHorizontalBox::Slot()
							.FillWidth(1.0)
							[
								SNew(SOverlay)
							]
							+ SHorizontalBox::Slot()
							.AutoWidth()
							[
								SNew(SButton)
								.Text(LOCTEXT("AddToGlobalConfig", "Add To Global"))
								.HAlign(HAlign_Center)
								.OnClicked(this, &SConfPanel::BtnClickEventAddToGlobal)
							]
							+ SHorizontalBox::Slot()
							.AutoWidth()
							[
								SAssignNew(BtnLaunchProject, SButton)
								.Text(this, &SConfPanel::GetLaunchProjectBtnText)
								.HAlign(HAlign_Center)
								.VAlign(VAlign_Center)
								.OnClicked(this, &SConfPanel::BtnClickEventLaunchProject)
							]
						]
					]
				]
				]

	];
	// initialize
	{
		UpdateEngineSelector(EngineLaunchTools::GetAllRegistedEngineMap());
		UpdateToolSelector(EngineLaunchTools::GetToolList());
		UpdateSelectedProject();
		UpdateLaunchParams();
	}

}
#include "Layout/Visibility.h"
void SConfPanel::HyLinkClickEventOpenDeveloperWebsite()
{
	FPlatformProcess::LaunchURL(TEXT("https://imzlp.com"), NULL, NULL);
}

EVisibility SConfPanel::BtnGenerateSlnVisibility()const
{
	EVisibility VisibilityStatus = EVisibility::Collapsed;
	FString ProjectPath = GetLaunchConf().Project;
	if(ProjectPath.EndsWith(TEXT(".uproject")) && FPaths::FileExists(ProjectPath))
	{
		VisibilityStatus = EVisibility::Visible;
	}
	return VisibilityStatus;
}

FReply SConfPanel::BtnClickEventAddLaunchParamButton()
{
	AddParamTextBoxToSlot(TEXT(""));
	return FReply::Handled();
}
FReply SConfPanel::BtnClickEventClearAllLaunchParamsButton()
{
	SrbWidgetLaunchParams->ClearChildren();
	AddParamTextBoxToSlot(TEXT(""));
	return FReply::Handled();
}

FReply SConfPanel::BtnClickEventLaunchEngine()
{
	FLaunchConf CurrentConf = GetLaunchConf();
	CurrentConf.Project = TEXT("");
	CurrentConf.Params.Empty();

	EngineLaunchTools::EngineLauncher(CurrentConf);
	return FReply::Handled();
}

FReply SConfPanel::BtnClickEventLaunchProject()
{
	EngineLaunchTools::EngineLauncher(GetLaunchConf());

	return FReply::Handled();
}
FReply SConfPanel::BtnClickEventOpenProjectFile()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

	if (DesktopPlatform)
	{
		TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(SharedThis(this));

		TArray<FString> OpenFilenames;
		const bool bOpened = DesktopPlatform->OpenFileDialog(
			(ParentWindow.IsValid()) ? ParentWindow->GetNativeWindow()->GetOSWindowHandle() : nullptr,
			LOCTEXT("OpenUEProjectDialogTitle", "Open .uproject").ToString(),
			FString(TEXT("")),
			TEXT(""),
			TEXT("UE4 Project (*.uproject)|*.uproject"),
			EFileDialogFlags::None,
			OpenFilenames
		);

		if (OpenFilenames.Num() > 0)
		{
			FString local_OpenProjectFilePath = FPaths::ConvertRelativePathToFull(OpenFilenames[0]);
			UpdateSelectedProject(local_OpenProjectFilePath);
		}
	}
	return FReply::Handled();
}

FReply SConfPanel::BtnClickEventOpenProjectFileDir()
{
	FString FinalCommdParas = TEXT("/e,/root,");

	FString IsValidDir=EngineLaunchTools::GetProjectDir(GetSelectedProjectPath());
	FinalCommdParas.Append(IsValidDir);

	FPlatformProcess::CreateProc(TEXT("explorer "), *FinalCommdParas, true, false, false, NULL, NULL, NULL, NULL, NULL);
	return FReply::Handled();
}

FReply SConfPanel::BtnClickEventOpenProjectSln()
{
	if (!GetSelectedProjectPath().IsEmpty() && GetSelectedProjectPath().EndsWith(TEXT("uproject")))
	{
		FString local_ProjectSln = GetSelectedProjectPath();
		{
			local_ProjectSln.RemoveFromEnd(TEXT("uproject"));
			local_ProjectSln.Append(TEXT("sln\""));
			local_ProjectSln.InsertAt(0, TEXT("\""));
		}

		FString FinalCmdParams = TEXT("/c ") + local_ProjectSln;
		FPlatformProcess::CreateProc(TEXT("cmd.exe"), *FinalCmdParams, true, false, false, NULL, NULL, NULL, NULL, NULL);
	}
	return FReply::Handled();
}

FReply SConfPanel::BtnClickEventGenerateProjectSln()
{
	FString EnginePath = GetLaunchConf().Engine;
	FString ProjectFile = GetLaunchConf().Project;
	FString UnrealBuildTool = EngineLaunchTools::GetUnrealBuildToolBin(EnginePath,EngineLaunchTools::GetBuildVersion(EnginePath));
	
	if(!UnrealBuildTool.IsEmpty() && ProjectFile.EndsWith(TEXT(".uproject")))
	{
		FString AllParams = FString::Printf(
			TEXT("-projectfiles -project=\"%s\" -game -rocket -progress"),
			*ProjectFile
			);
		FPlatformProcess::CreateProc(*UnrealBuildTool, *AllParams, true, false, false, NULL, NULL, NULL, NULL, NULL);
	}
	return FReply::Handled();
}

FReply SConfPanel::BtnClickEventOpenVS()
{
	FString ue4sln = GetSelectedEnginePath() + TEXT("//UE4.sln");
	{
		ue4sln.InsertAt(0, TEXT("\""));
		ue4sln.Append(TEXT("\""));
	}
	FString FinalCmdParams = TEXT("/c ") + ue4sln;
	FPlatformProcess::CreateProc(TEXT("cmd.exe"), *FinalCmdParams, true, false, false, NULL, NULL, NULL, NULL, NULL);
	return FReply::Handled();
}

FReply SConfPanel::BtnClickEventOpenEngineDir()
{
	FString FinalCommdParas = TEXT("/e,/root,");

	FString IsValidDir = GetSelectedEnginePath();
	FPaths::NormalizeFilename(IsValidDir);
	IsValidDir = IsValidDir.Replace(TEXT("/"),TEXT("\\"));
	FinalCommdParas.Append(IsValidDir);

	FPlatformProcess::CreateProc(TEXT("explorer "), *FinalCommdParas, true, false, false, NULL, NULL, NULL, NULL, NULL);
	return FReply::Handled();
}

FReply SConfPanel::BtnClickEventClearConfig()
{
	SetOpenedFile();
	FLaunchConf DefaultConfig;
	UpdateAll(DefaultConfig);
	return FReply::Handled();
}
FReply SConfPanel::BtnClickEventLoadConfig()
{
	FString SelectedLoadConfigPath;
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

	if (DesktopPlatform)
	{
		TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(SharedThis(this));

		TArray<FString> OpenFilenames;
		const bool bOpened = DesktopPlatform->OpenFileDialog(
			(ParentWindow.IsValid()) ? ParentWindow->GetNativeWindow()->GetOSWindowHandle() : nullptr,
			LOCTEXT("OpenUE4LaunchConfig", "Open .uejson").ToString(),
			FString(TEXT("")),
			TEXT(""),
			TEXT("UE4Launcher json (*.uejson)|*.uejson"),
			EFileDialogFlags::None,
			OpenFilenames
		);

		if (OpenFilenames.Num() > 0)
		{
			SelectedLoadConfigPath = FPaths::ConvertRelativePathToFull(OpenFilenames[0]);
			{
				SetOpenedFile(SelectedLoadConfigPath);
				FString jsonValue;
				bool flag = FFileHelper::LoadFileToString(jsonValue, *SelectedLoadConfigPath);
				if (flag)
				{
					FLaunchConf conf=SerializationTools::DeSerializationConf(jsonValue);
					UpdateAll(conf);
				}
			}
		}
	}
	return FReply::Handled();
}

FReply SConfPanel::BtnClickEventAddToGlobal()
{
	OnAddToGlobal.ExecuteIfBound(GetLaunchConf());
	return FReply::Handled();
}
FReply SConfPanel::BtnClickEventSaveConfig()
{
	FString SelectedSaveConfigPath;

	if (OpenedConfFile.IsEmpty())
	{
		IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

		if (DesktopPlatform)
		{
			TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(SharedThis(this));

			TArray<FString> SaveFilenames;
			const bool bOpened = DesktopPlatform->SaveFileDialog(
				(ParentWindow.IsValid()) ? ParentWindow->GetNativeWindow()->GetOSWindowHandle() : nullptr,
				LOCTEXT("SvedUE4LaunchConfig", "Save .uejson").ToString(),
				FString(TEXT("")),
				TEXT(""),
				TEXT("UE4Launcher json (*.uejson)|*.uejson"),
				EFileDialogFlags::None,
				SaveFilenames
			);

			if (SaveFilenames.Num() > 0)
			{
				SelectedSaveConfigPath = FPaths::ConvertRelativePathToFull(SaveFilenames[0]);
			}
		}
	}
	else
	{
		SelectedSaveConfigPath = GetOpenedFile();
	}
	// serialization config
	FFileHelper::SaveStringToFile(SerializationTools::SerializationConf(GetLaunchConf()), *SelectedSaveConfigPath);

	return FReply::Handled();
}

void SConfPanel::SetOpenedFile(const FString& Path)
{
	OpenedConfFile = FPaths::FileExists(Path)?Path:TEXT("");
	OnOpenedFileEvent.ExecuteIfBound(OpenedConfFile);
}

FString SConfPanel::GetOpenedFile()const
{
	return OpenedConfFile;
}
void SConfPanel::UpdateToolSelector(const TArray<FString>& ToolsList, const FString& DefaultTool)
{
	CmbToolSelector->ClearAllSelectItem();
	CmbToolSelector->UpdateSelector(ToolsList, DefaultTool);
}

void SConfPanel::UpdateAll(const FLaunchConf& conf)
{
	UpdateEngineSelector(EngineLaunchTools::GetAllRegistedEngineMap(),conf.Engine);
	UpdateToolSelector(EngineLaunchTools::GetToolList(), conf.Tool);
	UpdateSelectedProject(conf.Project);
	UpdateLaunchParams(conf.Params);
}
FLaunchConf SConfPanel::GetLaunchConf()const
{
	FLaunchConf Conf;
	Conf.Engine = GetSelectedEnginePath();
	Conf.Tool = GetSelectedTool();
	Conf.ToolPreArgs = EngineLaunchTools::GetToolInfo(GetSelectedTool()).PreArgs;
	Conf.Params = GetAllLaunchParams();
	Conf.Project = GetSelectedProjectPath();
	return Conf;
}

FString SConfPanel::GetSelectedTool()const
{
	return CmbToolSelector->GetCurretSelectedItem();
}
FText SConfPanel::GetProjectFileText()const
{
	return FText::FromString(OpenProjectFilePath);
}

FString SConfPanel::GetSelectedEnginePath()const
{
	return CmbEngineSelector->GetCurretSelectedItem();
}

FString SConfPanel::GetSelectedProjectPath()const
{
	return OpenProjectFilePath;
}

FText SConfPanel::GetLaunchEngineBtnText()const
{
	return FText::FromString(LaunchEngineBtnText);
}

FText SConfPanel::GetLaunchProjectBtnText()const
{
	return FText::FromString(LaunchProjectBtnText);
}

TArray<FString> SConfPanel::GetAllLaunchParams()const
{
	TArray<FString> resault;

	SScrollBox* ScrollboxWidget = &*SrbWidgetLaunchParams;
	FChildren* ScrollBoxChildren = SlateWidgetTools::GetScrollBoxChildren(ScrollboxWidget);

	if (ScrollBoxChildren->Num() > 0)
	{
		for (int32 index = 0; index < ScrollBoxChildren->Num(); index++)
		{
			SWidget* Widget = &(ScrollBoxChildren->GetChildAt(index).Get());
			SEditableBoxWraper* ScrollBoxChildTextItem = static_cast<SEditableBoxWraper*>(Widget);
			if (ScrollBoxChildTextItem)
			{
				FString TextItemString = ScrollBoxChildTextItem->GetEditableBoxText().ToString();
				if (TextItemString.Len() > 0)
				{
					resault.Add(TextItemString);
				}
			}
		}
	}

	return resault;
}


TSharedRef<SEditableBoxWraper> SConfPanel::CreateEditableTextBox(const FString& TextContent)
{
	TSharedRef<SEditableBoxWraper> CreatedWidget = SNew(SEditableBoxWraper)
		.EditableHintText(LOCTEXT("LaunchParam_0", "Please input Launch paramater."))
		.EditableText(FText::FromString(TextContent))
		.BtnOpenText(FText::FromString(TEXT("O")))
		.BtnClearText(FText::FromString(TEXT("C")))
		.BtnDeleteText(FText::FromString(TEXT("D")));
	return CreatedWidget;
}

void SConfPanel::AddParamTextBoxToSlot(const FString& TextContent)
{
	SrbWidgetLaunchParams->AddSlot()
		.Padding(0.0f, 3.0f)
		[
			CreateEditableTextBox(TextContent)
		];
	SrbWidgetLaunchParams->ScrollToEnd();
	SrbWidgetMain->ScrollToEnd();
}

void SConfPanel::HandleEngineSelectorChanged(const FString& NewEngine)
{
	UpdateOpenVSButton(NewEngine);
}


void SConfPanel::UpdateEngineSelector(const TMap<FString, FString>& EngineMap, FString DefaultEngine)
{
	CmbEngineSelector->UpdateSelector(EngineLaunchTools::GetAllRegistedEngineList(EngineMap),DefaultEngine);
}

void SConfPanel::UpdateOpenVSButton(const FString& EnginePath)
{
	bool IsLauncherInstalledEngine = FPaths::FileExists(EnginePath + TEXT("//Engine//Build//InstalledBuild.txt"));
	{
		BtnOpenVS->SetEnabled(!IsLauncherInstalledEngine);
		if (!IsLauncherInstalledEngine)
			BtnOpenVS->SetVisibility(EVisibility::Visible);
		else
			BtnOpenVS->SetVisibility(EVisibility::Collapsed);
	}
}


void SConfPanel::UpdateLaunchParams(const TArray<FString>& pParamsArray)
{
	SrbWidgetLaunchParams->ClearChildren();
	if (!pParamsArray.Num())
		AddParamTextBoxToSlot();
	for (const auto& ParamItem : pParamsArray)
	{
		if (!ParamItem.IsEmpty())
		{
			AddParamTextBoxToSlot(ParamItem);
		}
	}
}

void SConfPanel::UpdateSelectedProject(const FString& ProjectPath)
{
	OnProjectFileTextBoxChanged(FText::AsCultureInvariant(ProjectPath));
}


void SConfPanel::OnProjectFileTextBoxChanged(const FText& NewText)
{
	OpenProjectFilePath = NewText.ToString();
	UpdateOpenProjectSlnButton(OpenProjectFilePath);
}

void SConfPanel::UpdateOpenProjectSlnButton(const FString& SelectedProjectPath)
{
	bool HasSln = false;
	FString local_ProjectFullPath(SelectedProjectPath);

	if (!local_ProjectFullPath.IsEmpty() && 
		FPaths::FileExists(local_ProjectFullPath) &&
		local_ProjectFullPath.EndsWith(TEXT("uproject"))
		)
	{
		local_ProjectFullPath.RemoveFromEnd(TEXT("uproject"));
		local_ProjectFullPath.Append(TEXT("sln"));
		HasSln = FPaths::FileExists(local_ProjectFullPath);
	}

	if (HasSln)
	{
		BtnOpenProjectSln->SetVisibility(EVisibility::Visible);
	}
	else {
		BtnOpenProjectSln->SetVisibility(EVisibility::Collapsed);
	}
}

// TSharedRef<SWidget> MakeWidgetUELauncher()
// {
// 	extern TOptional<FSlateRenderTransform> GetTestRenderTransform();
// 	extern FVector2D GetTestRenderTransformPivot();
// 	return
// 		SNew(SConfPanel)
// 		.RenderTransform_Static(&GetTestRenderTransform)
// 		.RenderTransformPivot_Static(&GetTestRenderTransformPivot);
// }
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE
