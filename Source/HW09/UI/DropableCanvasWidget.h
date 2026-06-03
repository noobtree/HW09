// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetAcceptable.h"
#include "DropableCanvasWidget.generated.h"

/**
 * 
 */
UCLASS()
class HW09_API UDropableCanvasWidget : public UUserWidget, public IWidgetAcceptable
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

protected:
	// 드래그된 UI가 드롭될 수 있는 범위를 지정하는 Canvas Panel
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "Dropable Canvas Widget")
	TObjectPtr<class UCanvasPanel> rootCanvasPanel;

public:
	virtual UPanelSlot* AddChildWidget_Implementation(UUserWidget* widget) override;
};
