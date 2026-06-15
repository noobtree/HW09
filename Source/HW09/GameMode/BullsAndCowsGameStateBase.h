// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BullsAndCowsGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBullsAndCowsAnswerRevealedSignature, const FString&, RevealedAnswer);

/**
 * 
 */
UCLASS()
class HW09_API ABullsAndCowsGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	ABullsAndCowsGameStateBase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FBullsAndCowsAnswerRevealedSignature OnAnswerRevealed;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, ReplicatedUsing = OnRep_SealedAnswerString)
	FString revealedAnswerString;

protected:
	// 게임이 종료되었는지 판별하는 변수
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Replicated)
	bool bIsMatchEnd;

	// 재시작 투표 개수
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Replicated)
	int32 restartVotedCount;

	// 1라운드 남은 시간
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Replicated)
	int32 remainTime = 30;

public:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BroadcastAnnouncement(const FString& AnnounceMessage);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BroadcastBullsAndCowsGuess(const FString& guessString, const int32& bullCount, const int32& cowCount);

	UFUNCTION(BlueprintCallable)
	void SetBullsAndCowsAnswer(const FString& bullsAndCowsAnswerString);

	UFUNCTION(BlueprintCallable)
	void DecreaseRemainTime(float deltaTime);

	UFUNCTION(BlueprintCallable)
	void SetRestartVoteCount(int32 newVoteCount);

	UFUNCTION(BlueprintCallable)
	void IitializeGameState();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsGameOver() const { return bIsMatchEnd; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetRemainTime() const { return remainTime; }

	UFUNCTION()
	FORCEINLINE FString GetRevealedAnswerString() const { return revealedAnswerString; }

protected:
	UFUNCTION()
	void OnRep_SealedAnswerString();

	UFUNCTION()
	void OnRep_RemainTime();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SetGameTie();
};
