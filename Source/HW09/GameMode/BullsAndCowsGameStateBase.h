// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BullsAndCowsGameStateBase.generated.h"

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

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Replicated, ReplicatedUsing = OnRep_IsGameOver)
	bool bIsGameOver = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> gameResultWidgetClass;
	
public:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BroadcastAnnouncement(const FString& AnnounceMessage);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BroadcastBullsAndCowsGuess(const FString& guessString, const int32& bullCount, const int32& cowCount);

	UFUNCTION(BlueprintCallable)
	void ToggleGameOverState(bool bNewGameOver);

	UFUNCTION()
	FORCEINLINE bool IsGameOver() const { return bIsGameOver; }

protected:
	UFUNCTION()
	void OnRep_IsGameOver();
};
