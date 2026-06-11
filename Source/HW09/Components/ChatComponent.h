// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WidgetControllable.h"
#include "ChatComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HW09_API UChatComponent : public UActorComponent, public IWidgetControllable
{
	GENERATED_BODY()

protected:
	// Chat UI 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> chatWidgetClass;

protected:
	// 관리하는 UI 생성 및 이벤트 연결 등의 설정을 진행하는 함수
	UFUNCTION(BlueprintCallable)
	virtual void InitializeControlWidget() override;

	// Chat UI에서 메시지 전송 시 실행되는 함수
	UFUNCTION(BlueprintCallable)
	virtual void PushMessage(const FString& message);

	/// <summary>
	/// 클라이언트가 메시지를 전송하는 경우 Client에서 호출하여 Server에서 실행되는 함수
	/// </summary>
	/// <param name="client">전송자의 PlayerController</param>
	/// <param name="messageString">전송 메시지</param>
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_OnPushMessage(const APlayerController* caller, const FString& messageString);
};
