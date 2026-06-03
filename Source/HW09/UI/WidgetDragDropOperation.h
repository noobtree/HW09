// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "WidgetDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class HW09_API UWidgetDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	virtual void Dragged_Implementation(const FPointerEvent& PointerEvent) override;

	virtual void Drop_Implementation(const FPointerEvent& PointerEvent) override;

	virtual void DragCancelled_Implementation(const FPointerEvent& PointerEvent) override;

public:
#pragma region Drag & Drop

	// 드래그되는 UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"), Category = "DragDrop")
	UUserWidget* draggedWidget;

	// 드래그 시작 시점의 마우스 좌표와 UI 좌표의 차이
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"), Category = "DragDrop")
	FVector2D dragOffset;

	// Parent Widget before drag started
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"), Category = "DragDrop")
	class UPanelWidget* undoParentWidget;

#pragma endregion

public:
	FVector2D GetDraggedWidgetSize() const;
};
