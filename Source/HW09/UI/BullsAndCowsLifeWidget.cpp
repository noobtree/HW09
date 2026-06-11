// Fill out your copyright notice in the Description page of Project Settings.


#include "BullsAndCowsLifeWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/LifePointComponent.h"

void UBullsAndCowsLifeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UCanvasPanelSlot* widgetSlot = Cast<UCanvasPanelSlot>(Slot);
	if (IsValid(widgetSlot) == true)
	{
		widgetSlot->SetAnchors(FAnchors(0.f, 0.0f, 0.0f, 0.0f));
		widgetSlot->SetAlignment(FVector2D(0.0f, 0.0f));
		widgetSlot->SetPosition(FVector2D(80, 45));
		widgetSlot->SetAutoSize(true);
	}

	APlayerController* owningController = GetOwningPlayer();
	if (IsValid(owningController) == true)
	{
		ULifePointComponent* component = owningController->FindComponentByClass<ULifePointComponent>();
		if (IsValid(component) == true)
		{
			component->OnGuessCountChanged.AddDynamic(this, &UBullsAndCowsLifeWidget::OnGuessCountChanged);
		}
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

		InitializeLifePointImageWidget(lifePoint);
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

void UBullsAndCowsLifeWidget::InitializeLifePointImageWidget(UImage* lifePointWidget)
{
	lifePointWidget->SetVisibility(ESlateVisibility::Visible);
	lifePointWidget->SetColorAndOpacity(FLinearColor::Black);

	FSlateBrush imageBrush = lifePointWidget->GetBrush();
	imageBrush.DrawAs = ESlateBrushDrawType::RoundedBox;
	imageBrush.OutlineSettings.Color = FSlateColor(FLinearColor::White);
	imageBrush.OutlineSettings.Width = 0.5f;
	imageBrush.SetImageSize(FVector2D(45, 45));

	lifePointWidget->SetBrush(imageBrush);
}

void UBullsAndCowsLifeWidget::OnGuessCountChanged(const int32& remainGuessCount, const int32& maxGuessCount)
{
	// 현재 LifePoint 목록 얻기
	TArray<UWidget*> children = lifeHorizontalBox->GetAllChildren();

	for (int32 i = 0; i + remainGuessCount < children.Num(); ++i)
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
