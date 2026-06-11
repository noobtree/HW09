// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WidgetControllable.h"
#include "LifePointComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBullsAndCowsGuessCountChangedSignature, const int32&, remainGuessCount, const int32&, maxGuessCount);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HW09_API ULifePointComponent : public UActorComponent, public IWidgetControllable
{
	GENERATED_BODY()

public:
	ULifePointComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// IWidgetControllable을(를) 통해 상속됨
	void InitializeControlWidget() override;

public:
	UPROPERTY(BlueprintAssignable)
	FBullsAndCowsGuessCountChangedSignature OnGuessCountChanged;

protected:
	// 숫자 입력 Widget 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> lifeWidgetClass;

	// Replicate로 값이 변화하였을 경우 OnRep_RemainGuessCount함수 실행
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Replicated, ReplicatedUsing = OnRep_RemainGuessCount)
	int32 remainGuessCount;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Replicated)
	int32 maxGuessCount;

public:
	UFUNCTION(BlueprintCallable)
	void DecreaseRemainGuessCount(int32 deltaGuessCount);

	UFUNCTION(BlueprintCallable)
	void InitilizeRemainGuessCount();

	FORCEINLINE int32 GetRemainGuessCount() const { return remainGuessCount; }

protected:
	UFUNCTION()
	void OnRep_RemainGuessCount();
};
