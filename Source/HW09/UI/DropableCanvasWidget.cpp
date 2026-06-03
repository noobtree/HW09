// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DropableCanvasWidget.h"
#include "UI/WidgetDragDropOperation.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UDropableCanvasWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UDropableCanvasWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool bSuperOnDrop = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UWidgetDragDropOperation* opper = Cast<UWidgetDragDropOperation>(InOperation);
	if (opper == nullptr)
	{
		return bSuperOnDrop;
	}

	if (IsValid(rootCanvasPanel) == false || IsValid(opper->draggedWidget) == false)
	{
		return bSuperOnDrop;
	}

	// 드래그 된 위젯을 캔버스에 추가
	UCanvasPanelSlot* widgetSlot = rootCanvasPanel->AddChildToCanvas(opper->draggedWidget);

	// 드래그 된 위젯의 예상 위치 계산
	const FGeometry& canvasGeometry = rootCanvasPanel->GetCachedGeometry();
	FVector2D dropPosition = canvasGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
	FVector2D widgetPosition = dropPosition - opper->dragOffset;

	// 드래그 된 위젯의 위치 제한
	FVector2D maxPosition = canvasGeometry.GetLocalSize() - opper->GetDraggedWidgetSize();
	widgetSlot->SetDesiredPosition(widgetPosition);
	widgetSlot->SetAutoSize(true);

	return false;
}

UPanelSlot* UDropableCanvasWidget::AddChildWidget_Implementation(UUserWidget* widget)
{
	if (IsValid(rootCanvasPanel) == true)
	{
		return rootCanvasPanel->AddChildToCanvas(widget);
	}
	return nullptr;
}
