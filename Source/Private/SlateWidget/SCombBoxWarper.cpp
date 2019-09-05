// Fill out your copyright notice in the Description page of Project Settings.

#include "SlateWidget/SCombBoxWarper.h"
#include "SlateOptMacros.h"
//
#define LOCTEXT_NAMESPACE "CombBoxWraper"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SCombBoxWarper::Construct(const FArguments& InArgs)
{
	OnSelectorItemChanged = InArgs._OnSelectorItemChanged;

	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SAssignNew(SelectorWidget, SComboBox<TSharedPtr<FString>>)
			.OptionsSource(&SelectorTextList)
			.OnSelectionChanged(this, &SCombBoxWarper::HandleSelectionChanged)
			.OnGenerateWidget(this, &SCombBoxWarper::HandleGenerateWidget)
			[
				SNew(STextBlock)
				.Text(this, &SCombBoxWarper::HandleSelectionChangeText)
			]
		]
	];
}

void SCombBoxWarper::HandleSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	CurrentSelectedItem = NewSelection;
	OnSelectorItemChanged.ExecuteIfBound(GetCurretSelectedItem());
}

TSharedRef<SWidget> SCombBoxWarper::HandleGenerateWidget(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock)
		.Text(FText::FromString(*InItem));
}

FText SCombBoxWarper::HandleSelectionChangeText() const
{
	return CurrentSelectedItem.IsValid() ? FText::FromString(*CurrentSelectedItem) : FText::FromString(TEXT("AAAAAAAAAAAAAAAAAAAA"));
}

void SCombBoxWarper::UpdateSelector(const TArray<FString>& pSelectorList,const FString& pSelectedItem)
{
	int32 local_DefaultSelectedItemIndex = 0;
	SelectorTextList.Empty();
	for (const auto& SelectorItem : pSelectorList)
	{
		int Index=SelectorTextList.Add((MakeShareable(new FString(SelectorItem))));
		if (!pSelectedItem.IsEmpty() && SelectorItem.Equals(pSelectedItem))
		{
			local_DefaultSelectedItemIndex = Index;
		}
	}
	if (SelectorTextList.Num() > 0)
	{
		CurrentSelectedItem = SelectorTextList[local_DefaultSelectedItemIndex];
		SelectorWidget->RefreshOptions();
		SelectorWidget->SetSelectedItem(CurrentSelectedItem);
		
	}

}
FString SCombBoxWarper::GetCurretSelectedItem()const
{
	return *CurrentSelectedItem;
}

void SCombBoxWarper::ClearAllSelectItem()
{
	SelectorTextList.Empty();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE