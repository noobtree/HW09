// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatComponent.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerState.h"
#include "SubSystems/LocalWidgetManager.h"
#include "Player/ClientController.h"
#include "UI/WidgetAcceptable.h"
#include "UI/ChatWidget.h"

void UChatComponent::InitializeControlWidget()
{
	// UI Manager 얻기
	ULocalWidgetManager* widgetManager = ULocalWidgetManager::Get(this);

	// MainCanvas UI 찾기
	UUserWidget* mainCanvas = widgetManager->FindWidget(FName("MainCanvas"));

	if (mainCanvas == nullptr)
	{
		return;
	}

	// 채팅 UI 추가
	UUserWidget* widgetInstance = widgetManager->AddWidget(FName("Chat"), chatWidgetClass);

	// MainCanvas에 ChatWidget 추가
	IWidgetAcceptable::Execute_AddChildWidget(mainCanvas, widgetInstance);

	// 채팅 UI에 이벤트 등록
	UChatWidget* chatWidget = Cast<UChatWidget>(widgetInstance);
	if (chatWidget != nullptr)
	{
		chatWidget->onMessageCommitted.AddDynamic(this, &UChatComponent::PushMessage);
	}
}

void UChatComponent::PushMessage(const FString& message)
{
	UWorld* world = GetWorld();
	if (IsValid(world) == false)
	{
		return;
	}

	APlayerController* controller = world->GetFirstPlayerController();
	if (IsValid(controller) == false)
	{
		return;
	}

	if (controller->IsLocalPlayerController() == true)
	{
		Server_OnPushMessage(controller, message);
	}
}

void UChatComponent::Server_OnPushMessage_Implementation(const APlayerController* caller, const FString& messageString)
{
	// Client 환경에서 실행 방지를 위한 Authority 확인
	if (GetOwner()->HasAuthority() == false)
	{
		return;
	}

	// 전송자 이름 얻기
	if (IsValid(caller->PlayerState) == false)
	{
		return;
	}
	FString sender = caller->PlayerState->GetPlayerName();

	// World에 배치된 모든 Client의 Controller 얻기
	for (TActorIterator<AClientController> it = TActorIterator<AClientController>(GetWorld()); it; ++it)
	{
		// Client의 Controller 유효성 검사
		AClientController* client = *it;
		if (IsValid(client) == true)
		{
			// 메세지 수신 함수 실행
			client->Client_PullMessage(sender, messageString);
		}
	}
}
