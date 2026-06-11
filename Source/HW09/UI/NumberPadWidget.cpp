// Fill out your copyright notice in the Description page of Project Settings.


#include "NumberPadWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"

void UNumberPadWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 초기 위치 조정
	UCanvasPanelSlot* widgetSlot = Cast<UCanvasPanelSlot>(Slot);
	if (widgetSlot != nullptr)
	{

		widgetSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
		widgetSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		widgetSlot->SetPosition(FVector2D(0, 0));
		widgetSlot->SetAutoSize(true);
	}

	SetLengthLimit(lengthLimit);
}

void UNumberPadWidget::OnNumberButtonClicked(int32 number)
{
	// 입력한 숫자 범위 검사
	if (number >= 0 && number < 10)
	{
		// 문자열 길이 제한 검사
		if (inputString.Len() < lengthLimit)
		{
			// 문자열의 오른쪽 끝에 문자 추가
			inputString.AppendInt(number);

			// TextBlock 업데이트
			UpdateDisplayText(inputString);
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("Text is already fully filled"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Wrong Key value binding : %d"), number);
	return;
}

void UNumberPadWidget::OnBackspaceButtonClicked()
{
	// 문자열 길이 검사
	if (inputString.Len() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Text is empty"));
		return;
	}

	// 문자열의 오른쪽 끝 문자 제거
	inputString.LeftChopInline(1);

	// TextBlock 업데이트
	UpdateDisplayText(inputString);
}

void UNumberPadWidget::SetLengthLimit(int32 limit)
{
	lengthLimit = limit;
	inputString = TEXT("");
	// TextBlock 업데이트
	UpdateDisplayText(inputString);
}

void UNumberPadWidget::ClearTextBlock()
{
	inputString = TEXT("");
	UpdateDisplayText(inputString);
}

void UNumberPadWidget::UpdateDisplayText(FString newString)
{
	// TextBlock 업데이트
	FString displayString = newString.RightPad(lengthLimit);
	displayString.ReplaceCharInline(' ', '_');
	displayTextBlock->SetText(FText::FromString(displayString));
}
