// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ClientController.h"
#include "Blueprint/UserWidget.h"
#include "SubSystems/LocalWidgetManager.h"
#include "Components/WidgetControllable.h"
#include "BullsAndCowsPlayerState.h"

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

	// UI 매니저 얻기
	ULocalWidgetManager* widgetManager = GetLocalPlayer()->GetSubsystem<ULocalWidgetManager>();
	if (widgetManager != nullptr)
	{
		// MainCanvas Widget 생성
		UUserWidget* mainCanvasWidgetInstance = widgetManager->AddWidget(FName("MainCanvas"), mainCanvasWidgetClass);

		// 화면에 MainCanvas Widget 추가<
		mainCanvasWidgetInstance->AddToPlayerScreen();
	}

	TArray<UActorComponent*> widgetControlComponents = GetComponentsByInterface(UWidgetControllable::StaticClass());
	for (UActorComponent* component : widgetControlComponents)
	{
		IWidgetControllable* widgetControlComponent = Cast<IWidgetControllable>(component);
		if (widgetControlComponent != nullptr)
		{
			widgetControlComponent->InitializeControlWidget();
		}
	}
}

void AClientController::InitializeClientController()
{
	ABullsAndCowsPlayerState* clientState = GetPlayerState<ABullsAndCowsPlayerState>();
	if (IsValid(clientState) == true)
	{
		clientState->TogglePlayerWinnerState(false);
	}
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
