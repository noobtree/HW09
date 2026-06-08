// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatLogWidget.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBoxSlot.h"

void UChatLogWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// messageTextBlock Setting
	if (IsValid(messageTextBlock) == true)
	{
		UHorizontalBoxSlot* messageSlot = Cast<UHorizontalBoxSlot>(messageTextBlock->Slot);
		if (messageSlot != nullptr)
		{
			messageSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		}
		messageTextBlock->SetAutoWrapText(true);
	}
}

void UChatLogWidget::SetChatSenderName(FString senderName)
{
	// add suffix
	senderName.Append(TEXT(" : "));

	// set Text
	senderTextBlock->SetText(FText::FromString(senderName));
}

void UChatLogWidget::SetChatMessage(const FString& messageString)
{
	messageTextBlock->SetText(FText::FromString(messageString));
}
