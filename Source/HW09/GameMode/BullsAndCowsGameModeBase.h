// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BullsAndCowsGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameInitializedSignature);

/**
 * 
 */
UCLASS()
class HW09_API ABullsAndCowsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

protected:
	virtual void OnPostLogin(AController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnGameInitializedSignature OnGameInitialized;

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	FString answer;

	UPROPERTY()
	TArray<TWeakObjectPtr<class AClientController>> connectedClients;

public:
	void OnReceivedBullsAndCowsGuess(const APlayerController* controller, const FString& guessString);

	void InitializeBullsAndCowsGame();

protected:
	// Bulls And Cows의 정답에 해당하는 임의 문자열을 생성하는 함수
	FString GenerateRandomBullsAndCowsAnswer();

	// Bulls And Cows의 정답에 해당하는 임의 문자열과 비교하여 결과를 반환하는 함수
	TPair<int32, int32> JudgeBullsAndCows(const FString& guessString) const;

	void ConsumeGuessCount(const APlayerController* controller);

	bool JudgeIsGameOver(const APlayerController* controller, const int32& bullCount);
};
