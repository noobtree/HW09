// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ClientController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChatReceivedSignature, const FString&, sender, const FString&, message);

/**
 * 
 */
UCLASS()
class HW09_API AClientController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnChatReceivedSignature OnMessageReceived;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> mainCanvasWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> chatWidgetClass;

public:
	UFUNCTION(BlueprintCallable)
	void BroadcastReceivedMessage(const FString& sender, const FString& message);

protected:
	// Client에서 실행되는 함수
	UFUNCTION(Client, Reliable)
	void ClientRPC_OnMessageReceived(const FString& sender, const FString& message);
	//void ClientRPC_OnMessageReceived_Implementation(const FString& sender, const FString& message);

	// Server에서 실행되는 함수
	UFUNCTION(Server, Reliable)
	void ServerRPC_OnMessageCommited(const FText& inputText);
	//void ServerRPC_OnMessageCommited_Implementation(const FText& inputText);
};
