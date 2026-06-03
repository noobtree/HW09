// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragableWidget.generated.h"

/**
 * 
 */
UCLASS()
class HW09_API UDragableWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	// 위젯을 드래그 할 수 있는지를 결정하는 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DragableWidget")
	bool bIsDragable;

protected:
	// 드래그를 위하여 마우스 버튼을 눌러야하는 범위를 지정하는 SizeBox
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "DragableWidget")
	TObjectPtr<class USizeBox> dragableSizeBox;
	
	// 드래그하는 동안 마우스를 따라서 화면에 나타나는 위젯의 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DragableWidget")
	TSubclassOf<UUserWidget> dragVisualWidgetClass;
};
