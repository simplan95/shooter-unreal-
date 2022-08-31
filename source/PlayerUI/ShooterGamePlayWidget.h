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
		void OnResumeClicked();//�簳 Ŭ����

	UFUNCTION()
		void OnReturnToTitleClicked();//Ÿ��Ʋ�� Ŭ����

	UFUNCTION()
		void OnRetryGameClicked();//���� ����� Ŭ����

private:
	UPROPERTY()
		class UButton* ResumeButton;

	UPROPERTY()
		class UButton* ReturnToTitleButton;

	UPROPERTY()
		class UButton* RetryGameButton;
};
