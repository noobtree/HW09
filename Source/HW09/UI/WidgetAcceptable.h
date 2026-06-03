// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Blueprint/UserWidget.h"
#include "WidgetAcceptable.generated.h"

//class UPanelSlot;
//class UUserWidget;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWidgetAcceptable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HW09_API IWidgetAcceptable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 임의 Widget을 내부 Panel Widget의 자식(Content)으로 추가하는 함수
	UFUNCTION(BlueprintNativeEvent)
	UPanelSlot* AddChildWidget(UUserWidget* widget);
	virtual UPanelSlot* AddChildWidget_Implementation(UUserWidget* widget) = 0;
};
