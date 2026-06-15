// Fill out your copyright notice in the Description page of Project Settings.


#include "BullsAndCowsPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "GameMode/BullsAndCowsGameModeBase.h"
#include "GameMode/BullsAndCowsGameStateBase.h"
#include "SubSystems/LocalWidgetManager.h"
#include "UI/MatchResultWidget.h"

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
	DOREPLIFETIME(ThisClass, matchResult);
}

void ABullsAndCowsPlayerState::SetMatchPlayerResult(EMatchResult playerResult)
{
	if (HasAuthority() == true)
	{
		matchResult = playerResult;
	}
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

void ABullsAndCowsPlayerState::InitializePlayerState()
{
	if (HasAuthority() == false)
	{
		return;
	}

	matchResult = EMatchResult::NotFixed;
}

void ABullsAndCowsPlayerState::OnRep_IsWinner() const
{
	if (HasAuthority() == false)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("Winner Has Changed"));
	}
}

void ABullsAndCowsPlayerState::OnRep_MatchResult()
{
	// 서버 환경 실행 방지
	if (HasAuthority() == true)
	{
		return;
	}
	
	// 게임 결과 확인
	if (matchResult == EMatchResult::NotFixed)
	{
		return;
	}

	// UI Manager 얻기
	ULocalWidgetManager* widgetManager = ULocalWidgetManager::Get(this);
	if (IsValid(widgetManager) == false)
	{
		return;
	}

	// 화면 UI 초기화
	widgetManager->ClearWidgetInGame();

	// Widget 출력
	UUserWidget* widgetInstance = widgetManager->AddWidget(FName("MatchResult"), matchResultWidgetClass);
	widgetInstance->AddToPlayerScreen();

	UMatchResultWidget* matchResultWidget = Cast<UMatchResultWidget>(widgetInstance);
	if (IsValid(matchResultWidget) == true)
	{
		FString resultString = TEXT("");
		switch (matchResult)
		{
		case EMatchResult::Defeat:
			resultString = TEXT("You Lose");
			break;
		case EMatchResult::Tie:
			resultString = TEXT("Draw");
			break;
		case EMatchResult::Victory:
			resultString = TEXT("You Won");
			break;
		default:
			break;
		}

		matchResultWidget->SetMatchResultString(resultString);
	}
}
