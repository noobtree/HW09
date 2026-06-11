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

public:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UTextBlock> displayTextBlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidgetOptional))
	TObjectPtr<class UButton> commitButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
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

	UFUNCTION(BlueprintCallable)
	void SetLengthLimit(int32 limit);

	UFUNCTION(BlueprintCallable)
	void ClearTextBlock();

protected:
	UFUNCTION(BlueprintCallable)
	void UpdateDisplayText(FString newString);
};
