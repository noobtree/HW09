// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "LocalWidgetManager.generated.h"

class UUserWidget;

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
	TMap<TSubclassOf<UUserWidget>, TObjectPtr<UUserWidget>> widgetMap;

public:
	// 임의 클래스에 해당하는 Widget의 인스턴스를 반환하는 함수
	// 생성될 Widget의 Class가 유효하지 않으면 nullptr 반환
	// Widget의 인스턴스가 존재하지 않으면 생성 후 반환
	UFUNCTION(BlueprintCallable)
	UUserWidget* FindOrAddWidget(const TSubclassOf<UUserWidget>& widgetClass, int32 ZOrder = 0);

	// 임의 클래스에 해당하는 Widget의 Instance를 등록하는 함수
	UFUNCTION(BlueprintCallable)
	bool AddWidget(const TSubclassOf<UUserWidget>& widgetClass, UUserWidget* widgetInstance);

	UFUNCTION(BlueprintCallable)
	bool RemoveWidget(const TSubclassOf<UUserWidget>& widgetClass);

	UFUNCTION(BlueprintCallable)
	void SetWidgetHiddenInGame(const TSubclassOf<UUserWidget>& widgetClass, bool bNewHidden);

	UFUNCTION(BlueprintCallable)
	void ClearWidgetInGame();
};
