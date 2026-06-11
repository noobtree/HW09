// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetAcceptable.h"
#include "ChatWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageCommittedSignature, const FString&, inputString);

/**
 * 
 */
UCLASS()
class HW09_API UChatWidget : public UUserWidget, public IWidgetAcceptable
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

public:
	// 채팅 입력 완료 시 실행되는 이벤트 (const FText& inputText, ETextCommit::Type commitMethod)
	FOnMessageCommittedSignature onMessageCommitted;

protected:
	// 로그 Widget의 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> logWidgetClass;

	// 채팅 로그를 담는 스크롤박스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UScrollBox> chatLogScrollBox;

	// 메시지 입력 영역을 담는 Horizontal Box
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UHorizontalBox> inputRowHorizontalBox;

	// 메시지를 입력받을 수 있는 텍스트박스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UEditableTextBox> inputField;

	// 전송버튼
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	TObjectPtr<class UButton> commitButton;

public:
	// 채팅 로그를 관리하는 ScrollBox에 Widget을 추가
	UPanelSlot* AddChildWidget_Implementation(UUserWidget* widget) override;

protected:
	UFUNCTION(BlueprintCallable)
	void OnMessageCommitted(const FText& inputText, ETextCommit::Type commitMethod);

	UFUNCTION(BlueprintCallable)
	void OnMessageReceived(const FString& sender, const FString& message);
};
