// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BullsAndCowsNumberPadWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBullsAndCowsCommittedSignature, const FString&, BullsAndCowsString);

/**
 *
 */
UCLASS()
class HW09_API UBullsAndCowsNumberPadWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnBullsAndCowsCommittedSignature OnBullsAndCowsCommitted;

protected:
	// 입력된 문자열을 출력하는 TextBlock
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UTextBlock> displayTextBlock;

	// 입력한 문자열을 사용하는 Button
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UButton> commitButton;

	// 입력된 문자열을 저장하는 변수
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	FString inputString = "";

protected:
	// UI의 숫자 버튼 클릭 시 실행되는 함수
	UFUNCTION(BlueprintCallable)
	void OnNumberButtonClicked(int32 number);

	// UI의 Backspace 버튼 클릭 시 실행되는 함수
	UFUNCTION(BlueprintCallable)
	void OnBackspaceButtonClicked();

	// UI의 디스플레이에 노출되는 문자열을 설정하는 함수
	UFUNCTION(BlueprintCallable)
	void SetDisplayText(FString displayString);

	// 임의 문자열에 대하여 BullsAndCows 조건을 만족하는지 검사
	UFUNCTION(BlueprintCallable)
	bool IsValidBullsAndCows(const FString& guessString);

	// UI의 제출 버튼 클릭 시 실행되는 함수
	UFUNCTION(BlueprintCallable)
	void OnCommitButtonClicked();
};
