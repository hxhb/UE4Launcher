//// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
//
#include "WidgetUELauncher.h"
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

//
#define LOCTEXT_NAMESPACE "WidgetUELauncher"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SWidgetUELauncher::Construct(const FArguments& InArgs)
{
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
										.Text(LOCTEXT("LoadSaveConfig", "Load/Save Config"))
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
												.OnClicked(this, &SWidgetUELauncher::BtnClickEventLoadConfig)
											]
										+ SHorizontalBox::Slot()
											[
												SNew(SButton)
												.Text(LOCTEXT("SaveConfig", "Save Config"))
												.HAlign(HAlign_Center)
												.OnClicked(this, &SWidgetUELauncher::BtnClickEventSaveConfig)
											]
										+SHorizontalBox::Slot()
											.HAlign(HAlign_Center)
											[
												SNew(SHyperlink)
													.Text(LOCTEXT("Developer", "Developed by imzlp.me"))
													.OnNavigate(this, &SWidgetUELauncher::HyLinkClickEventOpenDeveloperWebsite)
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
							.Padding(0.0f, 3.0f)
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
									//+ SHorizontalBox::Slot()
									//.AutoWidth()
									//.HAlign(HAlign_Right)
									//[
									//	SNew(SHyperlink)
									//	.Text(LOCTEXT("Developer", "Developed by imzlp.me"))
									//	.OnNavigate(this, &SWidgetUELauncher::HyLinkClickEventOpenDeveloperWebsite)
									//]

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
											SAssignNew(CmdWidgetEngineSelector, SComboBox<TSharedPtr<FString>>)
											.OptionsSource(&SelectorInstalledEngineList)
											.OnSelectionChanged(this, &SWidgetUELauncher::HandleCmbEngineSelectionChanged)
											.OnGenerateWidget(this, &SWidgetUELauncher::HandleCmbEngineGenerateWidget)
											[
												SNew(STextBlock)
												.Text(this, &SWidgetUELauncher::HandleCmdEngineSelectionChangeText)
											]
										]
										+ SHorizontalBox::Slot()
										.HAlign(HAlign_Left)
										.AutoWidth()
										[
											SAssignNew(BtnLaunchEngine, SButton)
											//.Text(LOCTEXT("LaunchEngine", "Launch"))
											.Text(this, &SWidgetUELauncher::GetLaunchEngineBtnText)
											.OnClicked(this, &SWidgetUELauncher::BtnClickEventLaunchEngine)
										]
										+ SHorizontalBox::Slot()
											.HAlign(HAlign_Left)
											.AutoWidth()
											[
												SAssignNew(BtnOpenVS, SButton)
												.Text(LOCTEXT("OpenVS", "OpenVS"))
												.OnClicked(this, &SWidgetUELauncher::BtnClickEventOpenVS)
											]
									]
							]
					+ SGridPanel::Slot(0, 3)
						.HAlign(HAlign_Fill)
						.Padding(0.0f, 3.0f)
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
								.AutoWidth()
								.HAlign(HAlign_Left)
								.VAlign(VAlign_Center)
								[
									SNew(STextBlock)
									.Text(LOCTEXT("SelectPlatfrom", "Platfrom:"))
								]
							+ SHorizontalBox::Slot()
								.AutoWidth()
								.HAlign(HAlign_Left)
								.Padding(2.0f)
								[
									SAssignNew(CmdWidgetPlatfromSelector, SComboBox<TSharedPtr<FString>>)
									.OptionsSource(&SelectorPlatfromList)
									.OnSelectionChanged(this, &SWidgetUELauncher::HandleCmbPlatfromSelectionChanged)
									.OnGenerateWidget(this, &SWidgetUELauncher::HandleCmbPlatfromGenerateWidget)
									[
										SNew(STextBlock)
										.Text(this, &SWidgetUELauncher::HandleCmdPlatfromSelectionChangeText)
									]
								]
								+ SHorizontalBox::Slot()
									.AutoWidth()
									.HAlign(HAlign_Left)
									.VAlign(VAlign_Center)
									.Padding(5.0f, 0.0f, 0.0f, 0.0f)
									[
										SNew(STextBlock)
										.Text(LOCTEXT("UseCmdEngine", "UseCmd:"))
									]
								+ SHorizontalBox::Slot()
									.AutoWidth()
									.HAlign(HAlign_Left)
									.Padding(2.0f)
									[
										SAssignNew(CbUseCmdEngine, SCheckBox)
										.IsChecked(this, &SWidgetUELauncher::HandleUseCmdCBStateIsChecked, &bUseCmdEngine)
										.OnCheckStateChanged(this, &SWidgetUELauncher::HandleUseCmdCBStateChanged, &bUseCmdEngine)
										[
											SNew(STextBlock)
											.Text(LOCTEXT("UE4EditorCMD", "UE4Editor-cmd"))

										]
									]
						]
					// open project location
					+ SGridPanel::Slot(0, 4)
						.HAlign(HAlign_Left)
						.Padding(0.0f, 3.0f)
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
											.Text(this, &SWidgetUELauncher::GetProjectFileText)
											.HintText(LOCTEXT("SEditableTextBoxHint", "Please select you want launch .uproject file."))
											.OnTextChanged(this, &SWidgetUELauncher::OnProjectFileTextBoxChanged)

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
											.OnClicked(this, &SWidgetUELauncher::BtnClickEventOpenProjectFile)
										]
									+ SHorizontalBox::Slot()
										//.FillWidth(0.3f)
										.AutoWidth()
										[
											SNew(SButton)
											.Text(LOCTEXT("OpenInExplorer", "OpenDir"))
											.HAlign(HAlign_Center)
											.VAlign(VAlign_Center)
											.OnClicked(this, &SWidgetUELauncher::BtnClickEventOpenProjectFileDir)
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
										SNew(SEditableTextBox)
										.HintText(LOCTEXT("LaunchParam_0", "Input Project Launch paramater."))
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
											.OnClicked(this, &SWidgetUELauncher::BtnClickEventClearAllLaunchParamsButton)
										]
										+ SHorizontalBox::Slot()
											[
												SNew(SButton)
												.Text(LOCTEXT("AddParameter", "Add Parameter"))
												.HAlign(HAlign_Center)
												.OnClicked(this, &SWidgetUELauncher::BtnClickEventAddLaunchParamButton)
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
								.Text(LOCTEXT("HeaderContentLabel", "Launch"))
							]
						]

					// Launch Button
					+ SGridPanel::Slot(0, 7)
						[
							SAssignNew(BtnLaunchProject, SButton)
							.Text(this, &SWidgetUELauncher::GetLaunchProjectBtnText)
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Center)
							.OnClicked(this, &SWidgetUELauncher::BtnClickEventLaunchProject)
						]
					]
				]

				//+SHorizontalBox::Slot()
				//[
				//	SNew(SVerticalBox)
				//	+ SVerticalBox::Slot()
				//	[
				//		SNew(SHorizontalBox)
				//			+ SHorizontalBox::Slot()
				//			.HAlign(HAlign_Left)
				//			[
				//				SNew(STextBlock)
				//				.Text(LOCTEXT("HistoryList", "History:"))
				//			]
				//	]
				//]


				]

	];
	// initialize
	TMap<FString, FString> EngineMap = GetAllRegistedEngineMap();
	{
		
		//UpdateEngineSelector(EngineMap,TEXT("D:/UnrealEngine/Offical_Source/4.18"));
		//UpdatePlatfromSelector(GetSelectedEnginePath(), TEXT("Win32"));
		//UpdateSelectedProject(TEXT("C:/Users/visionsmile/Documents/Unreal Projects/OldScandinaviaMedievalVil/OldScandinaviaMedievalVil.uproject"));
		//UpdateLaunchParams({ TEXT("-game"),TEXT("-LOG") });
		//UpdateUseCmdEngine(true);
		UpdateEngineSelector(EngineMap);
		UpdatePlatfromSelector(GetSelectedEnginePath());
		UpdateSelectedProject();
		UpdateLaunchParams();
		UpdateUseCmdEngine();
	}

}



