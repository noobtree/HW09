// Fill out your copyright notice in the Description page of Project Settings.


#include "BullsAndCowsNumberPadWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UBullsAndCowsNumberPadWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(commitButton) == true)
	{
		commitButton->OnClicked.AddDynamic(this, &UBullsAndCowsNumberPadWidget::OnCommitButtonClicked);
	}

	// 초기 위치 조정
	UCanvasPanelSlot* widgetSlot = Cast<UCanvasPanelSlot>(Slot);
	if (widgetSlot != nullptr)
	{
		widgetSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
		widgetSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		widgetSlot->SetPosition(FVector2D(0, 0));
		widgetSlot->SetAutoSize(true);
	}
}

void UBullsAndCowsNumberPadWidget::OnNumberButtonClicked(int32 number)
{
	// 입력한 숫자 범위 검사
	if (number >= 0 && number < 10)
	{
		// 문자열 길이 제한 검사
		if (inputString.Len() < 4)
		{
			// 문자열의 오른쪽 끝에 문자 추가
			inputString.AppendInt(number);

			// TextBlock 업데이트
			SetDisplayText(inputString);
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("Text is already fully filled"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Wrong Key value binding : %d"), number);
	return;
}

void UBullsAndCowsNumberPadWidget::OnBackspaceButtonClicked()
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
	SetDisplayText(inputString);
}

void UBullsAndCowsNumberPadWidget::SetDisplayText(FString displayString)
{
	while (displayString.Len() < 4)
	{
		displayString.AppendChar('_');
	}

	displayTextBlock->SetText(FText::FromString(displayString));
}

bool UBullsAndCowsNumberPadWidget::IsValidBullsAndCows(const FString& guessString)
{
	// 길이 검사
	if (guessString.Len() != 4)
	{
		return false;
	}

	TSet<TCHAR> buffer;
	for (TCHAR c : guessString)
	{
		// 숫자 문자 검사
		if (FChar::IsDigit(c) == false)
		{
			return false;
		}

		// 문자 보관
		buffer.Add(c);
	}

	// 사용된 문자 종류의 개수 확인
	return buffer.Num() == 4;
}

void UBullsAndCowsNumberPadWidget::OnCommitButtonClicked()
{
	// Bulls And Cows 유효성 검사
	bool bIsValid = IsValidBullsAndCows(inputString);

	if (bIsValid == true)
	{
		// 이벤트 발생
		if (OnBullsAndCowsCommitted.IsBound() == true)
		{
			OnBullsAndCowsCommitted.Broadcast(inputString);
		}

		// Display 문자열 초기화
		inputString = TEXT("");
		SetDisplayText(inputString);
	}
}
