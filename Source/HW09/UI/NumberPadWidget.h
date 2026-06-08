// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragableWidget.h"
#include "NumberPadWidget.generated.h"

/**
 * 
 */
UCLASS()
class HW09_API UNumberPadWidget : public UDragableWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UTextBlock> displayTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 lengthLimit;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	FString inputString = "";

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FString GetInputString() const { return inputString; }

	UFUNCTION(BlueprintCallable)
	virtual void OnNumberButtonClicked(int32 number);

	UFUNCTION(BlueprintCallable)
	virtual void OnBackspaceButtonClicked();

};
