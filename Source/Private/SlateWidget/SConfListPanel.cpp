//// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
//
#include "SlateWidget/SConfigListPanel.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SBoxPanel.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SHeader.h"
#include "Widgets/Text/SMultiLineEditableText.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Input/SHyperlink.h"
#include "Paths.h"

// project files
#include "Widgets/SCanvas.h"
#include "Tools/SerializationTools.h"
#include "Tools/EngineLaunchTools.h"

#define LOCTEXT_NAMESPACE "WidgetConfigListPanel"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SConfigCard::Construct(const FArguments& InArgs,const FLaunchConf& Conf)
{
	Config = Conf;
	FString Args;
	for(const auto& Arg:Config.Params){ Args += TEXT(" ") + Arg; }
	
	FString ConfigText = FString::Printf(TEXT(" Engine: %s\n Tool: %s"),*Config.Engine,*Config.Tool);
	ConfigText += Config.Project.IsEmpty() ? Config.Project: FString::Printf(TEXT("\n Project: %s"),*Conf.Project);
	ConfigText += Args.IsEmpty() ? Args: FString::Printf(TEXT("\n Args: %s"),*Args);
	ChildSlot
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		.Padding(0,5,0,5)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(TextWidget,STextBlock)
			.AutoWrapText(false)
			.Text(FText::AsCultureInvariant(ConfigText))
		]
		+SOverlay::Slot()
		[
			SAssignNew(Button,SButton)
			// .ContentScale(FVector2D{2,1})
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			// .ButtonColorAndOpacity(FLinearColor{1.0,1.0,1.0,0})
			.OnHovered(this,&SConfigCard::OnHover)
			.OnUnhovered(this,&SConfigCard::OnUnhover)
			.OnClicked(this,&SConfigCard::OnClicked)
		]
	];
	Button->SetBorderBackgroundColor(FLinearColor{1.0,1.0,1.0,0});
}

void SConfigCard::OnHover()
{
	if(!IsRemoved())
	{
		Button->SetBorderBackgroundColor(FLinearColor{1.0,1.0,1.0,0.3});
	}
}

void SConfigCard::OnUnhover()
{
	if(!IsSelected() && !IsRemoved())
	{
		Button->SetBorderBackgroundColor(FLinearColor{1.0,1.0,1.0,0});
	}
}

FReply SConfigCard::OnClicked()
{
	if(!IsSelected() && !IsRemoved())
	{
		OnConfigCardSelected.ExecuteIfBound(this);
		Selected();
	}
	return FReply::Handled();
}

void SConfigCard::Selected()
{
	Button->SetBorderBackgroundColor(FLinearColor{1.0,1.0,1.0,0.3});
	bSelected = true;
}

void SConfigCard::CancelSelected()
{
	if(IsSelected() && !IsRemoved())
	{
		Button->SetBorderBackgroundColor(FLinearColor{1.0,1.0,1.0,0});
		bSelected = false;
	}
}

void SConfigCard::RemoveFromParent()
{
	OnConfigCardSelected.Unbind();
	CancelSelected();
	bIsRemoved = true;
}

void SConfigListPanel::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SBorder)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			.Padding(5.0)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.Text(LOCTEXT("Refresh", "Refresh"))
					.HAlign(HAlign_Center)
					.OnClicked(this,&SConfigListPanel::BtnClickEventRefresh)
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.Text(LOCTEXT("RemoveSelected", "Remove"))
					.HAlign(HAlign_Center)
					.OnClicked(this,&SConfigListPanel::BtnClickEventRemove)
				]
				+SHorizontalBox::Slot()
				.FillWidth(1.0)
				[
					SNew(SOverlay)
				]
				+SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Center)
				[
					SNew(SHyperlink)
					.Text(LOCTEXT("LauncherVersion", LAUNCHER_VERSION))
					.OnNavigate(this, &SConfigListPanel::HyLinkClickEventOpenVersionWebsite)
				]
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			.Padding(1.5)
			[
				SNew(SHeader)
				.HAlign(HAlign_Center)
				.Content()
				[
				SNew(STextBlock)
				.Text(LOCTEXT("GlobalConfig", "Global Configuration"))
				]
			]
			+SVerticalBox::Slot()
			.Padding(3.0)
			[
				SNew(SScrollBox)
				+ SScrollBox::Slot()
				.Padding(3.0f)
				[
					SAssignNew(ConfigListWidget,SVerticalBox)
				]
			]
			
		]
	];
	ImportGlobalConfig();
}

