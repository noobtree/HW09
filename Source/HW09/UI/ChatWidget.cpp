// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/EditableTextBox.h"
#include "Components/HorizontalBox.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "ChatLogWidget.h"
#include "Player/ClientController.h"

void UChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 이벤트 연결
	if (IsValid(inputField) == true)
	{
		if (inputField->OnTextCommitted.IsAlreadyBound(this, &UChatWidget::OnMessageCommitted) == false)
		{
			inputField->OnTextCommitted.AddDynamic(this, &UChatWidget::OnMessageCommitted);
		}
	}
	if (IsValid(GetOwningPlayer()) == true)
	{
		AClientController* clientController = Cast<AClientController>(GetOwningPlayer());
		clientController->OnMessageReceived.AddDynamic(this, &UChatWidget::OnMessageReceived);
	}

	// 초기 위치 조정
	UCanvasPanelSlot* widgetSlot = Cast<UCanvasPanelSlot>(Slot);
	if (widgetSlot != nullptr)
	{
		
		widgetSlot->SetAnchors(FAnchors(0.f, 1.f, 0.f, 1.f));
		widgetSlot->SetAlignment(FVector2D(0, 1));
		widgetSlot->SetPosition(FVector2D(80, -45));
		widgetSlot->SetAutoSize(true);
	}
}

void UChatWidget::NativeDestruct()
{
	if (IsValid(inputField) == true)
	{
		if (inputField->OnTextCommitted.IsAlreadyBound(this, &UChatWidget::OnMessageCommitted) == true)
		{
			inputField->OnTextCommitted.RemoveDynamic(this, &UChatWidget::OnMessageCommitted);
		}
	}

	Super::NativeDestruct();
}

UPanelSlot* UChatWidget::AddChildWidget_Implementation(UUserWidget* widget)
{
	return chatLogScrollBox->AddChild(widget);
}

void UChatWidget::OnMessageCommitted(const FText& inputText, ETextCommit::Type commitMethod)
{
	// 디버그 메시지 출력
	UE_LOG(LogTemp, Display, TEXT("%s"), *inputText.ToString());

	if (inputText.IsEmptyOrWhitespace() == true)
	{
		return;
	}

	FText commitMessage = inputText;
	inputField->SetText(FText::GetEmpty());

	if (GetOwningPlayer()->IsLocalController() == true)
	{
		if (onMessageCommitted.IsBound() == true)
		{
			onMessageCommitted.Broadcast(commitMessage.ToString());
		}
	}
}

void UChatWidget::OnMessageReceived(const FString& sender, const FString& message)
{
	// 로그 Widget 생성
	UUserWidget* logWidget = CreateWidget<UChatLogWidget>(GetOwningPlayer(), logWidgetClass);

	// ScrollBox에 Widget 추가
	UPanelSlot* widgetSlot = chatLogScrollBox->AddChild(logWidget);

	// 로그 내용 수정
	if (UChatLogWidget* chatLog = Cast<UChatLogWidget>(logWidget))
	{
		chatLog->SetChatSenderName(sender);
		chatLog->SetChatMessage(message);
	}
}
