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
	void BindCharacterStat(class UShooterActorComponent* NewCharacterStat);//캐릭터 스탯 클래스와 바인딩

protected:
	virtual void NativeConstruct()override;//UI 시스템 준비완료시 호출됨
	void UpdateHPWidget();//HP바 업데이트

private:
	TWeakObjectPtr<class UShooterActorComponent> CurrentCharacterStat;//약포인터 선언 (캐릭터 클래스와의 결합도를 낮추기 위함)

	UPROPERTY()
		class UProgressBar* HPProgressBar;
};
