// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetAcceptable.h"
#include "BullsAndCowsLogFeedWidget.generated.h"

/**
 * 
 */
UCLASS()
class HW09_API UBullsAndCowsLogFeedWidget : public UUserWidget, public IWidgetAcceptable
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:
	// 추가되는 Widget의 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UUserWidget> logWidgetClass;

	// Log가 추가되는 Feed를 담당하는 Scroll Box
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UScrollBox> logScrollBox;

public:
	UFUNCTION(BlueprintCallable)
	void AddBullsAndCowsLog(const FString& logText, const int32& bullCount, const int32& cowCount);

	// IWidgetAcceptable을(를) 통해 상속됨
	UPanelSlot* AddChildWidget_Implementation(UUserWidget* widget) override;
};
