// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "BullsAndCowsLogWidget.generated.h"

/**
 * 
 */
UCLASS()
class HW09_API UBullsAndCowsLogWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:
	// TextBlock indicates what is try for
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> logTextBlock;

	// HorizontalBox that contains circle icon for Cows (Ball)
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UHorizontalBox> cowsHorizontalBox;

	// TextBlock indicates count of Cows (Ball)
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> cowsTextBlock;

	// HorizontalBox that contains circle icon for Bulls (Strike)
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UHorizontalBox> bullsHorizontalBox;

	// TextBlock indicates count of Bulls (Strike)
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> bullsTextBlock;

public:
	UFUNCTION(BlueprintCallable)
	void SetLogText(const FString& logString);

	// Widget을 Bulls And Cows 결과를 표시하도록 설정하는 함수
	UFUNCTION(BlueprintNativeEvent)
	void SetBullsAndCows(int32 bullsCount, int32 cowsCount);
	virtual void SetBullsAndCows_Implementation(int32 bullsCount, int32 cowsCount);
};
