// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "Blueprint/UserWidget.h"
#include "ShooterHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER2_API UShooterHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindCharacterStat(class UShooterActorComponent* CharacterStat);//HUDŬ������ UABCharacterStatComponentŬ������ ���ε�
	void BindPlayerState(class AShooterPlayerState* PlayerState);//HUDŬ������ AShooterPlayerStateŬ������ ���ε�
	void GetDistanceToBoss(float Distance);

protected:
	virtual void NativeConstruct() override;//������ UIŬ���� ���ε�
	void UpdateCharacterHPStat();//ĳ������ ����(HP)�� ���Ҷ� ���� ������Ʈ
	void UpdateCharacterGrenadeStat();//ĳ������ ����(����ź ��)�� ���Ҷ� ���� ������Ʈ
	void UpdateCharacterAmmoStat();//ĳ������ ����(�Ѿ� ��)�� ���Ҷ� ���� ������Ʈ
	void UpdatePlayerScore();//ĳ���ͽ��ھ� ������ ������Ʈ

private:
	TWeakObjectPtr<class UShooterActorComponent> CurrentCharacterStat;//���� ĳ���� ������ ���¸� �������ͷ� ����
	TWeakObjectPtr<class AShooterPlayerState> CurrentPlayerState;//���� �÷��̾��� ���¸� �������ͷ� ����

	UPROPERTY()
		class UProgressBar* HPBar;//HP��

	UPROPERTY()
		class UTextBlock* PlayerMagazine;//�÷��̾� źâ �Ѿ�

	UPROPERTY()
		class UTextBlock* PlayerAmmo;//�÷��̾� ���� �Ѿ�

	UPROPERTY()
		class UTextBlock* PlayerGrenade;//�÷��̾� ���� ����ź ������

	UPROPERTY()
		class UTextBlock* CurrentScore;//��������

	UPROPERTY()
		class UTextBlock* DistanceToBoss;//���������Ÿ�

};
