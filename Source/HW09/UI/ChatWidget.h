// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetAcceptable.h"
#include "ChatWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChatCommittedSignature, const FText&, inputText, ETextCommit::Type, commitMethod);

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

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

public:
	// 채팅 입력 완료 시 실행되는 이벤트 (const FText& inputText, ETextCommit::Type commitMethod)
	FOnChatCommittedSignature onMessageCommitted;

protected:
	// 채팅 로그를 관리하는 스크롤박스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UScrollBox> chatLogScrollBox;

	// 메시지 입력 영역을 관리하는 사이즈박스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class USizeBox> inputRowSizeBox;

	// 메시지를 입력받을 수 있는 텍스트박스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UEditableTextBox> inputField;

	// 전송버튼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<class UButton> commitButton;

public:
	// 채팅 로그를 관리하는 ScrollBox에 Widget을 추가
	UPanelSlot* AddChildWidget_Implementation(UUserWidget* widget) override;

protected:
	UFUNCTION(BlueprintCallable)
	void OnInputFieldCommitted(const FText& inputText, ETextCommit::Type commitMethod);
};
