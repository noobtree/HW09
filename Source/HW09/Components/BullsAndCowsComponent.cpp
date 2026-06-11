// Fill out your copyright notice in the Description page of Project Settings.


#include "BullsAndCowsComponent.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "GameMode/BullsAndCowsGameModeBase.h"
#include "GameMode/BullsAndCowsGameStateBase.h"
#include "SubSystems/LocalWidgetManager.h"
#include "UI/WidgetAcceptable.h"
#include "UI/BullsAndCowsNumberPadWidget.h"

void UBullsAndCowsComponent::InitializeControlWidget()
{
	// UI Manager 얻기
	ULocalWidgetManager* widgetManager = ULocalWidgetManager::Get(this);

	// MainCanvas UI 찾기
	UUserWidget* mainCanvas = widgetManager->FindWidget(FName("MainCanvas"));

	if (mainCanvas == nullptr)
	{
		return;
	}

	// UI 추가
	UUserWidget* numberPadWidget = widgetManager->AddWidget(FName("NumberPad"), numberpadWidgetClass);
	UUserWidget* logFeedWidget = widgetManager->AddWidget(FName("LogFeed"), logFeedWidgetClass);

	// MainCanvas에 UI추가
	IWidgetAcceptable::Execute_AddChildWidget(mainCanvas, numberPadWidget);
	IWidgetAcceptable::Execute_AddChildWidget(mainCanvas, logFeedWidget);

	// NumberPad Widget의 이벤트 연결
	UBullsAndCowsNumberPadWidget* numberPad = Cast<UBullsAndCowsNumberPadWidget>(numberPadWidget);
	if (IsValid(numberPad) == true)
	{
		numberPad->OnBullsAndCowsCommitted.AddDynamic(this, &UBullsAndCowsComponent::CommitBullsAndCowsGuess);
	}
}

void UBullsAndCowsComponent::CommitBullsAndCowsGuess(const FString& guessString)
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
		Server_OnCommitBullsAndCowsGuess(controller, guessString);
	}
}

void UBullsAndCowsComponent::Server_OnCommitBullsAndCowsGuess_Implementation(const APlayerController* caller, const FString& guessString)
{
	// Client 환경에서 실행 방지를 위한 Authority 확인
	if (GetOwner()->HasAuthority() == false)
	{
		return;
	}

	// GameMode 얻기
	ABullsAndCowsGameModeBase* gamemode = GetWorld()->GetAuthGameMode<ABullsAndCowsGameModeBase>();
	if (IsValid(gamemode) == false)
	{
		return;
	}

	// 입력 문자열을 통해 Bulls And Cows 판별 진행
	gamemode->OnReceivedBullsAndCowsGuess(caller, guessString);
}

