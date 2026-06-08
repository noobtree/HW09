// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DropableCanvasWidget.h"
#include "BullsAndCowsMainCanvasWidget.generated.h"

class UChatWidget;
class UNumberPadWidget;

/**
 * 
 */
UCLASS()
class HW09_API UBullsAndCowsMainCanvasWidget : public UDropableCanvasWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
};
