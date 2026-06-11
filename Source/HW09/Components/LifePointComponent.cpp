// Fill out your copyright notice in the Description page of Project Settings.


#include "LifePointComponent.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "SubSystems/LocalWidgetManager.h"
#include "UI/WidgetAcceptable.h"

ULifePointComponent::ULifePointComponent()
{
	SetIsReplicatedByDefault(true);
	maxGuessCount = 5;
	remainGuessCount = maxGuessCount;
}

void ULifePointComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// playerNameString 프로퍼티 동기화
	DOREPLIFETIME(ThisClass, remainGuessCount);
	DOREPLIFETIME(ThisClass, maxGuessCount);
}

void ULifePointComponent::InitializeControlWidget()
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
	UUserWidget* widgetInstance = widgetManager->AddWidget(FName("Life"), lifeWidgetClass);

	// MainCanvas에 UI추가
	IWidgetAcceptable::Execute_AddChildWidget(mainCanvas, widgetInstance);
}

void ULifePointComponent::DecreaseRemainGuessCount(int32 deltaGuessCount)
{
	// 클라이언트에서 실행 방지
	if (GetOwner()->HasAuthority() == false)
	{
		return;
	}

	// 변화량 범위 제한
	deltaGuessCount = FMath::Clamp(deltaGuessCount, 0, remainGuessCount);

	if (deltaGuessCount > 0)
	{
		// 남은 Guess 횟수 변경
		remainGuessCount -= deltaGuessCount;
	}
}

void ULifePointComponent::InitilizeRemainGuessCount()
{
	if (GetOwner()->HasAuthority() == true)
	{
		remainGuessCount = maxGuessCount;
	}
}

void ULifePointComponent::OnRep_RemainGuessCount()
{
	// Guess Count 변화 이벤트 실행
	if (OnGuessCountChanged.IsBound() == true)
	{
		OnGuessCountChanged.Broadcast(remainGuessCount, maxGuessCount);
	}
}
