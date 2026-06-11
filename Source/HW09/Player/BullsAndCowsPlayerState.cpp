// Fill out your copyright notice in the Description page of Project Settings.


#include "BullsAndCowsPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "GameMode/BullsAndCowsGameModeBase.h"

ABullsAndCowsPlayerState::ABullsAndCowsPlayerState()
{
	SetReplicates(true);
	bIsWinner = false;
}

void ABullsAndCowsPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 프로퍼티 동기화
	DOREPLIFETIME(ThisClass, bIsWinner);
}

void ABullsAndCowsPlayerState::TogglePlayerWinnerState(bool bNewIsWinner)
{
	// Client 실행 방지를 위한 Authority 확인
	if (HasAuthority() == false)
	{
		return;
	}

	if (bIsWinner == bNewIsWinner)
	{
		return;
	}

	bIsWinner = bNewIsWinner;
}

void ABullsAndCowsPlayerState::OnRep_IsWinner() const
{
	if (HasAuthority() == false)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("Winner Has Changed"));
	}
}
