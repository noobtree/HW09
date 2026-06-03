// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetDragDropOperation.h"
#include "Blueprint/UserWidget.h"
#include "Components/PanelWidget.h"
#include "Components/PanelSlot.h"

void UWidgetDragDropOperation::Dragged_Implementation(const FPointerEvent& PointerEvent)
{
	Super::Dragged_Implementation(PointerEvent);

	if (draggedWidget == nullptr)
	{
		return;
	}

	draggedWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UWidgetDragDropOperation::Drop_Implementation(const FPointerEvent& PointerEvent)
{
	Super::Drop_Implementation(PointerEvent);

	if (draggedWidget == nullptr)
	{
		return;
	}

	draggedWidget->SetVisibility(ESlateVisibility::Visible);
}

void UWidgetDragDropOperation::DragCancelled_Implementation(const FPointerEvent& PointerEvent)
{
	Super::DragCancelled_Implementation(PointerEvent);

	if (IsValid(draggedWidget) == false)
	{
		return;
	}

	UPanelSlot* undoSlot = undoParentWidget->AddChild(draggedWidget);
	draggedWidget->SetVisibility(ESlateVisibility::Visible);

	draggedWidget->OnDragCancelled(PointerEvent, this);
}

FVector2D UWidgetDragDropOperation::GetDraggedWidgetSize() const
{
	if (IsValid(draggedWidget) == true)
	{
		return draggedWidget->GetCachedGeometry().GetLocalSize();
	}
	return FVector2D::ZeroVector;
}
