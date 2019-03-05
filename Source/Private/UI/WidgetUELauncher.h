#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"

/** @return a new Drag and Drop test widget */
TSharedRef<SWidget> MakeWidgetUELauncher();

TArray<FString> GetAllRegistedEngineList(const TMap<FString, FString>& pEngineMap);
TMap<FString, FString> GetAllRegistedEngineMap();

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
public:
	// Engine Version Selector Event
	void HandleCmbEngineSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	TSharedRef<SWidget> HandleCmbEngineGenerateWidget(TSharedPtr<FString> InItem);
	FText HandleCmdEngineSelectionChangeText() const;
	FString GetSelectedEngine()const;
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

private:
	// Engine Version
	TSharedPtr<FString>	CmbSelectCurrentPlatfrom;
	TSharedPtr<SComboBox<TSharedPtr<FString> > > CmdWidgetPlatfromSelector;
	TArray<TSharedPtr<FString> > SelectorPlatfromList;

public:
	void HandleUseCmdCBStateChanged(ECheckBoxState NewState, bool* CheckBoxThatChanged);
	ECheckBoxState HandleUseCmdCBStateIsChecked(bool* CheckBox) const;
	

public:
	// uproject file path
	FText GetProjectFileText()const;
	FText GetLaunchEngineBtnText()const;
	FText GetLaunchProjectBtnText()const;
	// Get All Launch Parameters
	FString GetSelectedEnginePath()const;
	// get Selected .uproject file path
	FString GetSelectedProjectPath()const;
	// Get Selected Platfrom Win32/Win64
	TSharedPtr<FString> GetSelectedPlatfrom()const;
	// UseCmdEngine is Checked?
	bool GetUseCmdEngine()const;
	TArray<FString> GetAllLaunchParams()const;
	FString CombineAllLaunchParams(const TArray<FString>& pAllParams)const;

public:
	// Add/Clear Launch parameter button clicked event
	FReply BtnClickEventAddLaunchParamButton();
	FReply BtnClickEventClearAllLaunchParamsButton();
	// Launch Project
	FReply BtnClickEventLaunchProject();
	// Lcunch Engine
	FReply BtnClickEventLaunchEngine();
	FReply BtnClickEventOpenVS();
	// Open uproject file clickd event
	FReply BtnClickEventOpenProjectFile();
	// open project directory
	FReply BtnClickEventOpenProjectFileDir();

	// open Developer website
	void HyLinkClickEventOpenDeveloperWebsite();

public:
	// state changed event
	void OnProjectFileTextBoxChanged(const FText& NewText);
public:
	// Update Engine Selector
	void UpdateEngineSelector(const TMap<FString, FString>& pEngineMap, FString DefaultEngine = TEXT(""));
	// Updator
	void UpdateOpenVSButton(const FString& EnginePath);
	// Update Platfrom Info from EnginePath
	void UpdatePlatfromSelector(const FString& EnginePath, FString DefaultPlatfrom=TEXT(""));
	// Update Selected Platfrom
	void UpdateSelectedPlatfrom(const FString& Platfrom);
	// Update Use UEEditor-cmd
	void UpdateUseCmdEngine(bool pUseCmd);

	void UpdateLaunchParams(TArray<FString> pParamsArray);
protected:

	// Create/Add a Editable Parameter Box.
	TSharedRef<SEditableTextBox> CreateEditableTextBox(const FString& TextContent);
	void AddParamTextBoxToSlot(const FString& TextContent);

	// Launch Engine
	void EngineLauncher(const FString& EnginePath, const FString& Params)const;


private:
	// Main panel scrollbox
	TSharedPtr<SScrollBox> SrbWidgetMain;
	TMap<FString, FString> RegisterEngineMap;

	// Add Launch Parameter
	TSharedPtr<SScrollBox> SrbWidgetLaunchParams;

	// open .uproject file
	FString OpenProjectFilePath;

	// button
	TSharedPtr<SButton> BtnLaunchEngine;
	TSharedPtr<SButton> BtnOpenVS;
	TSharedPtr<SButton> BtnLaunchProject;

	// UseCmd SCheckedBox
	TSharedPtr<SCheckBox> CbUseCmdEngine;
	// Button Text
	mutable FString LaunchEngineBtnText{ TEXT("Launch") };
	mutable FString LaunchProjectBtnText{ TEXT("Launch Configuration") };

	bool bUseCmdEngine = false;
};
