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

	// Bulls And Cow 판별
	TPair<int32, int32> result = JudgeBullsAndCows(guessString);

	// GameState를 통해 전체 클라이언트에게 전파
	gamestate->Multicast_BroadcastBullsAndCowsGuess(guessString, result.Key, result.Value);

	// 게임 종료 판정
	bool bIsGameOver = JudgeIsGameOver(controller, result.Key);
	
	if (bIsGameOver == true)
	{
		// 월드에 접속한 모든 클라이언트에게 게임 종료 알림 전파
		FString announceMessage = FString::Printf(TEXT("# Game Set - Winner : "), *controller->PlayerState->GetPlayerName());
		gamestate->Multicast_BroadcastAnnouncement(announceMessage);

		// 게임 종료 상태를 GameState의 프로퍼티에 적용
		gamestate->ToggleGameOverState(bIsGameOver);
	}
}

void ABullsAndCowsGameModeBase::InitializeBullsAndCowsGame()
{
	ABullsAndCowsGameStateBase* gamestate = GetGameState<ABullsAndCowsGameStateBase>();
	if (IsValid(gamestate) == false || gamestate->IsGameOver() == false)
	{
		return;
	}

	// 새로운 정답 생성
	answer = GenerateRandomBullsAndCowsAnswer();
	UE_LOG(LogTemp, Warning, TEXT("BullsAndCows Answer : %s"), *answer);

	// 게임 초기화 이벤트 실행
	if (OnGameInitialized.IsBound() == true)
	{
		OnGameInitialized.Broadcast();
	}

	// GameState에 정의된 게임 상태를 초기 상태로 설정
	gamestate->ToggleGameOverState(false);
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
	// Bull (Strike) 개수 확인
	if (bullCount == 4)
	{
		// 클라이언트 상태를 우승자로 설정
		ABullsAndCowsPlayerState* clientState = controller->GetPlayerState<ABullsAndCowsPlayerState>();
		if (IsValid(clientState) == true)
		{
			clientState->TogglePlayerWinnerState(true);
		}
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
			// 해당 클라이언트를 제외한 다른 클라이언트의 상태를 우승자로 설정
			for (TWeakObjectPtr<AClientController>& client : connectedClients)
			{
				if(client.IsValid() == true && client.Get() != controller)
				{
					ABullsAndCowsPlayerState* otherClientState = client->GetPlayerState<ABullsAndCowsPlayerState>();
					if (IsValid(otherClientState) == true)
					{
						otherClientState->TogglePlayerWinnerState(true);
					}
				}
			}
			
			return true;
		}
	}

	return false;
}
