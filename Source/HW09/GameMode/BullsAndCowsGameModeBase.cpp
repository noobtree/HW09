// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BullsAndCowsGameModeBase.h"
#include "BullsAndCowsGameStateBase.h"
#include "Player/ClientController.h"
#include "Player/BullsAndCowsPlayerState.h"
#include "Components/LifePointComponent.h"

void ABullsAndCowsGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	answer = GenerateRandomBullsAndCowsAnswer();
	UE_LOG(LogTemp, Warning, TEXT("BullsAndCows Answer : %s"), *answer);
}

void ABullsAndCowsGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	// 신규 접속 클라이언트 컨트롤러 유효성 확인
	AClientController* newClientController = Cast<AClientController>(NewPlayer);
	if (IsValid(newClientController) == true)
	{
		// 접속한 클라이언트 목록에 추가
		connectedClients.Add(newClientController);

		// 접속한 클라이언트의 이름 설정
		FString clientName = FString::Printf(TEXT("Client_%d"), connectedClients.Num());
		if (IsValid(newClientController->PlayerState) == true)
		{
			newClientController->PlayerState->SetPlayerName(FString::Printf(TEXT("Client_%d"), connectedClients.Num()));
		}

		// 메시지 설정
		FString message = FString::Printf(TEXT("New Client( %s ) has joined the Game"), *clientName);

		// GameState 얻기
		ABullsAndCowsGameStateBase* gameStateBase = GetGameState<ABullsAndCowsGameStateBase>();
		if (IsValid(gameStateBase) == true)
		{
			// 신규 클라이언트 접속 알림 전파
			gameStateBase->Multicast_BroadcastAnnouncement(message);
		}
	}
}

void ABullsAndCowsGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	// 접속 종료 클라이언트 컨트롤러 유효성 확인
	AClientController* exitClientController = Cast<AClientController>(Exiting);
	if (IsValid(exitClientController) == true)
	{
		// 종료한 클라이언트 목록에 추가
		connectedClients.Remove(exitClientController);

		// 종료한 클라이언트의 이름 얻기
		FString clientName = TEXT("Anonymous");
		if (IsValid(exitClientController->PlayerState) == true)
		{
			clientName = exitClientController->PlayerState->GetPlayerName();
		}

		// 메시지 설정
		FString message = FString::Printf(TEXT("Client( %s ) has left the Game"), *clientName);

		// GameState 얻기
		ABullsAndCowsGameStateBase* gameStateBase = GetGameState<ABullsAndCowsGameStateBase>();
		if (IsValid(gameStateBase) == true)
		{
			// 클라이언트 접속 종료 전파
			gameStateBase->Multicast_BroadcastAnnouncement(message);
		}
	}
}

void ABullsAndCowsGameModeBase::OnReceivedBullsAndCowsGuess(const APlayerController* controller, const FString& guessString)
{
	ABullsAndCowsGameStateBase* gamestate = GetGameState<ABullsAndCowsGameStateBase>();
	if (IsValid(gamestate) == false || gamestate->IsGameOver() == true)
	{
		return;
	}

	// 게임 종료 판정
	bool bIsGameOver = false;
	if (controller == nullptr)
	{
		// 게임 종료 판정
		bIsGameOver = JudgeIsGameOver(controller, -1);
	}
	else
	{
		// Bulls And Cow 판별
		TPair<int32, int32> result = JudgeBullsAndCows(guessString);

		// GameState를 통해 전체 클라이언트에게 전파
		gamestate->Multicast_BroadcastBullsAndCowsGuess(guessString, result.Key, result.Value);

		// 게임 종료 판정
		bIsGameOver = JudgeIsGameOver(controller, result.Key);
	}

	if (bIsGameOver == true)
	{
		// 타이머 중지
		StopTurnTimer();

		// 정답 공개 (Replicated) 및 게임 종료 상태 변경
		gamestate->SetBullsAndCowsAnswer(answer);

		// 월드에 접속한 모든 클라이언트에게 게임 종료 알림 전파
		FString announceMessage = FString::Printf(TEXT("# Game Set - Winner : "), *controller->PlayerState->GetPlayerName());
		gamestate->Multicast_BroadcastAnnouncement(announceMessage);
	}
}

void ABullsAndCowsGameModeBase::InitializeBullsAndCowsGame()
{
	// GameState가 유효하고, 게임이 종료된 상태인지 확인
	ABullsAndCowsGameStateBase* gs = GetGameState<ABullsAndCowsGameStateBase>();
	if (IsValid(gs) == false || gs->IsGameOver() == false)
	{
		return;
	}
	// 재시작 투표자 목록 초기화
	restartVotedClients.Empty();

	// 새로운 정답 생성
	answer = GenerateRandomBullsAndCowsAnswer();
	UE_LOG(LogTemp, Warning, TEXT("BullsAndCows Answer : %s"), *answer);

	gs->IitializeGameState();

	for (TWeakObjectPtr<AClientController>& client : connectedClients)
	{
		if (client.IsValid() == false)
		{
			continue;
		}

		client->InitializeClient();
	}

}

