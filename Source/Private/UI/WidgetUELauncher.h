#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"

/** @return a new Drag and Drop test widget */
TSharedRef<SWidget> MakeWidgetUELauncher();

TArray<FString> GetAllRegistedEngine(TMap<FString, FString> &outs);

class SWidgetUELauncher :public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SWidgetUELauncher) { }
	SLATE_END_ARGS()

public:

	/**
	 * Constructs the widget gallery.
	 *
	 * @param InArgs - Construction arguments.
	 */
	void Construct(const FArguments& InArgs);

	// open Developed by imzlp.me
	void OpenAboutMeWebsite();
public:
	// Engine Version Selector Event
	void HandleCmbEngineSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	TSharedRef<SWidget> HandleCmbEngineGenerateWidget(TSharedPtr<FString> InItem);
	FText HandleCmdEngineSelectionChangeText() const;
private:
	// Engine Version
	TSharedPtr<FString>	CmbSelectCurrentEngine;
	TSharedPtr<SComboBox<TSharedPtr<FString>>> CmdWidgetEngineSelector;
	TArray<TSharedPtr<FString> > SelectorInstalledEngineList;

public:
	// Platfrom Selector Event
	void HandleCmbPlatfromSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	TSharedRef<SWidget> HandleCmbPlatfromGenerateWidget(TSharedPtr<FString> InItem);
	FText HandleCmdPlatfromSelectionChangeText() const;

	// Update Platfrom Info from EnginePath
	void UpdatePlatfromSelector(TSharedPtr<FString> EngineChanged);
	TSharedPtr<FString> GetSelectedPlatfrom()const;
private:
	// Engine Version
	TSharedPtr<FString>	CmbSelectCurrentPlatfrom;
	TSharedPtr<SComboBox<TSharedPtr<FString> > > CmdWidgetPlatfromSelector;
	TArray<TSharedPtr<FString> > SelectorInstalledPlatfromList;

public:
	void HandleUseCmdCBStateChanged(ECheckBoxState NewState, bool* CheckBoxThatChanged);
	ECheckBoxState HandleUseCmdCBStateIsChecked(bool* CheckBox) const;
	bool UseCmdEngine()const;
private:
	bool bUseCmdEngine=false;
public:
	// uproject file path
	FText GetProjectFileText()const;

	FText GetLaunchEngineBtnText()const;
	mutable FString LaunchEngineBtnText{TEXT("Launch")};
	FText GetLaunchProjectBtnText()const;
	mutable FString LaunchProjectBtnText{ TEXT("Launch Configuration") };

	// Open uproject file clickd event
	FReply OnOpenProjectFileClicked();
	// open project directory
	FReply OnOpenProjectFileDirClicked();

	// Add Launch parameter
	FReply AddLaunchParamButtonClicked();
	FReply ClearAllLaunchParamsButtonClicked();

	// Launch Project
	FReply ClickEventLaunchProject();
	// Lcunch Engine
	FReply ClickEventLaunchEngine();

	// GetAllParams
	FString GetSelectedEnginePath()const;
	FString GetSelectedProjectPath()const;
	TArray<FString> GetAllParams()const;
	FString CombineAllParams(const TArray<FString>& pAllParams)const;

	void OnProjectFileTextBoxChanged(const FText& NewText);
protected:

	// Slot
	TSharedRef<SWidget> AddParamTextBox();
	void AddParamTextBoxToSlot();

	// Launch Engine
	void EngineLauncher(const FString& EnginePath, const FString& Params)const;
	FString GetCurrentSelectEngine()const;

private:
	// main panel scrollbox
	TSharedPtr<SScrollBox> SrbWidgetMain;
	TMap<FString, FString> RegisterEngineMap;
	// Add Launch Parameter
	TSharedPtr<SScrollBox> SrbWidgetLaunchArgs;

	// open .uproject file
	FString OpenProjectFilePath;

	// button
	TSharedPtr<SButton> BtnLaunchEngine;
	TSharedPtr<SButton> BtnLaunchProject;
};
