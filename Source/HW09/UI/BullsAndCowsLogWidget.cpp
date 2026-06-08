// Fill out your copyright notice in the Description page of Project Settings.


#include "BullsAndCowsLogWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"

void UBullsAndCowsLogWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBullsAndCowsLogWidget::SetLogText(const FString& logString)
{
	if (IsValid(logTextBlock) == true)
	{
		logTextBlock->SetText(FText::FromString(logString));
	}
}

void UBullsAndCowsLogWidget::SetBullsAndCows_Implementation(int32 bullsCount, int32 cowsCount)
{
	// Bull 개수를 표시하는 아이콘을 관리하는 Horizontal Box의 모든 자식 Widget 얻기
	TArray<UWidget*> childrenBulls = bullsHorizontalBox->GetAllChildren();

	// 자식 Widget의 개수 검사
	if (childrenBulls.Num() < bullsCount)
	{
		UE_LOG(LogTemp, Error, TEXT("Out Of Range Exception on children Widget in Horizontal Box to visualize Count of Bulls"));
	}
	
	// 불필요한 Widget의 Visibility 변경
	for (int32 i = bullsCount; i < childrenBulls.Num(); ++i)
	{
		childrenBulls[i]->SetVisibility(ESlateVisibility::Collapsed);
	}

	// Bulls Text 변경
	FString bullsString = FString::Printf(TEXT("%d S"), bullsCount);
	bullsTextBlock->SetText(FText::FromString(bullsString));

	// Cow 개수를 표시하는 아이콘을 관리하는 Horizontal Box의 모든 자식 Widget 얻기
	TArray<UWidget*> childrenCows = cowsHorizontalBox->GetAllChildren();

	// 자식 Widget의 개수 검사
	if (childrenCows.Num() < cowsCount)
	{
		UE_LOG(LogTemp, Error, TEXT("Out Of Range Exception on children Widget in Horizontal Box to visualize Count of Cows"));
	}
	
	// 불필요한 Widget의 Visibility 변경
	for (int32 i = cowsCount; i < childrenCows.Num(); ++i)
	{
		childrenCows[i]->SetVisibility(ESlateVisibility::Collapsed);
	}

	// Cows Text 변경
	FString cowsString = FString::Printf(TEXT("%d B"), cowsCount);
	cowsTextBlock->SetText(FText::FromString(cowsString));
}
