// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "Blueprint/UserWidget.h"
#include "ShooterGamePlayResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER2_API UShooterGamePlayResultWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindGameState(class AShooterGameState* GameState);//���� ������Ʈ Ŭ������ ���ε�

protected:
	virtual void NativeConstruct() override;

private:
	TWeakObjectPtr<class AShooterGameState> CurrentGameState;//���ӽ����̸� Ŭ���� ���� NativeConstruct�Լ��� AddViewport �Լ��� �ܺο��� ȣ�� �Ǹ� UI ������ �ʱ�ȭ�Ǿ� ȣ��Ǳ� ������
														//�̸� ���ӽ�����Ʈ���� ������ ������ ȣ���ϱ� ����

	UPROPERTY()
		class UButton* ReturnToTitleButton;//Ÿ��Ʋ�η�

	UPROPERTY()
		class UButton* RetryGameButton;//�����

	UFUNCTION()
		void OnReturnToTitleClicked();//Ÿ��Ʋ�� Ŭ����

	UFUNCTION()
		void OnRetryGameClicked();//����� Ŭ����
};
