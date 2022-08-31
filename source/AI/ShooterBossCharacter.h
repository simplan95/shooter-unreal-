// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "ShooterEnemyCharacter.h"
#include "GameFramework/Character.h"
#include "ShooterBossCharacter.generated.h"

UCLASS()
class SHOOTER2_API AShooterBossCharacter : public AShooterEnemyCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterBossCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		ECharacterState CurrentState;//���� ������Ʈ ����

	UPROPERTY()//AI ��Ʈ�ѷ�
		class AShooterBossAIController* ShooterAIController;

	UPROPERTY(EditAnywhere)//���� ����ź ���� ��
		TArray<USceneComponent*> BossGrenadeScene;

	UPROPERTY(EditAnywhere, Category = Mesh)//���� �̻��� ������
		USceneComponent* BossMissileScene;

	UPROPERTY(VisibleAnywhere, Category = Weapon)//������� ��
		class AShooterGripGun* NewWeapon;

	UPROPERTY(VisibleAnywhere, Category = Missile, Meta = (AllowPrivateAccess = true))//�̻��� �߻�� ��ü �޽�
		UStaticMeshComponent* MissileHead;

	UPROPERTY(VisibleAnywhere, Category = Missile, Meta = (AllowPrivateAccess = true))//�̻��� �߻�� �޽�
		UStaticMeshComponent* MissileLauncher;

	UPROPERTY(EditAnywhere, Category = Particle, meta = (AllowPrivateAccess = "true"))//��ź ����Ʈ
		class UParticleSystem* ImpactParticle;
	UPROPERTY(EditAnywhere, Category = Sound, meta = (AllowPrivateAccess = "true"))//��ź����
		class USoundBase* PitanSound;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Paticle, meta = (AllowPrivateAccess = "true"))//�̻��� �߻� ��ƼŬ
		class UParticleSystemComponent* MissileFireParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)//�̻��� �߻� ����
		class USoundBase* MissileFireSound;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//����ũ ������ �Լ� ���� ���̵�
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void PostInitializeComponents() override;

	void SetCharacterState(ECharacterState NewState);//ĳ���� ������Ʈ ����
	ECharacterState GetCharacterState() const;//ĳ���� ������Ʈ ��ȯ

	void CharacterDead();//HP 0 ��������Ʈ ȣ��� ���ó��
	void Fire();//�ѹ߻�
	void Grenade();//����ź
	void Missile();//�̻���
};
