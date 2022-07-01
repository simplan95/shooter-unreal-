// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterActorComponent.h"
#include "ShooterGripGrenade.h"//아이템 수류탄

//액터의 스탯(체력, 공격력, 충알 수, 수류탄 수) 관리
UShooterActorComponent::UShooterActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UShooterActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UShooterActorComponent::InitializeComponent()// PostInitializeComponents 보다 먼저 호출 역할은 동일
{
	Super::InitializeComponent();
}

// Called every frame
void UShooterActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UShooterActorComponent::GetShootDamage() const//슛 데미지 반환
{
	return UserShootDamage;
}

float UShooterActorComponent::GetAIShootDamage() const//Ai 슛 데미지 반환
{
	return AIDamage;
}

float UShooterActorComponent::GetAIBossShootDamage() const//Ai 보스 슛 데미지 반환
{
	return BossBulletDamage;
}

float UShooterActorComponent::GetAIBossMissileDamage() const//Ai 보스 미사일 데미지 반환
{
	return BossMissleDamage;
}

float UShooterActorComponent::GetMeleeDamage() const//밀리 공격 데미지 반환
{
	return UserMeleeDamage;
}

int32 UShooterActorComponent::GetGrenadeCount() const//현재 수류탄 수 반환
{
	return GrenadeCount;
}

int32 UShooterActorComponent::GetMaxBullet() const//소지 총알 수 반환
{
	return MaxBullet;
}

float UShooterActorComponent::GetCurrentHP() const//현재 체력 반환
{
	return CurrentHP;
}

int32 UShooterActorComponent::GetMagazineBullet() const//탄창 총알 수 반환
{
	return CurrentBullet;
}

float UShooterActorComponent::GetMaxHP() const//최대체력반환
{
	return MaxHP;
}

bool UShooterActorComponent::GetIsAlive() const//생존여부 반환
{
	return IsAlive;
}

void UShooterActorComponent::SetHP(float NewHP)//HP 세팅
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	
	if (CurrentHP < KINDA_SMALL_NUMBER&& IsAlive)//float 값을 0과 비교할 때 미세한 오차범위내에 있는지 판단 KINDA_SMALL_NUMBER
	{
		OnHPIsZero.Broadcast();
		IsAlive = false;
	}
}

float UShooterActorComponent::GetHPRatio() const//HP율 세팅
{
	return (CurrentHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / MaxHP);
	
}

void UShooterActorComponent::SetDamage(float NewDamage)//데미지 세팅
{
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, MaxHP));//HP율 설정, 현재 체력에 데미지 적용
}

void UShooterActorComponent::SetMaxHP(float HP)//현재체력 지정
{
	MaxHP = HP;
}

void UShooterActorComponent::SetCurrentHP(float HP)//최대체력 지정
{
	CurrentHP = HP;
}

void UShooterActorComponent::SetMaxBullet(int32 Bullet)
{
	MaxBullet += Bullet;
	OnAmmoCountChaned.Broadcast();//총알 수 변동시 브로드 캐스트
}

void UShooterActorComponent::SetMaxBulletQuantity(int32 Bullet)
{
	MaxBullet = Bullet;
	OnAmmoCountChaned.Broadcast();//총알 수 변동시 브로드 캐스트
}

void UShooterActorComponent::SetGrenadeCount(int32 Greande)
{
	GrenadeCount += Greande;
	OnGrenadeCountChanged.Broadcast();//수류탄 수 변동시 브로드 캐스트
}

void UShooterActorComponent::SetMagazineCount(int32 Bullet)
{
	CurrentBullet += Bullet;
	OnAmmoCountChaned.Broadcast();//총알 수 변동시 브로드 캐스트
}

