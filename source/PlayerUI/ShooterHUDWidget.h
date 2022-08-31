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
	void BindCharacterStat(class UShooterActorComponent* CharacterStat);//HUD클래스를 UABCharacterStatComponent클래스와 바인딩
	void BindPlayerState(class AShooterPlayerState* PlayerState);//HUD클래스를 AShooterPlayerState클래스와 바인딩
	void GetDistanceToBoss(float Distance);

protected:
	virtual void NativeConstruct() override;//변수와 UI클래스 바인딩
	void UpdateCharacterHPStat();//캐릭터의 스탯(HP)이 변할때 마다 업데이트
	void UpdateCharacterGrenadeStat();//캐릭터의 스탯(수류탄 수)이 변할때 마다 업데이트
	void UpdateCharacterAmmoStat();//캐릭터의 스탯(총알 수)이 변할때 마다 업데이트
	void UpdatePlayerScore();//캐릭터스코어 변동시 업데이트

private:
	TWeakObjectPtr<class UShooterActorComponent> CurrentCharacterStat;//현재 캐릭터 스탯의 상태를 약포인터로 선언
	TWeakObjectPtr<class AShooterPlayerState> CurrentPlayerState;//현재 플레이어의 상태를 약포인터로 선언

	UPROPERTY()
		class UProgressBar* HPBar;//HP바

	UPROPERTY()
		class UTextBlock* PlayerMagazine;//플레이어 탄창 총알

	UPROPERTY()
		class UTextBlock* PlayerAmmo;//플레이어 소지 총알

	UPROPERTY()
		class UTextBlock* PlayerGrenade;//플레이어 현재 수류탄 소지수

	UPROPERTY()
		class UTextBlock* CurrentScore;//현재점수

	UPROPERTY()
		class UTextBlock* DistanceToBoss;//보스까지거리

};
