// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "Blueprint/UserWidget.h"
#include "SupplyBoxProgressWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER2_API USupplyBoxProgressWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void BindSupplyBox(class ASupplyBox* NewSupplyBox);

protected:
	virtual void NativeConstruct()override;//UI �ý��� �غ�Ϸ�� ȣ���
	void UpdatePGWidget();//HP�� ������Ʈ

private:
	TWeakObjectPtr<class ASupplyBox> SupplyBox;//�������ͷ� ����

	UPROPERTY()
		class UProgressBar* HPProgressBar;
};