// Engine
void SWidgetUELauncher::HandleCmbEngineSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	CmbSelectCurrentEngine = NewSelection;
	UpdatePlatfromSelector(*CmbSelectCurrentEngine);
	UpdateOpenVSButton(*NewSelection);
}

TSharedRef<SWidget> SWidgetUELauncher::HandleCmbEngineGenerateWidget(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock)
		.Text(FText::FromString(*InItem));
}

FText SWidgetUELauncher::HandleCmdEngineSelectionChangeText() const
{
	return CmbSelectCurrentEngine.IsValid() ? FText::FromString(*CmbSelectCurrentEngine) : FText::GetEmpty();
}

// Platfrom
void SWidgetUELauncher::HandleCmbPlatfromSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	CmbSelectCurrentPlatfrom = NewSelection;
}

TSharedRef<SWidget> SWidgetUELauncher::HandleCmbPlatfromGenerateWidget(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock)
		.Text(FText::FromString(*InItem));
}

FText SWidgetUELauncher::HandleCmdPlatfromSelectionChangeText() const
{
	return CmbSelectCurrentPlatfrom.IsValid() ? FText::FromString(*CmbSelectCurrentPlatfrom) : FText::GetEmpty();
}

ECheckBoxState SWidgetUELauncher::HandleUseCmdCBStateIsChecked(bool* CheckBox) const
{
	return (*CheckBox)
		? ECheckBoxState::Checked
		: ECheckBoxState::Unchecked;
}
void SWidgetUELauncher::HandleUseCmdCBStateChanged(ECheckBoxState NewState, bool* CheckBoxThatChanged)
{
	*CheckBoxThatChanged = (NewState == ECheckBoxState::Checked);
}



