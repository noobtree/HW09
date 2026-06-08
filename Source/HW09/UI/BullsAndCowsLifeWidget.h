// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BullsAndCowsLifeWidget.generated.h"

/**
 * 
 */
UCLASS()
class HW09_API UBullsAndCowsLifeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UHorizontalBox> lifeHorizontalBox;

public:
	UFUNCTION(BlueprintCallable)
	void OnLifeCountChanged(int32 newLifeCount);

protected:
	UFUNCTION(BlueprintCallable)
	void InitializeLifeCount(int32 maxLifeCount);

	UFUNCTION(BlueprintCallable)
	void InitializeLifePointWidget(class UImage* lifePointWidget);
};
