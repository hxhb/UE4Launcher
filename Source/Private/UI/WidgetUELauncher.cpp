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
				SNew(SScrollBox)
				+SScrollBox::Slot()
					.Padding(5.0f)
					[
						SNew(SGridPanel)
						.FillColumn(0,1.0f)

						+SGridPanel::Slot(0,1)
						.HAlign(HAlign_Fill)
						.Padding(0.0f,3.0f)
						[
							SNew(SVerticalBox)
							+SVerticalBox::Slot()
								[
									SNew(SHorizontalBox)
									+SHorizontalBox::Slot()
										.HAlign(HAlign_Left)
										[
											SNew(STextBlock)
											.Text(LOCTEXT("SelectEngine", "Select Engine Version:"))
										]
									+SHorizontalBox::Slot()
										.AutoWidth()
										.HAlign(HAlign_Right)
										[
											SNew(STextBlock)
											.Text(LOCTEXT("Developer", "Developed by imzlp.me"))
										]
									
								]
							+SVerticalBox::Slot()
								.AutoHeight()
								.Padding(5.0f)
								[
									SNew(SHorizontalBox)
									+SHorizontalBox::Slot()
										.AutoWidth()
										.HAlign(HAlign_Left)
										[
											SAssignNew(CmdWidgetEngineSelector, SComboBox<TSharedPtr<FString>>)
											.OptionsSource(&SelectorInstalledEngineList)
											.OnSelectionChanged(this, &SWidgetUELauncher::HandleCmbEngineSelectionChanged)
											.OnGenerateWidget(this, &SWidgetUELauncher::HandleCmbGenerateWidget)
											[
												SNew(STextBlock)
													.Text(this, &SWidgetUELauncher::HandleCmdEngineSelectionChangeText)
											]
										]
									+ SHorizontalBox::Slot()
										.HAlign(HAlign_Left)
										.AutoWidth()
										[
											SAssignNew(BtnLaunchEngine,SButton)
												//.Text(LOCTEXT("LaunchEngine", "Launch"))
												.Text(this,&SWidgetUELauncher::GetLaunchEngineBtnText)
												.OnClicked(this, &SWidgetUELauncher::ClickEventLaunchEngine)
										]
								]
						]

						// open project location
						+SGridPanel::Slot(0,2)
							.HAlign(HAlign_Left)
							.Padding(0.0f,3.0f)
							[
								SNew(SVerticalBox)
								+SVerticalBox::Slot()
									[
										SNew(STextBlock)
										.Text(LOCTEXT("UE Project File", "Select your .uproject file:"))
									]
								+SVerticalBox::Slot()
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
												.Text(this,&SWidgetUELauncher::GetProjectFileText)
												.HintText(LOCTEXT("SEditableTextBoxHint", "Please select you want launch .uproject file."))
											]
										// open uproject file button
										+SHorizontalBox::Slot()
											//.FillWidth(0.3f)
											.AutoWidth()
											[
												SNew(SButton)
												.Text(LOCTEXT("AddBreadCrumbLabel", "Open"))
												.HAlign(HAlign_Center)
												.VAlign(VAlign_Center)
												.OnClicked(this, &SWidgetUELauncher::OnOpenProjectFileClicked)
											]

									]

							]

						// input launch params
						+ SGridPanel::Slot(0, 15)
							[
								SNew(SVerticalBox)
								+ SVerticalBox::Slot()
									[
										SNew(STextBlock)
											.Text(LOCTEXT("LauncherParams", "Launcher Parameters:"))
									]
								+ SVerticalBox::Slot()
									.AutoHeight()
									.Padding(3.0f)
									[
										// init create a input box
										SAssignNew(SrbWidgetLaunchArgs, SScrollBox)
										+ SScrollBox::Slot()
											.Padding(0.0f,3.0f)
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
												.OnClicked(this, &SWidgetUELauncher::ClearAllLaunchParamsButtonClicked)
											]
										+ SHorizontalBox::Slot()
											[
												SNew(SButton)
												.Text(LOCTEXT("AddParameter", "Add Parameter"))
												.HAlign(HAlign_Center)
												.OnClicked(this, &SWidgetUELauncher::AddLaunchParamButtonClicked)
											]
									]
							
							]

						// SHeader
						+ SGridPanel::Slot(0, 18)
							.Padding(0.0f, 3.0f,0.0f,3.0f)
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
						+SGridPanel::Slot(0,21)
							[
								SAssignNew(BtnLaunchProject,SButton)
								//.Text(LOCTEXT("LaunchProject","Launch Project"))
								.Text(this,&SWidgetUELauncher::GetLaunchProjectBtnText)
								.HAlign(HAlign_Center)
								.VAlign(VAlign_Center)
								.OnClicked(this,&SWidgetUELauncher::ClickEventLaunchProject)
							]
					]
				]
	];
	// initialize SComboBox
	{
		for (const FString& EnginePath : GetAllRegistedEngine(RegisterEngineMap))
		{
			SelectorInstalledEngineList.Add(MakeShareable(new FString(EnginePath)));
		}
		CmbSelectCurrentEngine = SelectorInstalledEngineList[0];

		CmdWidgetEngineSelector->RefreshOptions();
		CmdWidgetEngineSelector->SetSelectedItem(SelectorInstalledEngineList[0]);
	}

}


