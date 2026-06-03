// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ClientController.h"
#include "Blueprint/UserWidget.h"
#include "SubSystems/LocalWidgetManager.h"
#include "UI/WidgetAcceptable.h"

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
		// Widget 생성
		UUserWidget* mainCanvasWidgetInstance = widgetManager->FindOrAddWidget(mainCanvasWidgetClass);
		UUserWidget* chatWidgetInstance = widgetManager->FindOrAddWidget(chatWidgetClass);

		// 생성된 Widget 유효성 검사
		if ((IsValid(mainCanvasWidgetInstance) & IsValid(chatWidgetInstance)) == true)
		{
			// MainCanvas Widget에 Chat Widget 부착
			IWidgetAcceptable::Execute_AddChildWidget(mainCanvasWidgetInstance, chatWidgetInstance);
		}
	}
}
