// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "ShooterEnemyCharacter.h"
#include "GameFramework/Character.h"
#include "ShooterAICharacter.generated.h"

UCLASS()
class SHOOTER2_API AShooterAICharacter : public AShooterEnemyCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		ECharacterState CurrentState;//���� ������Ʈ ����

	UPROPERTY()//AI
		class AShooterAIController* ShooterAIController;//��Ʈ�ѷ� Ŭ���� ���� ������Ʈ Ȯ�ο�

	UPROPERTY()
		class UShooterAIAnimInstance* ShooterAnim;//�ִ��ν��Ͻ� Ŭ���� ����

	UPROPERTY(VisibleAnywhere, Category = Weapon)//������� ��
		class AShooterGripGun* NewWeapon;

	UPROPERTY(EditAnywhere, Category = Particle, meta = (AllowPrivateAccess = "true"))//��ź ����Ʈ
		class UParticleSystem* ImpactParticle;
	UPROPERTY(EditAnywhere, Category = Sound, meta = (AllowPrivateAccess = "true"))//��ź����
		class USoundBase* PitanSound;

	UPROPERTY(EditAnywhere, Category = Grenade)//AI ������ ����
		USceneComponent* DetectScene;

	//���ݰ��� Ȯ��
	UPROPERTY()
		bool CanAttack = true;

public:
	//����ũ ����� �Լ� �������̵�
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;


	void CharacterDead();//HP 0 ��������Ʈ ȣ��� ���ó��

	void Run();//�޸���ӵ���
	void Walk();//�ȱ�ӵ���

	void Fire();//�� �߻�
	void Reload();//������

	void FCanAttack();//���ݰ������� ����
	void FCannotAttack();//���ݺҰ������� ����
	void SetCharacterState(ECharacterState NewState);//������Ʈ����

	bool GetCanAttack() const;//���ݰ��� ���� ��ȯ
	class UShooterAIAnimInstance* GetAIAnim() const;//�ִ� �ν��Ͻ� ��ȯ
	ECharacterState GetCharacterState() const;
	FVector GetDtectPoint() const;//Ž�����۽�
};
