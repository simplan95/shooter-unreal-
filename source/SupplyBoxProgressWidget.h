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
	virtual void NativeConstruct()override;//UI 시스템 준비완료시 호출됨
	void UpdatePGWidget();//HP바 업데이트

private:
	TWeakObjectPtr<class ASupplyBox> SupplyBox;//약포인터로 선언

	UPROPERTY()
		class UProgressBar* HPProgressBar;
};
