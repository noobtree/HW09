// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystems/LocalWidgetManager.h"

void ULocalWidgetManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

inline ULocalWidgetManager* ULocalWidgetManager::Get(const UObject* worldContextObject)
{
	if (worldContextObject == nullptr)
	{
		return nullptr;
	}

	UWorld* world = worldContextObject->GetWorld();
	if (world != nullptr)
	{
		APlayerController* firstPlayerController = world->GetFirstPlayerController();
		if (firstPlayerController != nullptr)
		{
			ULocalPlayer* firstLocalPlayer = firstPlayerController->GetLocalPlayer();
			if (firstLocalPlayer != nullptr)
			{
				return firstLocalPlayer->GetSubsystem<ULocalWidgetManager>();
			}
		}
	}

	return nullptr;
}

UUserWidget* ULocalWidgetManager::FindWidget(const FName& widgetName)
{
	// 키에 해당하는 Widget 정보가 존재하는지 확인
	if (widgetMap.Contains(widgetName) == true)
	{
		UUserWidget* widgetInstance = widgetMap[widgetName];
		return widgetInstance;
	}
	return nullptr;
}

UUserWidget* ULocalWidgetManager::AddWidget(const FName& widgetName, const TSubclassOf<UUserWidget>& widgetClass)
{
	// 생성하려는 Widget의 Class 유효성 확인
	if (IsValid(widgetClass) == false)
	{
		return nullptr;
	}

	// 동일 키로 등록된 Instance 찾기
	UUserWidget* widgetInstance = FindWidget(widgetName);
	
	// 검색된 Instance 유효성 검사
	if (IsValid(widgetInstance) == false)
	{
		// 로컬 플레이어 객체 유효성 검사
		ULocalPlayer* localPlayer = GetLocalPlayer();
		if (IsValid(localPlayer) == false)
		{
			// Widget 생성 불가
			return nullptr;
		}

		// Instance 생성
		widgetInstance = CreateWidget<UUserWidget>(localPlayer->PlayerController, widgetClass);

		// 생성된 Instance nullptr 검사
		if (widgetInstance == nullptr)
		{
			return nullptr;
		}

		// Map 등록
		widgetMap.Add(widgetName, widgetInstance);
	}

	// 검색된 Instance와 생성하려는 Widget Class 비교
	if (widgetInstance->IsA(widgetClass) == false)
	{
		return nullptr;
	}

	// Instance 반환
	return widgetInstance;
}

bool ULocalWidgetManager::AddWidgetInstance(const FName& widgetName, UUserWidget* widgetInstance)
{
	// 등록하려는 Instance nullptr 검사
	if (widgetInstance == nullptr)
	{
		return false;
	}

	// 동일 키로 등록된 Instance 찾기
	UUserWidget* existedInstance = FindWidget(widgetName);

	// 기존 Instance의 유효성 검사
	if (IsValid(existedInstance) == true)
	{
		// 기존 Instance와 신규 Instance 동일성 비교
		return existedInstance == widgetInstance;
	}

	// 새로운 Instance 등록
	widgetMap.Add(widgetName, widgetInstance);
	return true;
}

bool ULocalWidgetManager::RemoveWidget(const FName& widgetName)
{
	// Map에서 Key 값 제거 및 Instance얻기
	TObjectPtr<UUserWidget> widgetInstance;
	if (widgetMap.RemoveAndCopyValue(widgetName, widgetInstance) == true)
	{
		// Instance의 유효성 확인
		if (IsValid(widgetInstance) == true)
		{
			// 화면에서 Widget Instance 제거
			widgetInstance->RemoveFromParent();
		}
	}
	return true;
}

void ULocalWidgetManager::SetWidgetHiddenInGame(const FName& widgetName, bool bNewHidden)
{
	// 동일 키로 등록된 Instance 얻기
	UUserWidget* widgetInstance = FindWidget(widgetName);

	// Instance 유효성 검사
	if (IsValid(widgetInstance) == true)
	{
		// Visibility 속성 변경
		widgetInstance->SetVisibility(bNewHidden == true ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}
	return;
}

void ULocalWidgetManager::ClearWidgetInGame()
{
	// WidgetMap에 등록된 모든 Pair 조회
	for (auto pair : widgetMap)
	{
		// 생성된 Widget 인스턴스의 유효성 검사
		if (IsValid(pair.Value) == true)
		{
			// 화면에서 Widget 제거
			pair.Value->RemoveFromParent();
		}
	}

	// Key-Value 목록 초기화
	widgetMap.Empty();
	return;
}