void SWidgetUELauncher::HandleCmbEngineSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	CmbSelectCurrentEngine = NewSelection;
}

TSharedRef<SWidget> SWidgetUELauncher::HandleCmbGenerateWidget(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock)
		.Text(FText::FromString(*InItem));
}

FText SWidgetUELauncher::HandleCmdEngineSelectionChangeText() const
{
	return CmbSelectCurrentEngine.IsValid() ? FText::FromString(*CmbSelectCurrentEngine) : FText::GetEmpty();
}

FReply SWidgetUELauncher::OnOpenProjectFileClicked()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

	if (DesktopPlatform)
	{
		TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(SharedThis(this));

		TArray<FString> OpenFilenames;
		const bool bOpened = DesktopPlatform->OpenFileDialog(
			(ParentWindow.IsValid()) ? ParentWindow->GetNativeWindow()->GetOSWindowHandle() : nullptr,
			LOCTEXT("OpenUEProjectDialogTitle", "Open .uproject").ToString(),
			FPaths::GameAgnosticSavedDir(),
			TEXT(""),
			TEXT("UE4 Project (*.uproject)|*.uproject"),
			EFileDialogFlags::None,
			OpenFilenames
		);

		if (OpenFilenames.Num() > 0)
		{
			OpenProjectFilePath = OpenFilenames[0];
		}
	}
	return FReply::Handled();
}

FText SWidgetUELauncher::GetProjectFileText()const
{
	return FText::FromString(OpenProjectFilePath);
}

FReply SWidgetUELauncher::AddLaunchParamButtonClicked()
{
	AddParamTextBoxToSlot();
	return FReply::Handled();
}
FReply SWidgetUELauncher::ClearAllLaunchParamsButtonClicked()
{
	SrbWidgetLaunchArgs->ClearChildren();
	AddParamTextBoxToSlot();
	return FReply::Handled();
}

TSharedRef<SWidget> SWidgetUELauncher::AddParamTextBox()
{
	return SNew(SEditableTextBox)
			.HintText(LOCTEXT("LaunchParam_0", "Please input Launch paramater."));
}

void SWidgetUELauncher::AddParamTextBoxToSlot()
{
	SrbWidgetLaunchArgs->AddSlot()
		.Padding(0.0f, 3.0f)
		[
			AddParamTextBox()
		];
}

FReply SWidgetUELauncher::ClickEventLaunchEngine()
{

	EngineLauncher(GetSelectedEnginePath(), TEXT(""));
	return FReply::Handled();
}

FReply SWidgetUELauncher::ClickEventLaunchProject()
{
	FString EnginePath=GetSelectedEnginePath();
	FString ProjectPath = GetSelectedProjectPath();
	FString AllParams = CombineAllParams(GetAllParams());

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

void SWidgetUELauncher::EngineLauncher(const FString& EnginePath, const FString& Params)const
{
	FPlatformProcess::CreateProc(*EnginePath, *Params, true, false, false, NULL, NULL, NULL, NULL, NULL);
}


FString SWidgetUELauncher::GetCurrentSelectEngine()const
{
	return *CmbSelectCurrentEngine;
}
FString SWidgetUELauncher::GetSelectedEnginePath()const
{
	FString resault(FPaths::Combine(*GetCurrentSelectEngine(), TEXT("Engine/Binaries/Win64/UE4Editor.exe")));
	return resault;
}

#include "../Tools/HackPrivateMember.hpp"
DECL_HACK_PRIVATE_DATA(SScrollBox, TSharedPtr<SScrollPanel>, ScrollPanel)

TArray<FString> SWidgetUELauncher::GetAllParams()const
{
	TArray<FString> resault;

	SScrollBox* ScrollboxWidget = &*SrbWidgetLaunchArgs;
	SPanel* ScrollPanelWidget=reinterpret_cast<SPanel*>(&*(GET_VAR_PRIVATE_DATA_MEMBER(ScrollboxWidget, SScrollBox, ScrollPanel)));
	FChildren* ScrollBoxChildren = ScrollPanelWidget->GetChildren();

	if (ScrollBoxChildren->Num()>0)
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

FString SWidgetUELauncher::CombineAllParams(const TArray<FString>& pAllParams)const
{
	FString resault;

	for (auto index : pAllParams)
	{
		resault.Append(" ");
		resault.Append(index);
	}

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
TSharedRef<SWidget> MakeWidgetUELauncher()
{
	extern TOptional<FSlateRenderTransform> GetTestRenderTransform();
	extern FVector2D GetTestRenderTransformPivot();
	return
		SNew(SWidgetUELauncher)
		.RenderTransform_Static(&GetTestRenderTransform)
		.RenderTransformPivot_Static(&GetTestRenderTransformPivot);
}



TArray<FString> GetAllRegistedEngine(TMap<FString,FString> &outs)
{
	FDesktopPlatformModule::Get()->EnumerateEngineInstallations(outs);
	TArray<FString> EngineAllKey;
	outs.GetKeys(EngineAllKey);

	TArray<FString> resault;

	for (FString& Key : EngineAllKey)
	{
		if (outs.Find(Key))
		{
			resault.Add(*outs.Find(Key));
		}
	}
	return resault;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE
