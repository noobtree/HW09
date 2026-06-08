// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/HorizontalBox.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "ChatLogWidget.h"

void UChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(commitButton) == true)
	{
		//commitButton->OnClicked.AddDynamic(inputField, inputField->OnTextCommitted)
	}
	if (IsValid(inputField) == true)
	{
		if (inputField->OnTextCommitted.IsAlreadyBound(this, &UChatWidget::OnMessageCommitted) == false)
		{
			inputField->OnTextCommitted.AddDynamic(this, &UChatWidget::OnMessageCommitted);
		}
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

void UChatWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	//inputRowHorizontalBox->SetVisibility(ESlateVisibility::Visible);
}

void UChatWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	//inputRowHorizontalBox->SetVisibility(ESlateVisibility::Hidden);
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
			onMessageCommitted.Broadcast(commitMessage);
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
