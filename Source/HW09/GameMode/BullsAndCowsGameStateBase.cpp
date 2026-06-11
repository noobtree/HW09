// Fill out your copyright notice in the Description page of Project Settings.


#include "BullsAndCowsGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "Player/ClientController.h"
#include "SubSystems/LocalWidgetManager.h"
#include "UI/BullsAndCowsLogFeedWidget.h"

ABullsAndCowsGameStateBase::ABullsAndCowsGameStateBase()
{
	SetReplicates(true);
}

void ABullsAndCowsGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, bIsGameOver);
}

void ABullsAndCowsGameStateBase::ToggleGameOverState(bool bNewGameOver)
{
	// Client 실행 방지를 위한 Authority 확인
	if (HasAuthority() == false)
	{
		return;
	}

	if (bIsGameOver == bNewGameOver)
	{
		return;
	}

	bIsGameOver = bNewGameOver;
}

void ABullsAndCowsGameStateBase::OnRep_IsGameOver()
{
	// 서버 환경 실행 방지를 위한 Authority 확인
	if (HasAuthority() == true)
	{
		return;
	}

	// UI 매니저 얻기
	ULocalWidgetManager* widgetManager = ULocalWidgetManager::Get(this);

	// Player Screen 초기화
	widgetManager->ClearWidgetInGame();

	// 게임 종료 시
	if (bIsGameOver == true)
	{
		// 게임 결과 UI 생성 및 화면에 출력
		if (IsValid(gameResultWidgetClass) == true)
		{
			UUserWidget* gameResultWidget = widgetManager->AddWidget(FName("GameResult"), gameResultWidgetClass);
			gameResultWidget->AddToPlayerScreen();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("!!    GameOver    !!"));
		}
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
