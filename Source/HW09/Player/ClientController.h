// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ClientController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMessageReceivedSignature, const FString&, senderString, const FString&, messageString);

/**
 * 
 */
UCLASS()
class HW09_API AClientController : public APlayerController
{
	GENERATED_BODY()
	
	AClientController();

public:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnMessageReceivedSignature OnMessageReceived;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> mainCanvasWidgetClass;

public:
	// 메시지를 수신하는 경우 Server에서 호출하여 Client에서 실행되는 함수
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_PullMessage(const FString& senderString, const FString& messageString);

protected:
	void InitializeClientController();
};
