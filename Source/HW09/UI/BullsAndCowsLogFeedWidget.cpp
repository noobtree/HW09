// Fill out your copyright notice in the Description page of Project Settings.


#include "BullsAndCowsLogFeedWidget.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/CanvasPanelSlot.h"
#include "BullsAndCowsLogWidget.h"

void UBullsAndCowsLogFeedWidget::NativeConstruct()
{
	// 초기 위치 조정
	UCanvasPanelSlot* widgetSlot = Cast<UCanvasPanelSlot>(Slot);
	if (widgetSlot != nullptr)
	{
		widgetSlot->SetAnchors(FAnchors(1.0f, 0.f, 1.f, 0.f));
		widgetSlot->SetAlignment(FVector2D(1, 0));
		widgetSlot->SetPosition(FVector2D(-80, 45));
		widgetSlot->SetAutoSize(true);
	}
}

void UBullsAndCowsLogFeedWidget::AddBullsAndCowsLog(const FString& logText, const int32& bullCount, const int32& cowCount)
{
	// Widget 생성
	UUserWidget* logWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), logWidgetClass);

	// ListView에 Widget 추가
	logScrollBox->AddChild(logWidget);

	// Cast
	UBullsAndCowsLogWidget* castedLog = Cast<UBullsAndCowsLogWidget>(logWidget);
	if (castedLog != nullptr)
	{
		// log Widget의 속성 값 설정
		castedLog->SetLogText(logText);
		castedLog->SetBullsAndCows(bullCount, cowCount);
	}

	logScrollBox->ScrollToEnd();
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
