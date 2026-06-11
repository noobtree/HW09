// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "LocalWidgetManager.generated.h"

/**
 * 
 */
UCLASS()
class HW09_API ULocalWidgetManager : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

protected:
	UPROPERTY(BlueprintReadWrite)
	TMap<FName, TObjectPtr<UUserWidget>> widgetMap;

public:
	UFUNCTION(BlueprintCallable)
	static ULocalWidgetManager* Get(const UObject* worldContextObject);

	// 동일한 이름으로 등록된 Widget의 Instance를 반환하는 함수
	// Instance가 존재하지 않으면 nullptr 반환
	UFUNCTION(BlueprintCallable)
	UUserWidget* FindWidget(const FName& widgetName);

	// 동일한 이름으로 등록된 Widget의 Instance를 반환하는 함수
	// Instance의 클래스가 WidgetClass로부터 파생되지 않으면 nullptr 반환
	// Instance가 존재하지 않으면 WidgetClass를 통해 생성 및 등록 후 Instance 반환
	UFUNCTION(BlueprintCallable)
	UUserWidget* AddWidget(const FName& widgetName, const TSubclassOf<UUserWidget>& widgetClass);

	// 임의 이름으로Widget의 Instance를 등록하는 함수
	UFUNCTION(BlueprintCallable)
	bool AddWidgetInstance(const FName& widgetName, UUserWidget* widgetInstance);

	// 동일한 이름으로 등록된 Widget의 Instance를 제거하는 함수
	UFUNCTION(BlueprintCallable)
	bool RemoveWidget(const FName& widgetName);

	// 임의 이름으로Widget의 Instance의 Visibility를 변경하는 함수
	UFUNCTION(BlueprintCallable)
	void SetWidgetHiddenInGame(const FName& widgetName, bool bNewHidden);

	// 관리되는 전체 Widget Instance를 제거하는 함수
	UFUNCTION(BlueprintCallable)
	void ClearWidgetInGame();
};
