// Fill out your copyright notice in the Description page of Project Settings.


#include "BullsAndCowsGameStateBase.h"
#include "Player/ClientController.h"

void ABullsAndCowsGameStateBase::MulticastRPC_OnClientLogin_Implementation(const FString& InNameString)
{
	if (HasAuthority() == true)
	{
		return;
	}

	APlayerController* controller = GetWorld()->GetFirstPlayerController();
	if (IsValid(controller) == true)
	{
		AClientController* clientController = Cast<AClientController>(controller);
		if (IsValid(clientController) == true)
		{
			FString notificationString = InNameString + TEXT("has joined the Game");
			clientController->BroadcastReceivedMessage(TEXT("Server"), notificationString);
		}
	}
}
