// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChatWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"

void UChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(commitButton) == true)
	{
		//commitButton->OnClicked.AddDynamic(inputField, inputField->OnTextCommitted)
	}
	if (IsValid(inputField) == true)
	{
		if (inputField->OnTextCommitted.IsAlreadyBound(this, &UChatWidget::OnInputFieldCommitted) == false)
		{
			inputField->OnTextCommitted.AddDynamic(this, &UChatWidget::OnInputFieldCommitted);
		}
	}
}

void UChatWidget::NativeDestruct()
{
	if (IsValid(inputField) == true)
	{
		if (inputField->OnTextCommitted.IsAlreadyBound(this, &UChatWidget::OnInputFieldCommitted) == true)
		{
			inputField->OnTextCommitted.RemoveDynamic(this, &UChatWidget::OnInputFieldCommitted);
		}
	}
}

void UChatWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	//inputRowSizeBox->SetVisibility(ESlateVisibility::Visible);
}

void UChatWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	//inputRowSizeBox->SetVisibility(ESlateVisibility::Hidden);
}

UPanelSlot* UChatWidget::AddChildWidget_Implementation(UUserWidget* widget)
{
	return chatLogScrollBox->AddChild(widget);
}

void UChatWidget::OnInputFieldCommitted(const FText& inputText, ETextCommit::Type commitMethod)
{
	// 디버그 메시지 출력
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, inputText.ToString());

	if (inputText.IsEmptyOrWhitespace() == true)
	{
		return;
	}

	inputField->SetText(FText::GetEmpty());

	if (onMessageCommitted.IsBound() == true)
	{
		onMessageCommitted.Broadcast(inputText, commitMethod);
	}
}