void SConfigListPanel::AddConfig(const FLaunchConf& Conf)
{
	if(ConfigListWidget.IsValid())
	{
		TSharedPtr<SWidget> Widget = ConfigListWidget->AddSlot()
		.Padding(1.5)
		.AutoHeight()
		[
			SNew(SConfigCard,Conf)
		].GetWidget();
		SConfigCard* CardIns = (SConfigCard*)(Widget.Get());
		CardIns->OnConfigCardSelected.BindRaw(this,&SConfigListPanel::OnConfigSelected);
		ConfigCardsWidget.Add(Widget);
	}
	SaveGlobalConfig();
}

void SConfigListPanel::OnConfigSelected(SConfigCard* Card)
{
	for(const auto& CardWidget:ConfigCardsWidget)
	{
		SConfigCard* CardIns = (SConfigCard*)CardWidget.Get();
		if(CardWidget.IsValid() && !CardIns->IsRemoved())
		{
			CardIns->CancelSelected();
		}
	}
	Card->Selected();
	OnConfigFileSelected.ExecuteIfBound(Card->Config);
}

void SConfigListPanel::HyLinkClickEventOpenVersionWebsite()
{
	FPlatformProcess::LaunchURL(TEXT("https://imzlp.com/posts/31962/"), NULL, NULL);
}


FReply SConfigListPanel::BtnClickEventRefresh()
{
	Refresh();
	return FReply::Handled();
}

FReply SConfigListPanel::BtnClickEventRemove()
{
	for(int32 index = 0;index < ConfigCardsWidget.Num();)
	{
		SConfigCard* CardIns = (SConfigCard*)(ConfigCardsWidget[index].Get());
		if(CardIns->IsSelected())
		{
			CardIns->RemoveFromParent();
			ConfigListWidget->RemoveSlot(ConfigCardsWidget[index].ToSharedRef());
			ConfigCardsWidget.RemoveAt(index);
			// CardIns->SetVisibility(EVisibility::Collapsed);
		}
		else
		{
			++index;
		}
	}
	SaveGlobalConfig();
	return FReply::Handled();
}

void SConfigListPanel::Refresh()
{
	for(int32 index = 0;index < ConfigCardsWidget.Num();)
	{
		SConfigCard* CardIns = (SConfigCard*)(ConfigCardsWidget[index].Get());
		CardIns->RemoveFromParent();
		ConfigListWidget->RemoveSlot(ConfigCardsWidget[index].ToSharedRef());
		ConfigCardsWidget.RemoveAt(index);
	}
	ImportGlobalConfig();
}

static FString GetGlobalConfigPath()
{
	FString GlobalConfigPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectSavedDir(),TEXT("Global.json")));
	return GlobalConfigPath;
}
void SConfigListPanel::ImportGlobalConfig()
{
	FString JsonStr;
	FFileHelper::LoadFileToString(JsonStr, *GetGlobalConfigPath());
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonStr);
	TSharedPtr<FJsonObject> JsonObject;
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		TArray<TSharedPtr<FJsonValue>> CardGuidJsonValus = JsonObject->GetArrayField(TEXT("GUIDs"));
		for(const auto& CardGuidJsonValue:CardGuidJsonValus)
		{
			FString GUID = CardGuidJsonValue->AsString();
			 TSharedPtr<FJsonObject> CongJsonObject = JsonObject->GetObjectField(GUID);
			if(CongJsonObject.IsValid())
			{
				FLaunchConf Config = SerializationTools::DeSerializationConfByJsonObj(CongJsonObject);
				AddConfig(Config);
			}
		}
	}
}

void SConfigListPanel::SaveGlobalConfig()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TMap<FString,TSharedPtr<FJsonObject>> CardJsonObjectMaps;
	for(const auto& CardWidget:ConfigCardsWidget)
	{
		SConfigCard* CardIns = (SConfigCard*)CardWidget.Get();
		if(CardWidget.IsValid() && !CardIns->IsRemoved())
		{
			TSharedPtr<FJsonObject> CurrentJsonObject = SerializationTools::SerializationConfAsJsonObj(CardIns->Config);
			FString GUID = FGuid::NewGuid().ToString();
			CardJsonObjectMaps.Add(GUID,CurrentJsonObject);
		}
	}

	TArray<FString> Keys;
	CardJsonObjectMaps.GetKeys(Keys);
	TArray<TSharedPtr<FJsonValue>> CardGuidJsonValus;
	for(const auto& Key:Keys)
	{
		CardGuidJsonValus.Add(MakeShareable(new FJsonValueString(Key)));
		JsonObject->SetObjectField(Key,*CardJsonObjectMaps.Find(Key));
	}
	JsonObject->SetArrayField(TEXT("GUIDs"),CardGuidJsonValus);
	
	FString JsonStr;
	auto JsonWriter = TJsonWriterFactory<>::Create(&JsonStr);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);
	
	FFileHelper::SaveStringToFile(JsonStr, *GetGlobalConfigPath());
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE
