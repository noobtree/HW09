// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BullsAndCowsGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class HW09_API ABullsAndCowsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void OnPostLogin(AController* NewPlayer) override;

protected:
	//int32 answer;

public:
	FString GeneratedRandomBullsAndCowsAnswer();

	bool IsValidBullsAndCows(const FString& guessString);

	FString JudgeBullsAndCows(const FString& answer, const FString& guessString);
};
