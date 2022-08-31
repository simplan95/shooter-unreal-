// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "Components/ActorComponent.h"
#include "ShooterActorComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);//�������� ��������Ʈ ���� (��ü����)
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);//HP ���� ��������Ʈ ���� (��ü����)
DECLARE_MULTICAST_DELEGATE(FOnAmmoCountCangedDelegate);//�Ѿ� �� ��������Ʈ ���� (��ü����)
DECLARE_MULTICAST_DELEGATE(FOnGrenadeCountChagedDelegate);//����ź �� ��������Ʈ ���� (��ü����)

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
	virtual void InitializeComponent()override;// PostInitializeComponents ���� ���� ȣ�� ������ ����(TickComponent�Լ��� �����Ǿ�  Tick �Լ��� �ʿ�����Ƿ� ��ü��)
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:	

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))//���� ü���� ������ ������ ������ ����ǹǷ� ����ȭ(�Ӽ�����) ����(Transient)
		float CurrentHP;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))//�ִ� ü��
		float MaxHP;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))//���� ������
		float UserShootDamage = 20.0f;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))//���� ������
		float UserMeleeDamage = 50.0f;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))//AI ������
		float AIDamage = 5.0f;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))//�������� �ִ� �Ѿ�
		int32 MaxBullet = 0;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))//źâ�� �Ѿ��� ������ ������ ������ ����ǹǷ� ����ȭ(�Ӽ�����) ����(Transient)
		int32 CurrentBullet=0;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))//ĳ���� �������
		bool IsAlive=true;

	UPROPERTY(Transient, EditAnywhere)
		int32 GrenadeCount=0;


	//��������
	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))//AI �� ������
		float BossBulletDamage = 5.0f;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))//AI ������
		float BossGrenadeDamage = 50.0f;
	
	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))//AI �̻��� ������
		float BossMissleDamage = 15.0f;


public:
	float GetShootDamage() const;//�� ������ ��ȯ
	float GetAIShootDamage() const;//Ai ������ ��ȯ
	float GetAIBossShootDamage() const;//Ai ���� �� ������ ��ȯ
	float GetAIBossMissileDamage() const;//Ai ���� �̻��� ������ ��ȯ
	float GetMeleeDamage() const;//�и� ���ݷ� ��ȯ
	int32 GetGrenadeCount() const;//����ź �� ��ȯ
	int32 GetMaxBullet() const;//���� �Ѿ� �� ��ȯ
	int32 GetMagazineBullet() const;//���� �Ѿ� �� ��ȯ
	float GetCurrentHP() const;//���� ü�¹�ȯ
	float GetMaxHP() const;//������ �ִ�ü�� ��ȯ
	bool GetIsAlive() const;//��������

	void SetDamage(float NewDamage);//������ ����
	void SetHP(float NewHP);//HP ����
	float GetHPRatio() const;//HP�� 

	void SetMaxHP(float HP);//�ִ�ü�� ����
	void SetCurrentHP(float HP);//�ִ� HP ����
	void SetMaxBullet(int32 Bullet);//���� ������ �ִ� �����Ѿ� ���� ���ϰų� ��
	void SetMaxBulletQuantity(int32 Bullet);//���� ������ �ִ� �����Ѿ� ���� ����
	void SetGrenadeCount(int32 Greande);//���� ������ �ִ� ����ź�� ����
	void SetMagazineCount(int32 Bullet);//źâ �Ѿ� �� ����
		
	FOnHPIsZeroDelegate OnHPIsZero;//HP�� 0�϶� �߻��ϴ� ��������Ʈ �ν��Ͻ�
	FOnHPChangedDelegate OnHPChanged;//HP������Ʈ�� �߻�
	FOnAmmoCountCangedDelegate OnAmmoCountChaned;//�Ѿ� �� ������
	FOnGrenadeCountChagedDelegate OnGrenadeCountChanged;//����ź �� ������
};
