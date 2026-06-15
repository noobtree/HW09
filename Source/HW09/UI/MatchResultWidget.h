// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatchResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class HW09_API UMatchResultWidget : public UUserWidget
{
	GENERATED_BODY()

public :
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> matchResultTextBlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> answerTextBlock;

public:
	UFUNCTION()
	void SetMatchResultString(const FString& resultString);

protected:
	UFUNCTION()
	void OnBullsAndCowsAnswerRevealed(const FString& revealedAnswerString);
};
