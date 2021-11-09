#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"
#include "Data/FLaunchConf.h"
#include "Tools/EngineLaunchTools.h"

class SEditableBoxWraper;
class SConfigCard;

DECLARE_DELEGATE_OneParam(FOnConfigCardSelected, SConfigCard*)
DECLARE_DELEGATE_OneParam(FOnConfigFileSelected, FLaunchConf)

class SConfigCard :public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SConfigCard)
	{ }

	SLATE_END_ARGS()

public:

	/**
	 * Constructs the widget gallery.
	 *
	 * @param InArgs - Construction arguments.
	 */
	void Construct(const FArguments& InArgs,const FLaunchConf& Conf);

	void OnHover();
	void OnUnhover();
	FReply OnClicked();
	FOnConfigCardSelected OnConfigCardSelected;
	void Selected();
	void CancelSelected();
	void RemoveFromParent();
	bool IsSelected()const { return bSelected; }
	bool IsRemoved()const { return bIsRemoved; }
	
	FLaunchConf Config;
private:
	bool bSelected = false;
	bool bIsRemoved = false;
	TSharedPtr<SButton> Button;
	TSharedPtr<STextBlock> TextWidget;
};

/** @return a new Drag and Drop test widget */
// TSharedRef<SWidget> MakeWidgetUELauncher();

class SConfigListPanel :public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SConfigListPanel)
	{ }

	SLATE_END_ARGS()

public:

	/**
	 * Constructs the widget gallery.
	 *
	 * @param InArgs - Construction arguments.
	 */
	void Construct(const FArguments& InArgs);

	void AddConfig(const FLaunchConf& Conf);

	void OnConfigSelected(class SConfigCard* Card);
	void HyLinkClickEventOpenVersionWebsite();
	FReply BtnClickEventLoadGlobal();
	FReply BtnClickEventRefresh();
	FReply BtnClickEventRemove();
	void CleanAllConfig();
	void Refresh();
	void ImportGlobalConfig(const FString& ConfigFile);
	void SaveGlobalConfig();
	
	FOnConfigFileSelected OnConfigFileSelected;
private:
	
	TSharedPtr<SVerticalBox> ConfigListWidget;
	TArray<TSharedPtr<SWidget>> ConfigCardsWidget;
};