void SWidgetUELauncher::HyLinkClickEventOpenDeveloperWebsite()
{
	FPlatformProcess::LaunchURL(TEXT("https://imzlp.me"), NULL, NULL);
}


FReply SWidgetUELauncher::BtnClickEventAddLaunchParamButton()
{
	AddParamTextBoxToSlot(TEXT(""));
	return FReply::Handled();
}
FReply SWidgetUELauncher::BtnClickEventClearAllLaunchParamsButton()
{
	SrbWidgetLaunchParams->ClearChildren();
	AddParamTextBoxToSlot(TEXT(""));
	return FReply::Handled();
}


FReply SWidgetUELauncher::BtnClickEventLaunchEngine()
{

	EngineLauncher(GetSelectedEngineBinPath(), TEXT(""));
	return FReply::Handled();
}

FReply SWidgetUELauncher::BtnClickEventLaunchProject()
{
	FString EnginePath=GetSelectedEngineBinPath();
	FString ProjectPath = GetSelectedProjectPath();
	FString AllParams = CombineAllLaunchParams(GetAllLaunchParams());

	if(!(EnginePath.Len()>0 && FPaths::FileExists(*EnginePath)))
		return FReply::Handled();
	if (!(EnginePath.Len() > 0 && FPaths::FileExists(*EnginePath)))
	{
		ProjectPath.Empty();
	}
	{
		FString Params = FString::Printf(TEXT("\"%s\" %s"), *ProjectPath, *AllParams);
		EngineLauncher(EnginePath, Params);
	}

	return FReply::Handled();
}
FReply SWidgetUELauncher::BtnClickEventOpenProjectFile()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

	if (DesktopPlatform)
	{
		TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(SharedThis(this));

		TArray<FString> OpenFilenames;
		const bool bOpened = DesktopPlatform->OpenFileDialog(
			(ParentWindow.IsValid()) ? ParentWindow->GetNativeWindow()->GetOSWindowHandle() : nullptr,
			LOCTEXT("OpenUEProjectDialogTitle", "Open .uproject").ToString(),
			FString(TEXT("C:\\")),
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

FReply SWidgetUELauncher::BtnClickEventOpenProjectFileDir()
{
	FString FinalCommdParas = TEXT("/e,/root,");
	TArray<FString> OutArray;
	GetSelectedProjectPath().ParseIntoArray(OutArray, TEXT("/"));

	FString FinalValidPath;
	{
		for (const auto& item : OutArray)
		{
			if (FPaths::DirectoryExists(FinalValidPath + item))
			{
				FinalValidPath.Append(item);
				FinalValidPath.Append(TEXT("\\"));
			}
		}
	}
	FinalCommdParas.Append(FinalValidPath);

	FPlatformProcess::CreateProc(TEXT("explorer "), *FinalCommdParas, true, false, false, NULL, NULL, NULL, NULL, NULL);
	return FReply::Handled();
}

FReply SWidgetUELauncher::BtnClickEventOpenVS()
{
	FString ue4sln = GetSelectedEnginePath() + TEXT("//UE4.sln");
	FString FinalCmdParams = TEXT("/c start devenv.exe ") + ue4sln;
	FPlatformProcess::CreateProc(TEXT("cmd.exe"), *FinalCmdParams, true, false, false, NULL, NULL, NULL, NULL, NULL);
	return FReply::Handled();
}

FReply SWidgetUELauncher::BtnClickEventLoadConfig()
{
	FString SelectedLoadConfigPath;
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

	if (DesktopPlatform)
	{
		TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(SharedThis(this));

		TArray<FString> OpenFilenames;
		const bool bOpened = DesktopPlatform->OpenFileDialog(
			(ParentWindow.IsValid()) ? ParentWindow->GetNativeWindow()->GetOSWindowHandle() : nullptr,
			LOCTEXT("OpenUE4LaunchConfig", "Open .json").ToString(),
			FString(TEXT("C:\\")),
			TEXT(""),
			TEXT("UE4Launcher json (*.json)|*.json"),
			EFileDialogFlags::None,
			OpenFilenames
		);

		if (OpenFilenames.Num() > 0)
		{
			SelectedLoadConfigPath = FPaths::ConvertRelativePathToFull(OpenFilenames[0]);
			{
				FString jsonValue;
				bool flag = FFileHelper::LoadFileToString(jsonValue, *SelectedLoadConfigPath);
				if (flag)
				{
					FUELaunchConf conf=DeSerializationConf(jsonValue);
					UpdateAll(conf);
				}
			}
		}
	}
	return FReply::Handled();
}
FReply SWidgetUELauncher::BtnClickEventSaveConfig()
{
	FString SelectedSaveConfigPath;
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

	if (DesktopPlatform)
	{
		TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(SharedThis(this));

		TArray<FString> SaveFilenames;
		const bool bOpened = DesktopPlatform->SaveFileDialog(
			(ParentWindow.IsValid()) ? ParentWindow->GetNativeWindow()->GetOSWindowHandle() : nullptr,
			LOCTEXT("OpenUE4LaunchConfig", "Open .json").ToString(),
			FString(TEXT("C:\\")),
			TEXT(""),
			TEXT("UE4Launcher json (*.json)|*.json"),
			EFileDialogFlags::None,
			SaveFilenames
		);

		if (SaveFilenames.Num() > 0)
		{
			SelectedSaveConfigPath = FPaths::ConvertRelativePathToFull(SaveFilenames[0]);

			{
				// serialization config
				FFileHelper::SaveStringToFile(SerializationConf(GetLaunchConf()), *SelectedSaveConfigPath);
			}
		}


	}
	return FReply::Handled();
}

FUELaunchConf SWidgetUELauncher::GetLaunchConf()const
{
	FUELaunchConf Conf;
	Conf.LaunchEngine = GetSelectedEnginePath();
	Conf.LaunchPlatfrom = *GetSelectedPlatfrom();
	Conf.bUseCmdEngine = GetUseCmdEngine();
	Conf.LaunchParams = GetAllLaunchParams();
	Conf.LaunchProject = GetSelectedProjectPath();
	return Conf;
}
FString SWidgetUELauncher::SerializationConf(const FUELaunchConf& SaveConfig)
{
	FString JsonStr;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);
	JsonWriter->WriteObjectStart();
	JsonWriter->WriteValue(TEXT("LaunchEngine"), SaveConfig.LaunchEngine);
	JsonWriter->WriteValue(TEXT("LaunchPlatfrom"), SaveConfig.LaunchPlatfrom);
	JsonWriter->WriteValue(TEXT("LaunchProject"), SaveConfig.LaunchProject);
	JsonWriter->WriteValue(TEXT("bUseCmdEngine"), SaveConfig.bUseCmdEngine);
	JsonWriter->WriteArrayStart("LaunchParams");
	for (const auto& ParamItem : SaveConfig.LaunchParams)
	{
		JsonWriter->WriteObjectStart();
		JsonWriter->WriteValue(TEXT("ParamItem"), ParamItem);
		JsonWriter->WriteObjectEnd();
	}
	JsonWriter->WriteArrayEnd();
	JsonWriter->WriteObjectEnd();
	JsonWriter->Close();
	return JsonStr;
}
FUELaunchConf SWidgetUELauncher::DeSerializationConf(const FString& jsonConf)
{
	FUELaunchConf loadConf;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(jsonConf);
	TSharedPtr<FJsonObject> JsonObject;
	bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);
	if (BFlag)
	{
		loadConf.LaunchEngine = JsonObject->GetStringField(TEXT("LaunchEngine"));
		loadConf.LaunchPlatfrom = JsonObject->GetStringField(TEXT("LaunchPlatfrom"));
		loadConf.LaunchProject = JsonObject->GetStringField(TEXT("LaunchProject"));
		loadConf.bUseCmdEngine = JsonObject->GetBoolField(TEXT("bUseCmdEngine"));
		TArray<TSharedPtr<FJsonValue>> LaunchParamsData = JsonObject->GetArrayField("LaunchParams");
		for (const auto& DataItem : LaunchParamsData)
		{
			loadConf.LaunchParams.Add(DataItem->AsObject()->GetStringField(TEXT("ParamItem")));
		}
	}
	return loadConf;
}

