// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatLogWidget.generated.h"

/**
 * 
 */
UCLASS()
class HW09_API UChatLogWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:
	// 메세지를 전송자의 이름을 보관하는 TextBlock
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> senderTextBlock;

	// 메시지 내용을 보관하는 TextBlock
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBlock> messageTextBlock;

public:
	void SetChatSenderName(FString senderName);
	void SetChatMessage(const FString& messageString);
};
