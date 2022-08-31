// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Shooter2.h"
#include "GameFramework/Character.h"
#include "Shooter2Character.generated.h"

UCLASS(config=Game)
class AShooter2Character : public ACharacter
{
	GENERATED_BODY()

public:
		AShooter2Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** ī�޶� �� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** ī�޶� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

public:

	UPROPERTY(EditAnywhere, Category = Grenade)//����ź�� ��ȯ�Ǵ� ��
		USceneComponent* GrenadeScene;

	UPROPERTY(EditAnywhere, Category = Grenade)//AI ������ ����
		USceneComponent* DetectedScene;

	UPROPERTY(VisibleAnywhere, Category = Weapon)//���� Ŭ����
		class AShooterGripGun* CurrentGun;

	UPROPERTY(VisibleAnywhere, Category = UI)//���� Ŭ����
		class UWidgetComponent* HPBarWidget;

	UPROPERTY(VisibleAnywhere, Category = Supply)//���޻��� Ŭ����
		class ASupplyBox* SupplyBox;

private:
	UPROPERTY()
		class UShooterAnimInstance* ShooterAnim;//�ִ��ν��Ͻ� Ŭ���� ����

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		ECharacterState CurrentState;//���� ������Ʈ ����

	UPROPERTY()
		class AShooterCharacterPlayerController* ShooterPlayerController;//��Ʈ�ѷ� Ŭ���� ���� ������Ʈ Ȯ�ο�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float DeadTimer;//����Ÿ�̸�

	UPROPERTY()
		FTimerHandle DeadTimerHandle = {};//����Ÿ�̸� �ڵ�

	UPROPERTY()
		FTimerHandle FireTimer;//�� ���縦 ���� Ÿ�̸��ڵ�

	UPROPERTY(VisibleAnywhere, Category = MyMesh)
		USkeletalMeshComponent* VisibleGrenade;//����ź ���̷�Ż �Ž� (����ź�� ������ ���־������θ� ��Ÿ��)


	UPROPERTY(EditAnywhere, Category = Particle, meta = (AllowPrivateAccess = "true"))//��ź ����Ʈ
		class UParticleSystem* ImpactParticle;
	UPROPERTY(EditAnywhere, Category = Sound, meta = (AllowPrivateAccess = "true"))//��ź����
		class USoundBase* PitanSound;
	UPROPERTY(EditAnywhere, Category = Sound, meta = (AllowPrivateAccess = "true"))//�������� �̽� ����
		class USoundBase* MissSound;
	UPROPERTY(EditAnywhere, Category = Sound, meta = (AllowPrivateAccess = "true"))//���� ���� ��Ʈ ����
		class USoundBase* HitSound;

	//�ӵ�����
	float StandSpeed = 250.0f;//���� �ӵ�
	float StandRunSpeed = 500.0f;//�޸��� �ӵ�
	float CrouchSpeed = 200.0f;//�ɾ��� �� �ӵ�

	//��Ÿ����
	bool bIsAttacking = false;//���� �� ����
	FVector AimVec = FVector(0.0f, 30.0f, 10.0f);//���ӽ� ī�޶� ��ġ
	FVector AimOffVec = FVector(0.0f, 50.0f, 0.0f);//���� off ��ī�޶� ��ġ
	float CameraBoomLenth = 200.0f;//������ �� ����
	float CameraLeftRight = 30.0;//ī�޶� �¿� ���� ������
	float NewCameraLeftRight = 30.0;
	bool bCanSupply = false;//���ް��ɿ���

	FVector DamegePoint = FVector(0.0f, 0.0f, 0.0f);//�������� �ִ� ���� ����

public:
	UPROPERTY(VisibleAnywhere, Category = Stat)//���� Ŭ����
		class UShooterActorComponent* CharacterStat;

	//�޸���,�ȱ�,�ɱ�,����,����,���� �� �׼�Ű ����
	void Run();
	void Walk();
	void DoCrouch();
	void Aim();
	void Aimoff();
	void Attack();
	void AttackEnd();
	void MyJump();
	void MyJumpStop();
	void Reload();
	void ThrowGrena();
	void GrenadeSet();
	void GrenadeRelease();
	void WeaponSetAfterGrenade();
	void AimChange();
	void InterAct();


	void MeleeAttack();//��Ƽ���� ȣ��� ����üũ
	void SwapWeapon();//��������Ʈ ȣ��� ���� ����
	void CharacterDead();//HP 0 ��������Ʈ ȣ��� ���ó��

	void SetCharacterState(ECharacterState NewState);//ĳ���� ������Ʈ ����(, Ready, Dead)
	ECharacterState GetCharacterState() const;//���� ������Ʈ ��ȯ

	//���� ����
	bool CanGetWeapon = true;
	void GunGrap();//���⺯�� ȣ��
	bool CanSetWeapon();//������� ���� ����
	void WeaponModeChange();//���� �߻� ��� ����
	void Fire();//�ѹ߻�
	void SetWeapon(class AShooterGripGun* Weapon);//��������

	void SetDamagePoint(FVector DamageSource);//������ ���� ����
	void KockBack(float Impulse);//�˹�
	FVector GetTargetPoint() const;
	bool GetIsAttacking() const;

	UFUNCTION()
		void OnSupplyBoxOverLap(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);//������ �̺�Ʈ�� ó���ϴ� �Լ�(���ö��̹ڽ�)

	UFUNCTION()
		void OnSupplyBoxEndOverLap(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);//������ �̺�Ʈ�� ó���ϴ� �Լ�(���ö��̹ڽ�)

protected:

	// ƽ
	virtual void Tick(float DeltaTime) override;

	//����ũ ����� �Լ� �������̵�
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;

	virtual void PostInitializeComponents() override;

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

