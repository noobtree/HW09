// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BullsAndCowsGameModeBase.h"
#include "BullsAndCowsGameStateBase.h"

void ABullsAndCowsGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ABullsAndCowsGameStateBase* gameStateBase = GetGameState<ABullsAndCowsGameStateBase>();
	if (IsValid(gameStateBase) == true, HasAuthority() == true)
	{
		gameStateBase->MulticastRPC_OnClientLogin(TEXT("ABCDE"));
	}
}

FString ABullsAndCowsGameModeBase::GeneratedRandomBullsAndCowsAnswer()
{
	TArray<int32> candidates = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };

	FString randNums;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 randIndex = FMath::RandRange(0, candidates.Num() - 1);
		randNums.AppendInt(candidates[randIndex]);
		candidates.RemoveAt(randIndex);
	}

	return randNums;
}

bool ABullsAndCowsGameModeBase::IsValidBullsAndCows(const FString& guessString)
{
	return false;
}

FString ABullsAndCowsGameModeBase::JudgeBullsAndCows(const FString& answer, const FString& guessString)
{
	return FString();
}
