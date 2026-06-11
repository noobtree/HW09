// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BullsAndCowsPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class HW09_API ABullsAndCowsPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABullsAndCowsPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Replicated, ReplicatedUsing = OnRep_IsWinner)
	bool bIsWinner;

public:
	UFUNCTION()
	void TogglePlayerWinnerState(bool bNewIsWinner);

protected:
	UFUNCTION()
	void OnRep_IsWinner() const;
};
