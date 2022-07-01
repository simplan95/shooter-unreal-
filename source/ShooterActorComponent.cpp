// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterActorComponent.h"
#include "ShooterGripGrenade.h"//������ ����ź

//������ ����(ü��, ���ݷ�, ��� ��, ����ź ��) ����
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

void UShooterActorComponent::InitializeComponent()// PostInitializeComponents ���� ���� ȣ�� ������ ����
{
	Super::InitializeComponent();
}

// Called every frame
void UShooterActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UShooterActorComponent::GetShootDamage() const//�� ������ ��ȯ
{
	return UserShootDamage;
}

float UShooterActorComponent::GetAIShootDamage() const//Ai �� ������ ��ȯ
{
	return AIDamage;
}

float UShooterActorComponent::GetAIBossShootDamage() const//Ai ���� �� ������ ��ȯ
{
	return BossBulletDamage;
}

float UShooterActorComponent::GetAIBossMissileDamage() const//Ai ���� �̻��� ������ ��ȯ
{
	return BossMissleDamage;
}

float UShooterActorComponent::GetMeleeDamage() const//�и� ���� ������ ��ȯ
{
	return UserMeleeDamage;
}

int32 UShooterActorComponent::GetGrenadeCount() const//���� ����ź �� ��ȯ
{
	return GrenadeCount;
}

int32 UShooterActorComponent::GetMaxBullet() const//���� �Ѿ� �� ��ȯ
{
	return MaxBullet;
}

float UShooterActorComponent::GetCurrentHP() const//���� ü�� ��ȯ
{
	return CurrentHP;
}

int32 UShooterActorComponent::GetMagazineBullet() const//źâ �Ѿ� �� ��ȯ
{
	return CurrentBullet;
}

float UShooterActorComponent::GetMaxHP() const//�ִ�ü�¹�ȯ
{
	return MaxHP;
}

bool UShooterActorComponent::GetIsAlive() const//�������� ��ȯ
{
	return IsAlive;
}

void UShooterActorComponent::SetHP(float NewHP)//HP ����
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	
	if (CurrentHP < KINDA_SMALL_NUMBER&& IsAlive)//float ���� 0�� ���� �� �̼��� ������������ �ִ��� �Ǵ� KINDA_SMALL_NUMBER
	{
		OnHPIsZero.Broadcast();
		IsAlive = false;
	}
}

float UShooterActorComponent::GetHPRatio() const//HP�� ����
{
	return (CurrentHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / MaxHP);
	
}

void UShooterActorComponent::SetDamage(float NewDamage)//������ ����
{
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, MaxHP));//HP�� ����, ���� ü�¿� ������ ����
}

void UShooterActorComponent::SetMaxHP(float HP)//����ü�� ����
{
	MaxHP = HP;
}

void UShooterActorComponent::SetCurrentHP(float HP)//�ִ�ü�� ����
{
	CurrentHP = HP;
}

void UShooterActorComponent::SetMaxBullet(int32 Bullet)
{
	MaxBullet += Bullet;
	OnAmmoCountChaned.Broadcast();//�Ѿ� �� ������ ��ε� ĳ��Ʈ
}

void UShooterActorComponent::SetMaxBulletQuantity(int32 Bullet)
{
	MaxBullet = Bullet;
	OnAmmoCountChaned.Broadcast();//�Ѿ� �� ������ ��ε� ĳ��Ʈ
}

void UShooterActorComponent::SetGrenadeCount(int32 Greande)
{
	GrenadeCount += Greande;
	OnGrenadeCountChanged.Broadcast();//����ź �� ������ ��ε� ĳ��Ʈ
}

void UShooterActorComponent::SetMagazineCount(int32 Bullet)
{
	CurrentBullet += Bullet;
	OnAmmoCountChaned.Broadcast();//�Ѿ� �� ������ ��ε� ĳ��Ʈ
}

