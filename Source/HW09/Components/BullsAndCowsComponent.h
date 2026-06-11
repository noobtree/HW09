// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WidgetControllable.h"
#include "BullsAndCowsComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HW09_API UBullsAndCowsComponent : public UActorComponent, public IWidgetControllable
{
	GENERATED_BODY()

protected:
	// 숫자 입력 Widget 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> numberpadWidgetClass;

	// BullsAndCows 로그 피드 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> logFeedWidgetClass;

protected:
	UFUNCTION(BlueprintCallable)
	virtual void InitializeControlWidget() override;

	UFUNCTION(BlueprintCallable)
	void CommitBullsAndCowsGuess(const FString& guessString);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_OnCommitBullsAndCowsGuess(const APlayerController* caller, const FString& guessString);
};
