// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DragableWidget.h"
#include "UI/WidgetDragDropOperation.h"
#include "Components/SizeBox.h"

void UDragableWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

FReply UDragableWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply superReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (IsValid(dragableSizeBox) == true)
	{
		// 마우스 버튼을 누른 지점 확인
		FVector2D downPosition = InMouseEvent.GetScreenSpacePosition();
		const FGeometry& dragableSizeBoxGeometry = dragableSizeBox->GetCachedGeometry();

		// 지정 범위 내부에서 버튼 입력 시 드래그 감지 시작
		if (dragableSizeBoxGeometry.IsUnderLocation(downPosition) == true)
		{
			TSharedPtr<SWidget> cachedWidget = GetCachedWidget();
			// 드래그 키 = EKeys::LeftMouseButton
			return FReply::Handled().DetectDrag(cachedWidget.ToSharedRef(), EKeys::LeftMouseButton);
		}
	}
	return superReply;
}

void UDragableWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	// 드래그 가능 여부 확인, 드래그 가능한 위젯 컴포넌트 확인
	if (bIsDragable == false || IsValid(dragableSizeBox) == false)
	{
		return;
	}

	// 드래그 드롭을 위한 클래그 인스턴스 생성
	UWidgetDragDropOperation* opper = NewObject<UWidgetDragDropOperation>();

	// 드래그 드롭에 필요한 정보 저장
	opper->draggedWidget = this;
	opper->dragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	opper->undoParentWidget = GetParent();

	opper->DefaultDragVisual = IsValid(dragVisualWidgetClass) == true ? CreateWidget(GetOwningPlayer(), dragVisualWidgetClass) : this;
	SetColorAndOpacity(FLinearColor(1, 1, 1, 0.5f));

	OutOperation = opper;
}

void UDragableWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	SetColorAndOpacity(FLinearColor::White);
}
