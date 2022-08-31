// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "Components/ActorComponent.h"
#include "ShooterActorComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);//죽음전달 델리게이트 선언 (전체파일)
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);//HP 연동 델리게이트 선언 (전체파일)
DECLARE_MULTICAST_DELEGATE(FOnAmmoCountCangedDelegate);//총알 수 델리게이트 선언 (전체파일)
DECLARE_MULTICAST_DELEGATE(FOnGrenadeCountChagedDelegate);//수류탄 수 델리게이트 선언 (전체파일)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER2_API UShooterActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UShooterActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent()override;// PostInitializeComponents 보다 먼저 호출 역할은 동일(TickComponent함수가 제공되어  Tick 함수가 필요없으므로 대체함)
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:	

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))//현재 체력은 게임을 시작할 때마다 변경되므로 직렬화(속성저장) 제외(Transient)
		float CurrentHP;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))//최대 체력
		float MaxHP;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))//유저 데미지
		float UserShootDamage = 20.0f;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))//유저 데미지
		float UserMeleeDamage = 50.0f;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))//AI 데미지
		float AIDamage = 5.0f;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))//소지가능 최대 총알
		int32 MaxBullet = 0;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))//탄창안 총알은 게임을 시작할 때마다 변경되므로 직렬화(속성저장) 제외(Transient)
		int32 CurrentBullet=0;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))//캐릭터 사망여부
		bool IsAlive=true;

	UPROPERTY(Transient, EditAnywhere)
		int32 GrenadeCount=0;


	//보스스탯
	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))//AI 슛 데미지
		float BossBulletDamage = 5.0f;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))//AI 데미지
		float BossGrenadeDamage = 50.0f;
	
	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))//AI 미사일 데미지
		float BossMissleDamage = 15.0f;


public:
	float GetShootDamage() const;//슛 데미지 반환
	float GetAIShootDamage() const;//Ai 데미지 반환
	float GetAIBossShootDamage() const;//Ai 보스 슛 데미지 반환
	float GetAIBossMissileDamage() const;//Ai 보스 미사일 데미지 반환
	float GetMeleeDamage() const;//밀리 공격력 반환
	int32 GetGrenadeCount() const;//수류탄 수 반환
	int32 GetMaxBullet() const;//소지 총알 수 반환
	int32 GetMagazineBullet() const;//소지 총알 수 반환
	float GetCurrentHP() const;//현재 체력반환
	float GetMaxHP() const;//설정한 최대체력 반환
	bool GetIsAlive() const;//생존여부

	void SetDamage(float NewDamage);//데미지 세팅
	void SetHP(float NewHP);//HP 세팅
	float GetHPRatio() const;//HP율 

	void SetMaxHP(float HP);//최대체력 설정
	void SetCurrentHP(float HP);//최대 HP 설정
	void SetMaxBullet(int32 Bullet);//현재 가지고 있는 예비총알 수를 더하거나 뺌
	void SetMaxBulletQuantity(int32 Bullet);//현재 가지고 있는 예비총알 수를 세팅
	void SetGrenadeCount(int32 Greande);//현재 가지고 있는 수류탄수 세팅
	void SetMagazineCount(int32 Bullet);//탄창 총알 수 설정
		
	FOnHPIsZeroDelegate OnHPIsZero;//HP가 0일때 발생하는 델리게이트 인스턴스
	FOnHPChangedDelegate OnHPChanged;//HP업데이트시 발생
	FOnAmmoCountCangedDelegate OnAmmoCountChaned;//총알 수 변동시
	FOnGrenadeCountChagedDelegate OnGrenadeCountChanged;//수류탄 수 변동시
};
