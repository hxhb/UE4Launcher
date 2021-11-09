// Fill out your copyright notice in the Description page of Project Settings.

#include "SlateWidget/SEditableBoxWraper.h"
#include "SlateOptMacros.h"
//
#define LOCTEXT_NAMESPACE "EditableBoxWraper"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SEditableBoxWraper::Construct(const FArguments& InArgs)
{
	OnDeleteSelgClicked = InArgs._OnDeleteClicked;
	ChildSlot
		[
			SAssignNew(HorzontaBox, SHorizontalBox)
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SAssignNew(EditableTextBox,SEditableTextBox)
				.Text(InArgs._EditableText)
				.HintText(InArgs._EditableHintText)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.Text(InArgs._BtnClearText)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.OnClicked(this, &SEditableBoxWraper::OnClickEventClearText)
			]
			// open uproject file button
			+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.Text(InArgs._BtnDeleteText)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.OnClicked(this,&SEditableBoxWraper::OnClickEventDeleteSelf)
				]
	];
	
}

FReply SEditableBoxWraper::OnClickEventClearText()
{
	EditableTextBox->SetText(FText::FromString(TEXT("")));
	return FReply::Handled();
}
FReply SEditableBoxWraper::OnClickEventDeleteSelf()
{
	EditableTextBox->SetText(FText::FromString(TEXT("")));
	this->SetVisibility(EVisibility::Collapsed);
	return FReply::Handled();
}

void SEditableBoxWraper::SetText(const FText& NewText)
{
	EditableTextBox->SetText(NewText);
}
FText SEditableBoxWraper::GetEditableBoxText()const
{
	return EditableTextBox->GetText();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE