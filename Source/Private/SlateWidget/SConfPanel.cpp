//// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
//
#include "SlateWidget/SConfPanel.h"

#include "Animation/CurveSequence.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SBoxPanel.h"
#include "Styling/SlateTypes.h"
#include "Styling/CoreStyle.h"
#include "Layout/WidgetPath.h"
#include "SlateOptMacros.h"
#include "Framework/Application/MenuStack.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Layout/SFxWidget.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Images/SSpinningImage.h"
#include "Widgets/Notifications/SProgressBar.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SHeader.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Input/SMenuAnchor.h"
#include "Widgets/Text/SMultiLineEditableText.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Input/SSlider.h"
#include "Widgets/Input/SComboBox.h"
#include "Framework/Docking/TabManager.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Navigation/SBreadcrumbTrail.h"
#include "Widgets/Images/SThrobber.h"
#include "Widgets/Input/SHyperlink.h"
#include "Widgets/Input/STextComboBox.h"
#include "Widgets/Input/SVolumeControl.h"
#include "Widgets/Input/STextComboPopup.h"
#include "Paths.h"

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
								[
									SNew(SHeader)
									.HAlign(HAlign_Center)
									.Content()
									[
										SNew(STextBlock)
										.Text(LOCTEXT("LoadSaveConfig", "Load/Save Config & Developer Info"))
									]
								]
								+SVerticalBox::Slot()
								.AutoHeight()
								[
									SNew(SHorizontalBox)
									+ SHorizontalBox::Slot()
									[
										SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
											[
												SNew(SButton)
												.Text(LOCTEXT("LoadConfig", "Load Config"))
												.HAlign(HAlign_Center)
												.OnClicked(this, &SConfPanel::BtnClickEventLoadConfig)
											]
										+ SHorizontalBox::Slot()
											[
												SNew(SButton)
												.Text(LOCTEXT("SaveConfig", "Save Config"))
											.HAlign(HAlign_Center)
											.OnClicked(this, &SConfPanel::BtnClickEventSaveConfig)
											]
										+SHorizontalBox::Slot()
											.HAlign(HAlign_Center)
											[
												SNew(SHyperlink)
													.Text(LOCTEXT("Developer", "Developed by imzlp.me"))
													.OnNavigate(this, &SConfPanel::HyLinkClickEventOpenDeveloperWebsite)
											]
									]
								]
								+SVerticalBox::Slot()
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
											//.Text(LOCTEXT("LaunchEngine", "Launch"))
											.Text(this, &SConfPanel::GetLaunchEngineBtnText)
											.OnClicked(this, &SConfPanel::BtnClickEventLaunchEngine)
										]
										+ SHorizontalBox::Slot()
											.HAlign(HAlign_Left)
											.AutoWidth()
											[
												SAssignNew(BtnOpenVS, SButton)
												.Text(LOCTEXT("OpenVS", "OpenVS"))
												.OnClicked(this, &SConfPanel::BtnClickEventOpenVS)
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
										+ SHorizontalBox::Slot()
										[
											SNew(SButton)
											.Text(LOCTEXT("ClearAllParams", "Clear All Params"))
											.HAlign(HAlign_Center)
											.OnClicked(this, &SConfPanel::BtnClickEventClearAllLaunchParamsButton)
										]
										+ SHorizontalBox::Slot()
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
						.Padding(0.0f, 3.0f, 0.0f, 3.0f)
						[
							SNew(SHeader)
							.HAlign(HAlign_Center)
							.Content()
							[
								SNew(STextBlock)
								.Text(LOCTEXT("ManageConfig", "Manage Config"))
							]
						]

					// Launch Button
					+ SGridPanel::Slot(0, 7)
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							[
								SNew(SButton)
								.Text(LOCTEXT("ClearConfig","Clear Config"))
								.HAlign(HAlign_Center)
								.VAlign(VAlign_Center)
								.OnClicked(this, &SConfPanel::BtnClickEventClearConfig)
							]
							+SHorizontalBox::Slot()
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
	TMap<FString, FString> EngineMap = EngineLaunchTools::GetAllRegistedEngineMap();
	{
		UpdateEngineSelector(EngineMap);
		UpdateToolSelector(EngineLaunchTools::GetToolList());
		UpdateSelectedProject();
		UpdateLaunchParams();
	}

}

void SConfPanel::HyLinkClickEventOpenDeveloperWebsite()
{
	FPlatformProcess::LaunchURL(TEXT("https://imzlp.me"), NULL, NULL);
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
	EngineLaunchTools::EngineLauncher(GetLaunchConf());
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
			OpenProjectFilePath = FPaths::ConvertRelativePathToFull(OpenFilenames[0]);
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

FReply SConfPanel::BtnClickEventOpenVS()
{
	FString ue4sln = GetSelectedEnginePath() + TEXT("//UE4.sln");
	FString FinalCmdParams = TEXT("/c start devenv.exe ") + ue4sln;
	FPlatformProcess::CreateProc(TEXT("cmd.exe"), *FinalCmdParams, true, false, false, NULL, NULL, NULL, NULL, NULL);
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
	UpdateEngineSelector(RegisterEngineMap,conf.Engine);
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
	CmbEngineSelector->UpdateSelector(EngineLaunchTools::GetAllRegistedEngineList(EngineMap));
}

void SConfPanel::UpdateOpenVSButton(const FString& EnginePath)
{
	bool IsLauncherInstalledEngine = FPaths::FileExists(EnginePath + TEXT("//Engine//Build//InstalledBuild.txt"));
	{
		BtnOpenVS->SetEnabled(!IsLauncherInstalledEngine);
		if (!IsLauncherInstalledEngine)
			BtnOpenVS->SetVisibility(EVisibility::Visible);
		else
			BtnOpenVS->SetVisibility(EVisibility::Hidden);
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
	OpenProjectFilePath = ProjectPath;
}


void SConfPanel::OnProjectFileTextBoxChanged(const FText& NewText)
{
	OpenProjectFilePath = NewText.ToString();
}

TSharedRef<SWidget> MakeWidgetUELauncher()
{
	extern TOptional<FSlateRenderTransform> GetTestRenderTransform();
	extern FVector2D GetTestRenderTransformPivot();
	return
		SNew(SConfPanel)
		.RenderTransform_Static(&GetTestRenderTransform)
		.RenderTransformPivot_Static(&GetTestRenderTransformPivot);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE
