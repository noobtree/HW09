// Fill out your copyright notice in the Description page of Project Settings.


#include "BullsAndCowsGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "Player/ClientController.h"
#include "SubSystems/LocalWidgetManager.h"
#include "UI/BullsAndCowsLogFeedWidget.h"
#include "GameMode/BullsAndCowsGameModeBase.h"

ABullsAndCowsGameStateBase::ABullsAndCowsGameStateBase()
{
	SetReplicates(true);

	IitializeGameState();
}

void ABullsAndCowsGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, bIsMatchEnd);
	DOREPLIFETIME(ThisClass, revealedAnswerString);
	DOREPLIFETIME(ThisClass, remainTime);
	DOREPLIFETIME(ThisClass, restartVotedCount);
}

void ABullsAndCowsGameStateBase::SetBullsAndCowsAnswer(const FString& bullsAndCowsAnswerString)
{
	// 클라이언트 환경 실행 방지를 위한 Authority 확인
	if (HasAuthority() == false)
	{
		return;
	}

	revealedAnswerString = bullsAndCowsAnswerString;
	bIsMatchEnd = bullsAndCowsAnswerString.IsEmpty() ? false : true;
}

void ABullsAndCowsGameStateBase::DecreaseRemainTime(float deltaTime)
{
	if (HasAuthority() == true)
	{
		remainTime -= deltaTime;
	}
}

void ABullsAndCowsGameStateBase::SetRestartVoteCount(int32 newVoteCount)
{
	if (HasAuthority() == true)
	{
		restartVotedCount = newVoteCount;
	}
}

void ABullsAndCowsGameStateBase::IitializeGameState()
{
	// 클라이언트 환경 실행 방지를 위한 Authority 확인
	if (HasAuthority() == false)
	{
		return;
	}

	revealedAnswerString = TEXT("");
	bIsMatchEnd = false;
	restartVotedCount = 0;
	remainTime = 30;

	ABullsAndCowsGameModeBase* gm = Cast<ABullsAndCowsGameModeBase>(AuthorityGameMode);
	if (IsValid(gm) == true)
	{
		gm->StartTurnTimer();
	}
}

void ABullsAndCowsGameStateBase::OnRep_SealedAnswerString()
{
	if (revealedAnswerString.IsEmpty() == false)
	{
		OnAnswerRevealed.Broadcast(revealedAnswerString);
	}
}

void ABullsAndCowsGameStateBase::OnRep_RemainTime()
{
	if (remainTime <= 0)
	{
		Server_SetGameTie();
	}
}

void ABullsAndCowsGameStateBase::Multicast_BroadcastAnnouncement_Implementation(const FString& AnnounceMessage)
{
	// 서버 실행 방지를 위한 Authority 확인
	if (HasAuthority() == true)
	{
		return;
	}

	// 클라이언트의 컨트롤러 얻기
	AClientController* clientController = GetWorld()->GetFirstPlayerController<AClientController>();
	if (IsValid(clientController) == true)
	{
		// 메세지 수신 함수 실행
		clientController->Client_PullMessage(TEXT("Server"), AnnounceMessage);
	}
}

void ABullsAndCowsGameStateBase::Multicast_BroadcastBullsAndCowsGuess_Implementation(const FString& guessString, const int32& bullCount, const int32& cowCount)
{
	// 서버 실행 방지를 위한 Authority 확인
	if (HasAuthority() == true)
	{
		return;
	}

	// UI 매니저 얻기
	ULocalWidgetManager* widgetManager = ULocalWidgetManager::Get(this);
	if (IsValid(widgetManager) == false)
	{
		return;
	}

	// Bulls And Cows Log Feed 얻기
	UBullsAndCowsLogFeedWidget* logFeedWidget = Cast<UBullsAndCowsLogFeedWidget>(widgetManager->FindWidget(FName("LogFeed")));
	if (IsValid(logFeedWidget) == true)
	{
		// Bulls And Cows 추정 결과를 나타내는 UI 추가
		logFeedWidget->AddBullsAndCowsLog(guessString, bullCount, cowCount);
	}
}

void ABullsAndCowsGameStateBase::Server_SetGameTie_Implementation()
{
	if (HasAuthority() == false)
	{
		return;
	}

	ABullsAndCowsGameModeBase* gm = Cast<ABullsAndCowsGameModeBase>(AuthorityGameMode);
	if (IsValid(gm) == true)
	{
		// nullptr를 통해 게임 판정 진행
		gm->OnReceivedBullsAndCowsGuess(nullptr, TEXT(""));
	}
}