void ABullsAndCowsGameModeBase::VoteToRestart(APlayerController* controller)
{
	// 기존 투표자 확인
	if (restartVotedClients.Contains(controller) == false)
	{
		restartVotedClients.Add(controller);
	}

	// 접속한 클라이언트 수와 같으면 재시작
	if (restartVotedClients.Num() == connectedClients.Num())
	{
		InitializeBullsAndCowsGame();
	}
}

void ABullsAndCowsGameModeBase::StartTurnTimer()
{
	ABullsAndCowsGameStateBase* gs = GetGameState<ABullsAndCowsGameStateBase>();
	if (IsValid(gs) == false || gs->IsGameOver() == true)
	{
		return;
	}

	FTimerDelegate delegator = FTimerDelegate::CreateUObject(gs, &ABullsAndCowsGameStateBase::DecreaseRemainTime, 1.f);

	GetWorldTimerManager().SetTimer(turnTimer, delegator, 1, true, 0);
}

void ABullsAndCowsGameModeBase::StopTurnTimer()
{
	ABullsAndCowsGameStateBase* gs = GetGameState<ABullsAndCowsGameStateBase>();
	if (IsValid(gs) == false)
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(turnTimer);
}

FString ABullsAndCowsGameModeBase::GenerateRandomBullsAndCowsAnswer()
{
	TArray<int32> candidates = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	FString randNums;
	for (int32 i = 0; i < 4; ++i)
	{
		int32 randIndex = FMath::RandRange(0, candidates.Num() - 1);
		randNums.AppendInt(candidates[randIndex]);
		candidates.RemoveAt(randIndex);
	}

	return randNums;
}

TPair<int32, int32> ABullsAndCowsGameModeBase::JudgeBullsAndCows(const FString& guessString) const
{
	int32 bullCount = 0;
	int32 cowCount = 0;

	for (int32 i = 0; i < answer.Len(); ++i)
	{
		if (answer[i] == guessString[i])
		{
			++bullCount;
			continue;
		}

		int32 lowerBound;
		if (answer.FindChar(guessString[i], lowerBound) == true)
		{
			++cowCount;
		}
	}

	return { bullCount, cowCount };
}

void ABullsAndCowsGameModeBase::ConsumeGuessCount(const APlayerController* controller)
{
	ULifePointComponent* lifeComponent = controller->FindComponentByClass<ULifePointComponent>();
	if (IsValid(lifeComponent) == true)
	{
		// 남은 Guess Count 1만큼 감소
		lifeComponent->DecreaseRemainGuessCount(1);
	}
}

bool ABullsAndCowsGameModeBase::JudgeIsGameOver(const APlayerController* controller, const int32& bullCount)
{
	// 잘못된 개수를 입력받는 경우 Draw 판별 (체력 감소 없음)
	if (bullCount < 0)
	{
		// 전체 클라이언트 무승부 처리
		SetWinnerController(nullptr, false);
		return true;
	}

	// Bull (Strike) 개수 확인
	if (bullCount == 4)
	{
		// 해당 컨트롤러 우승 처리
		SetWinnerController(controller, true);

		return true;
	}

	// 클라이언트의 체력 컴포넌트 얻기
	ULifePointComponent* lifeComponent = controller->FindComponentByClass<ULifePointComponent>();
	if (IsValid(lifeComponent) == true)
	{
		// 체력? 감소
		lifeComponent->DecreaseRemainGuessCount(1);

		// 남은 체력 확인
		if (lifeComponent->GetRemainGuessCount() <= 0)
		{
			// 해당 컨트롤러 패배 처리
			SetWinnerController(controller, false);
			
			return true;
		}
	}

	return false;
}

void ABullsAndCowsGameModeBase::SetWinnerController(const APlayerController* controller, bool bisWinner)
{
	for (TWeakObjectPtr<AClientController>& client : connectedClients)
	{
		if (client.IsValid() == false)
		{
			continue;
		}

		ABullsAndCowsPlayerState* otherClientState = client->GetPlayerState<ABullsAndCowsPlayerState>();
		if (IsValid(otherClientState) == false)
		{
			continue;
		}

		if (controller == nullptr)
		{
			otherClientState->SetMatchPlayerResult(EMatchResult::Tie);
		}
		else
		{
			if (client.Get() != controller)
			{
				otherClientState->SetMatchPlayerResult(bisWinner == true ? EMatchResult::Defeat : EMatchResult::Victory);
			}
			else
			{
				otherClientState->SetMatchPlayerResult(bisWinner == true ? EMatchResult::Victory : EMatchResult::Defeat);
			}
		}
	}
}
