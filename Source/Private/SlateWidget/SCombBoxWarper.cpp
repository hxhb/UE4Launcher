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

void SCombBoxWarper::UpdateSelector(const TArray<FString>& SelectorList,const FString& SelectedItem)
{
	SelectorTextList.Empty();
	int32 local_DefaultSelectedItemIndex = 0;
	for (const auto& SelectorItem : SelectorList)
	{
		int Index=SelectorTextList.Add((MakeShareable(new FString(SelectorItem))));
		if (!SelectedItem.IsEmpty() && SelectedItem.Equals(SelectedItem))
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
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE