// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchResultWidget.h"
#include "Components/TextBlock.h"
#include "GameMode/BullsAndCowsGameStateBase.h"

void UMatchResultWidget::NativeConstruct()
{
	ABullsAndCowsGameStateBase* gamestate =GetWorld()->GetGameState<ABullsAndCowsGameStateBase>();
	if (IsValid(gamestate) == true)
	{
		// gamestate에서 공개된 정답 얻기
		OnBullsAndCowsAnswerRevealed(gamestate->GetRevealedAnswerString());

		// Replicate 주기 차이로 인하여 Widget의 빠른 생성 및 Text 설정에 대비하여 이벤트 연결
		gamestate->OnAnswerRevealed.AddDynamic(this, &UMatchResultWidget::OnBullsAndCowsAnswerRevealed);
	}
}

void UMatchResultWidget::SetMatchResultString(const FString& resultString)
{
	matchResultTextBlock->SetText(FText::FromString(resultString));
}

void UMatchResultWidget::OnBullsAndCowsAnswerRevealed(const FString& revealedAnswerString)
{
	FString combined = FString::Printf(TEXT("Answer : %s"), *revealedAnswerString);
	answerTextBlock->SetText(FText::FromString(combined));
}
