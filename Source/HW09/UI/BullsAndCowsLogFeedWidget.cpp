// Fill out your copyright notice in the Description page of Project Settings.


#include "BullsAndCowsLogFeedWidget.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "BullsAndCowsLogWidget.h"

void UBullsAndCowsLogFeedWidget::AddBullsAndCowsLog(const FString& logText, const int32& bullCount, const int32& cowCount)
{
	// Widget 생성
	UUserWidget* logWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), logWidgetClass);

	// ListView에 Widget 추가
	logScrollBox->AddChild(logWidget);

	// Cast
	UBullsAndCowsLogWidget* castedLog = CreateWidget<UBullsAndCowsLogWidget>(GetOwningPlayer());
	if (castedLog != nullptr)
	{
		// log Widget의 속성 값 설정
		castedLog->SetLogText(logText);
		castedLog->SetBullsAndCows(bullCount, cowCount);
	}
}

UPanelSlot* UBullsAndCowsLogFeedWidget::AddChildWidget_Implementation(UUserWidget* widget)
{
	if (IsValid(logScrollBox) == true)
	{
		UPanelSlot* buffer = logScrollBox->AddChild(widget);
		//Cast<UScrollBoxSlot>(buffer)->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		return buffer;
	}
	return nullptr;
}
