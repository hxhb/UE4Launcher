// Fill out your copyright notice in the Description page of Project Settings.

#include "SEditableBoxWraper.h"
#include "SlateOptMacros.h"
//
#define LOCTEXT_NAMESPACE "EditableBoxWraper"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SEditableBoxWraper::Construct(const FArguments& InArgs)
{
	
	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		//.AutoWidth()
		.FillWidth(1.0f)
		[
			SNew(SEditableTextBox)
			.HintText(LOCTEXT("SEditableTextBoxHint", "Please select you want launch .uproject file."))


		]
		// open uproject file button
		+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.Text(LOCTEXT("SelectProjectFile", "Select"))
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
			]
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE