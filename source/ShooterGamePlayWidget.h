// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "Blueprint/UserWidget.h"
#include "ShooterGamePlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER2_API UShooterGamePlayWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnResumeClicked();//재개 클릭시

	UFUNCTION()
		void OnReturnToTitleClicked();//타이틀로 클릭시

	UFUNCTION()
		void OnRetryGameClicked();//게임 재시작 클릭시

private:
	UPROPERTY()
		class UButton* ResumeButton;

	UPROPERTY()
		class UButton* ReturnToTitleButton;

	UPROPERTY()
		class UButton* RetryGameButton;
};
