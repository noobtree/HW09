// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ClientController.h"
#include "GameFramework/PlayerState.h"
#include "Blueprint/UserWidget.h"
#include "EngineUtils.h"
#include "SubSystems/LocalWidgetManager.h"
#include "UI/BullsAndCowsMainCanvasWidget.h"
#include "UI/WidgetAcceptable.h"
#include "UI/ChatWidget.h"

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
		UUserWidget* mainCanvasWidgetInstance = widgetManager->FindOrAddWidget(mainCanvasWidgetClass);

		// Chat Widget 이벤트 연결
		UBullsAndCowsMainCanvasWidget* mainCanvas = Cast<UBullsAndCowsMainCanvasWidget>(mainCanvasWidgetInstance);
		//if (IsValid(mainCanvas->chatWidget) == true)
		//{
		//	mainCanvas->chatWidget->onMessageCommitted.AddDynamic(this, &AClientController::ServerRPC_OnMessageCommited);
		//	OnMessageReceived.AddDynamic(mainCanvas->chatWidget, &UChatWidget::OnMessageReceived);
		//}
	}
}

void AClientController::BroadcastReceivedMessage(const FString& sender, const FString& message)
{
	UE_LOG(LogTemp, Display, TEXT("%s : %s"), *sender, *message);
	if (OnMessageReceived.IsBound() == true)
	{
		OnMessageReceived.Broadcast(sender, message);
	}
}

void AClientController::ClientRPC_OnMessageReceived_Implementation(const FString& sender, const FString& message)
{
	BroadcastReceivedMessage(sender, message);
}

void AClientController::ServerRPC_OnMessageCommited_Implementation(const FText& inputText)
{
	// 메시지 전송자의 이름 얻기
	FString sender = PlayerState->GetPlayerName();

	// 전송하려는 메시지를 FString으로 변환
	FString message = inputText.ToString();

	// 서버에 접속중인 다른 PlayerController들에게 메시지 전파
	for (TActorIterator<AClientController> it = TActorIterator<AClientController>(GetWorld()); it; ++it)
	{
		// 클라이언트의 Controller 유효성 검사
		AClientController* remoteController = *it;
		if (IsValid(remoteController) == true)
		{
			// 메시지 수신 이벤트 실행
			remoteController->ClientRPC_OnMessageReceived(sender, message);
		}
	}
}
