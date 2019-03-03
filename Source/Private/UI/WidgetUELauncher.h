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

	// Select EngineVersion
	void HandleCmbEngineSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	TSharedRef<SWidget> HandleCmbGenerateWidget(TSharedPtr<FString> InItem);
	FText HandleCmdEngineSelectionChangeText() const;
	FText GetProjectFileText()const;

	FText GetLaunchEngineBtnText()const;
	mutable FString LaunchEngineBtnText{TEXT("Launch")};
	FText GetLaunchProjectBtnText()const;
	mutable FString LaunchProjectBtnText{ TEXT("Launch Project") };

	// Open uproject file clickd event
	FReply OnOpenProjectFileClicked();
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

protected:

	// Slot
	TSharedRef<SWidget> AddParamTextBox();
	void AddParamTextBoxToSlot();

	// Launch Engine
	void EngineLauncher(const FString& EnginePath, const FString& Params)const;
	FString GetCurrentSelectEngine()const;

private:
	TMap<FString, FString> RegisterEngineMap;
	// Add Launch Parameter
	TSharedPtr<SScrollBox> SrbWidgetLaunchArgs;
	// Engine Version
	TSharedPtr<FString>	CmbSelectCurrentEngine;
	TSharedPtr<SComboBox<TSharedPtr<FString> > > CmdWidgetEngineSelector;
	TArray<TSharedPtr<FString> > SelectorInstalledEngineList;
	// open .uproject file
	FString OpenProjectFilePath;

	// button
	TSharedPtr<SButton> BtnLaunchEngine;
	TSharedPtr<SButton> BtnLaunchProject;
};
