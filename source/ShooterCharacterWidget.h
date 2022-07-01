// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "Blueprint/UserWidget.h"
#include "ShooterCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER2_API UShooterCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class UShooterActorComponent* NewCharacterStat);//ĳ���� ���� Ŭ������ ���ε�

protected:
	virtual void NativeConstruct()override;//UI �ý��� �غ�Ϸ�� ȣ���
	void UpdateHPWidget();//HP�� ������Ʈ

private:
	TWeakObjectPtr<class UShooterActorComponent> CurrentCharacterStat;//�������� ���� (ĳ���� Ŭ�������� ���յ��� ���߱� ����)

	UPROPERTY()
		class UProgressBar* HPProgressBar;
};
