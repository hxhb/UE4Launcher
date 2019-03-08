#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"
#include "Data/FLaunchConf.h"
#include "Tools/EngineLaunchTools.h"

class SEditableBoxWraper;
class SConfPanel;

/** @return a new Drag and Drop test widget */
TSharedRef<SWidget> MakeWidgetUELauncher();

class SConfPanel :public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SConfPanel) { }
	SLATE_END_ARGS()

public:

	/**
	 * Constructs the widget gallery.
	 *
	 * @param InArgs - Construction arguments.
	 */
	void Construct(const FArguments& InArgs);
	

public:
	// Widget Show Text
	FText GetProjectFileText()const;
	FText GetLaunchEngineBtnText()const;
	FText GetLaunchProjectBtnText()const;

public:
	// Get conf
	FString GetSelectedEnginePath()const;
	// get Selected .uproject file path
	FString GetSelectedProjectPath()const;
	// Get Selected Tool
	FString GetSelectedTool()const;
	// Get All Launch Parameters
	TArray<FString> GetAllLaunchParams()const;
	// Get All Conf
	FLaunchConf GetLaunchConf()const;



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

	// read/write config
	FReply BtnClickEventLoadConfig();
	FReply BtnClickEventSaveConfig();
	FReply BtnClickEventClearConfig();
	// open Developer website
	void HyLinkClickEventOpenDeveloperWebsite();

public:
	// state changed event
	void OnProjectFileTextBoxChanged(const FText& NewText);
public:
	// Update Engine Selector
	void UpdateEngineSelector(const TMap<FString, FString>& pEngineMap, FString DefaultEngine = TEXT(""));
	// Update Tools Selector
	void UpdateToolSelector(const TArray<FString>& ToolsList,const FString& DefaultTool=TEXT(""));
	// Update is show OpenVS Button
	void UpdateOpenVSButton(const FString& EnginePath);
	
	void UpdateSelectedProject(const FString& ProjectPath=TEXT(""));

	void UpdateLaunchParams(const TArray<FString>& pParamsArray = TArray<FString>{});

	// Update ALL
	void UpdateAll(const FLaunchConf& conf);
protected:

	//FReply DeleteParamExitableBoxWidget(TSharedPtr<SEditableBoxWraper> pWidget);
	// Create/Add a Editable Parameter Box.
	TSharedRef<SEditableBoxWraper> CreateEditableTextBox(const FString& TextContent);
	void AddParamTextBoxToSlot(const FString& TextContent=TEXT(""));
public:
	void HandleEngineSelectorChanged(const FString& NewEngine);

private:
	// Main panel scrollbox
	TSharedPtr<SScrollBox> SrbWidgetMain;
	// Selector
	TSharedPtr<class SCombBoxWarper> CmbEngineSelector;
	TSharedPtr<class SCombBoxWarper> CmbToolSelector;
	// Add Launch Parameter
	TSharedPtr<SScrollBox> SrbWidgetLaunchParams;
	// button
	TSharedPtr<SButton> BtnLaunchEngine;
	TSharedPtr<SButton> BtnOpenVS;
	TSharedPtr<SButton> BtnLaunchProject;

	// Button Text
	mutable FString LaunchEngineBtnText{ TEXT("Launch") };
	mutable FString LaunchProjectBtnText{ TEXT("Launch Configuration") };

	// opened .uproject file
	FString OpenProjectFilePath;
private:
	// general data
	TMap<FString, FString> RegisterEngineMap;
	
};