void SWidgetUELauncher::UpdateAll(const FUELaunchConf& conf)
{
	UpdateEngineSelector(RegisterEngineMap,conf.LaunchEngine);
	UpdatePlatfromSelector(GetSelectedEnginePath(),conf.LaunchPlatfrom);
	UpdateSelectedProject(conf.LaunchProject);
	UpdateLaunchParams(conf.LaunchParams);
	UpdateUseCmdEngine(conf.bUseCmdEngine);
}

TSharedPtr<FString> SWidgetUELauncher::GetSelectedPlatfrom()const
{
	return CmbSelectCurrentPlatfrom;
}

FText SWidgetUELauncher::GetProjectFileText()const
{
	return FText::FromString(OpenProjectFilePath);
}

FString SWidgetUELauncher::GetSelectedEnginePath()const
{
	return *CmbSelectCurrentEngine;
}
FString SWidgetUELauncher::GetSelectedEngineBinPath()const
{
	TSharedPtr<FString> Platfrom = GetSelectedPlatfrom();
	FString EngineProgramName = GetUseCmdEngine() ? TEXT("UE4Editor-cmd.exe") : TEXT("UE4Editor.exe");
	FString resault(FPaths::Combine(*GetSelectedEnginePath(), TEXT("Engine/Binaries/"),*Platfrom,*EngineProgramName));
	return resault;
}

