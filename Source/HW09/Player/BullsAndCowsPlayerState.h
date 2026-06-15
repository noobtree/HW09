// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BullsAndCowsPlayerState.generated.h"

UENUM()
enum class EMatchResult : int8
{
	NotFixed,	// 아직 판정되지 않은 상태
	Defeat,		// 패배
	Tie,		// 비김
	Victory		// 승리
};

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
	// 게임(승부) 결과
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, ReplicatedUsing = OnRep_MatchResult)
	EMatchResult matchResult;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> matchResultWidgetClass;

public:
	UFUNCTION()
	void SetMatchPlayerResult(EMatchResult playerResult);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FString GetMatchResultString() const { return UEnum::GetValueAsString(matchResult); }

	void InitializePlayerState();

protected:
	UFUNCTION()
	void OnRep_MatchResult();
};
