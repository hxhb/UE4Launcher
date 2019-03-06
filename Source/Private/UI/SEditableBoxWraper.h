// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Input/Reply.h"
#include "Widgets/SWidget.h"
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */

class SEditableBoxWraper;
DECLARE_DELEGATE_RetVal_OneParam(FReply, FOnDeleteClicked, TSharedPtr<SEditableBoxWraper>)

class SEditableBoxWraper : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEditableBoxWraper)
	{}
	SLATE_ATTRIBUTE(FText, EditableText)
	SLATE_ATTRIBUTE(FText, ButtonText)
	SLATE_EVENT(FOnDeleteClicked, OnDeleteClicked)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
};