FString SWidgetUELauncher::GetSelectedProjectPath()const
{
	return OpenProjectFilePath;
}

FText SWidgetUELauncher::GetLaunchEngineBtnText()const
{
	return FText::FromString(LaunchEngineBtnText);
}

FText SWidgetUELauncher::GetLaunchProjectBtnText()const
{
	return FText::FromString(LaunchProjectBtnText);
}
#include "../Tools/HackPrivateMember.hpp"
DECL_HACK_PRIVATE_DATA(SScrollBox, TSharedPtr<SScrollPanel>, ScrollPanel)

TArray<FString> SWidgetUELauncher::GetAllLaunchParams()const
{
	TArray<FString> resault;

	SScrollBox* ScrollboxWidget = &*SrbWidgetLaunchParams;
	SPanel* ScrollPanelWidget = reinterpret_cast<SPanel*>(&*(GET_VAR_PRIVATE_DATA_MEMBER(ScrollboxWidget, SScrollBox, ScrollPanel)));
	FChildren* ScrollBoxChildren = ScrollPanelWidget->GetChildren();

	if (ScrollBoxChildren->Num() > 0)
	{
		for (int32 index = 0; index < ScrollBoxChildren->Num(); index++)
		{
			SWidget* Widget = &(ScrollBoxChildren->GetChildAt(index).Get());
			SEditableTextBox* ScrollBoxChildTextItem = static_cast<SEditableTextBox*>(Widget);
			if (ScrollBoxChildTextItem)
			{
				FString TextItemString = ScrollBoxChildTextItem->GetText().ToString();
				if (TextItemString.Len() > 0)
				{
					resault.Add(TextItemString);
				}
			}
		}
	}

	return resault;
}

FString SWidgetUELauncher::CombineAllLaunchParams(const TArray<FString>& pAllParams)const
{
	FString resault;

	for (auto index : pAllParams)
	{
		resault.Append(" ");
		resault.Append(index);
	}

	return resault;
}

bool SWidgetUELauncher::GetUseCmdEngine()const
{
	return bUseCmdEngine;
}

TSharedRef<SEditableTextBox> SWidgetUELauncher::CreateEditableTextBox(const FString& TextContent)
{
	return SNew(SEditableTextBox)
		.HintText(LOCTEXT("LaunchParam_0", "Please input Launch paramater."))
		.Text(FText::FromString(TextContent));
}

void SWidgetUELauncher::AddParamTextBoxToSlot(const FString& TextContent)
{
	SrbWidgetLaunchParams->AddSlot()
		.Padding(0.0f, 3.0f)
		[
			CreateEditableTextBox(TextContent)
		];
	SrbWidgetLaunchParams->ScrollToEnd();
	SrbWidgetMain->ScrollToEnd();
}
void SWidgetUELauncher::UpdateEngineSelector(const TMap<FString, FString>& EngineMap, FString DefaultEngine)
{
	// initialize SComboBox
	{
		bool bUseDefaultEngine=false;
		int32 DefaultIndex = 0;
		RegisterEngineMap = EngineMap;
		for (const FString& EnginePath : GetAllRegistedEngineList(RegisterEngineMap))
		{
			int32 index=SelectorInstalledEngineList.Add(MakeShareable(new FString(EnginePath)));
			if (!bUseDefaultEngine)
			{
				bUseDefaultEngine = !DefaultEngine.IsEmpty() && EnginePath.Equals(DefaultEngine);
				DefaultIndex = index;
			}
		}

		CmbSelectCurrentEngine = SelectorInstalledEngineList[DefaultIndex];

		CmdWidgetEngineSelector->RefreshOptions();
		CmdWidgetEngineSelector->SetSelectedItem(CmbSelectCurrentEngine);
		//UpdatePlatfromSelector(*CmbSelectCurrentEngine);
	}
}
void SWidgetUELauncher::UpdatePlatfromSelector(const FString& EnginePath,FString DefaultPlatfrom)
{

#define EXECUTABLE_FORMAT TEXT(".exe")
	int32 DefaultPlatfromIndex=0;
	FString ue4editor(TEXT("UE4Editor"));
	ue4editor.Append(EXECUTABLE_FORMAT);
	SelectorPlatfromList.Empty();
	TArray<TSharedPtr<FString>> Platfroms = {
		MakeShareable(new FString(TEXT("Win64"))),
		MakeShareable(new FString(TEXT("Win32")))
	};

	for (const auto& PlatfromItem : Platfroms)
	{
		
		if (FPaths::FileExists(EnginePath+TEXT("/Engine/Binaries/")+*PlatfromItem+TEXT("/")+ue4editor))
		{
			int32 Index = SelectorPlatfromList.AddUnique(PlatfromItem);
			if (!DefaultPlatfrom.IsEmpty() && PlatfromItem->Equals(DefaultPlatfrom))
			{
				DefaultPlatfromIndex = Index;
			}
		}

	}
	if (SelectorPlatfromList.Num() > 0)
	{
		CmbSelectCurrentPlatfrom = SelectorPlatfromList[DefaultPlatfromIndex];

		CmdWidgetPlatfromSelector->RefreshOptions();
		CmdWidgetPlatfromSelector->SetSelectedItem(CmbSelectCurrentPlatfrom);
	}

#undef EXECUTABLE_FORMAT
}

void SWidgetUELauncher::UpdateSelectedPlatfrom(const FString& Platfrom)
{
	UpdatePlatfromSelector(GetSelectedEngineBinPath(),Platfrom);

}
void SWidgetUELauncher::UpdateOpenVSButton(const FString& EnginePath)
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

void SWidgetUELauncher::UpdateUseCmdEngine(bool pUseCmd)
{
	bUseCmdEngine = pUseCmd;
	ECheckBoxState local_IsChecked = GetUseCmdEngine() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	CbUseCmdEngine->SetIsChecked(local_IsChecked);
}

void SWidgetUELauncher::UpdateLaunchParams(const TArray<FString>& pParamsArray)
{
	if (!!pParamsArray.Num())
		SrbWidgetLaunchParams->ClearChildren();
	for (const auto& ParamItem : pParamsArray)
	{
		if (!ParamItem.IsEmpty())
		{
			AddParamTextBoxToSlot(ParamItem);
		}
	}

}

void SWidgetUELauncher::UpdateSelectedProject(const FString& ProjectPath)
{
	OpenProjectFilePath = ProjectPath;
}


void SWidgetUELauncher::EngineLauncher(const FString& EnginePath, const FString& Params)const
{
	FPlatformProcess::CreateProc(*EnginePath, *Params, true, false, false, NULL, NULL, NULL, NULL, NULL);
}


void SWidgetUELauncher::OnProjectFileTextBoxChanged(const FText& NewText)
{
	OpenProjectFilePath = NewText.ToString();
}

TSharedRef<SWidget> MakeWidgetUELauncher()
{
	extern TOptional<FSlateRenderTransform> GetTestRenderTransform();
	extern FVector2D GetTestRenderTransformPivot();
	return
		SNew(SWidgetUELauncher)
		.RenderTransform_Static(&GetTestRenderTransform)
		.RenderTransformPivot_Static(&GetTestRenderTransformPivot);
}

TMap<FString, FString> GetAllRegistedEngineMap()
{
	TMap<FString, FString> resault;
	FDesktopPlatformModule::Get()->EnumerateEngineInstallations(resault);

	return resault;
}

TArray<FString> GetAllRegistedEngineList(const TMap<FString, FString>& pEngineMap)
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

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE
