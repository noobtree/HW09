// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BullsAndCowsGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class HW09_API ABullsAndCowsGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_OnClientLogin(const FString& InNameString = FString(TEXT("XXXXXXXX")));
};
