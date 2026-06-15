// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ClientController.h"
#include "Blueprint/UserWidget.h"
#include "SubSystems/LocalWidgetManager.h"
#include "Components/WidgetControllable.h"
#include "BullsAndCowsPlayerState.h"
#include "GameMode/BullsAndCowsGameModeBase.h"
#include "Components/LifePointComponent.h"

AClientController::AClientController()
{
	SetReplicates(true);
}

void AClientController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	// UI 입력 전용 모드 변경
	FInputModeUIOnly clientInputMode;
	clientInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(clientInputMode);
	SetShowMouseCursor(true);

	// UI 생성
	Client_InitializeWidget();
}

void AClientController::Client_InitializeWidget_Implementation()
{
	if (IsLocalController() == false)
	{
		return;
	}

	// UI 매니저 얻기
	ULocalWidgetManager* widgetManager = GetLocalPlayer()->GetSubsystem<ULocalWidgetManager>();
	if (widgetManager != nullptr)
	{
		// 기존 UI 초기화
		widgetManager->ClearWidgetInGame();

		// MainCanvas Widget 생성
		UUserWidget* mainCanvasWidgetInstance = widgetManager->AddWidget(FName("MainCanvas"), mainCanvasWidgetClass);

		// 화면에 MainCanvas Widget 추가
		mainCanvasWidgetInstance->AddToPlayerScreen();
	}

	// 담당하는 Widget이 있는 컴포넌트 얻기
	TArray<UActorComponent*> widgetControlComponents = GetComponentsByInterface(UWidgetControllable::StaticClass());

	// 각 컴포넌트에서 다루는 Widget 초기화
	for (UActorComponent* component : widgetControlComponents)
	{
		IWidgetControllable* widgetControlComponent = Cast<IWidgetControllable>(component);
		if (widgetControlComponent != nullptr)
		{
			widgetControlComponent->InitializeControlWidget();
		}
	}
}

void AClientController::InitializeClient()
{
	// PlayerState 초기화
	ABullsAndCowsPlayerState* clientState = GetPlayerState<ABullsAndCowsPlayerState>();
	if (IsValid(clientState) == true)
	{
		clientState->InitializePlayerState();
	}

	// Widget 초기화
	Client_InitializeWidget();

	// Replicated 프로퍼티를 위한 서버쪽 초기화
	ULifePointComponent* lifepointComponent = FindComponentByClass<ULifePointComponent>();
	lifepointComponent->InitilizeRemainGuessCount();
}

void AClientController::Client_PullMessage_Implementation(const FString& senderString, const FString& messageString)
{
	// 서버 실행 방지
	if (IsLocalController() == false)
	{
		return;
	}

	// 메시지 수신 이벤트 실행
	if (OnMessageReceived.IsBound() == true)
	{
		OnMessageReceived.Broadcast(senderString, messageString);
	}
}

void AClientController::Server_VoteRestart_Implementation()
{
	if (HasAuthority() == false)
	{
		return;
	}

	ABullsAndCowsGameModeBase* gamemode = Cast<ABullsAndCowsGameModeBase>(GetWorld()->GetAuthGameMode());
	if (IsValid(gamemode) == true)
	{
		gamemode->VoteToRestart(this);
	}
}
