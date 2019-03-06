// Fill out your copyright notice in the Description page of Project Settings.

#include "SEditableBoxWraper.h"
#include "SlateOptMacros.h"
//
#define LOCTEXT_NAMESPACE "EditableBoxWraper"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SEditableBoxWraper::Construct(const FArguments& InArgs)
{
	OnDeleteSelgClicked = InArgs._OnDeleteClicked;
	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		//.AutoWidth()
		.FillWidth(1.0f)
		[
			SNew(SEditableTextBox)
			.Text(InArgs._EditableText)
			.HintText(InArgs._EditableHintText)


		]
		// open uproject file button
		+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.Text(InArgs._ButtonText)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.OnClicked(this,&SEditableBoxWraper::OnClickEventDeleteSelf)
			]
	];
	
}

FReply SEditableBoxWraper::OnClickEventDeleteSelf()
{
	OnDeleteSelgClicked.Execute(MakeShareableEditBox(this));
	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedPtr<SEditableBoxWraper> MakeShareableEditBox(SEditableBoxWraper* EditableBoxWraperObj)
{
	return ::MakeShareable(EditableBoxWraperObj);
}
#undef LOCTEXT_NAMESPACE