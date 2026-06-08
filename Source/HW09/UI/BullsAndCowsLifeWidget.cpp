// Fill out your copyright notice in the Description page of Project Settings.


#include "BullsAndCowsLifeWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

void UBullsAndCowsLifeWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBullsAndCowsLifeWidget::OnLifeCountChanged(int32 newLifeCount)
{
	// 현재 LifePoint 목록 얻기
	TArray<UWidget*> children = lifeHorizontalBox->GetAllChildren();
	
	for (int32 i = 0; i + newLifeCount < children.Num(); ++i)
	{
		UImage* lifePoint = Cast<UImage>(children[i]);
		if (lifePoint == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Widget Type for Inidicating LifePoint is Not UImage"));
			return;
		}

		lifePoint->SetColorAndOpacity(FLinearColor::Red);
	}
}

void UBullsAndCowsLifeWidget::InitializeLifeCount(int32 maxLifeCount)
{
	// 현재 LifePoint 목록 얻기
	TArray<UWidget*> children = lifeHorizontalBox->GetAllChildren();

	// 설정하려는 최대 개수까지 LifePoint 초기화
	for (int32 i = 0; i < maxLifeCount; ++i)
	{
		UImage* lifePoint = nullptr;
		// check validation of index
		if (children.IsValidIndex(i) == true)
		{
			// check validation of element
			if (IsValid(children[i]) == true)
			{
				// set Image Color as Black
				lifePoint = Cast<UImage>(children[i]);
			}
			else
			{
				// Create Widget
				lifePoint = WidgetTree->ConstructWidget<UImage>();

				// add created Widget to Horizontal Box as child
				UHorizontalBoxSlot* widgetSlot = lifeHorizontalBox->AddChildToHorizontalBox(lifePoint);
				widgetSlot->SetPadding(FMargin(4));
			}
		}

		InitializeLifePointWidget(lifePoint);
	}
	
	if (maxLifeCount < children.Num())
	{
		//  초과되는 LifePoint Widget 제거
		for (int32 i = children.Num() - 1; i >= maxLifeCount ; --i)
		{
			children[i]->RemoveFromParent();
		}
	}
}

void UBullsAndCowsLifeWidget::InitializeLifePointWidget(UImage* lifePointWidget)
{
	lifePointWidget->SetVisibility(ESlateVisibility::Visible);
	lifePointWidget->SetColorAndOpacity(FLinearColor::Black);
	lifePointWidget->Brush.DrawAs = ESlateBrushDrawType::RoundedBox;
	lifePointWidget->Brush.OutlineSettings.Color = FSlateColor(FLinearColor::White);
	lifePointWidget->Brush.OutlineSettings.Width = 0.5f;
	lifePointWidget->Brush.SetImageSize(FVector2D(45, 45));
}
