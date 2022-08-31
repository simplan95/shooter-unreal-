// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "Animation/AnimInstance.h"
#include "ShooterAnimInstance.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnWeaponSwapDelegate)//���� ���Ұ��� ��������Ʈ ��Ƽĳ��Ʈ�� ���� ��ɾ�:Broadcast(��ü����)6
DECLARE_MULTICAST_DELEGATE(FOnAttackCheckDelegate)//�Ǽ� ���ݰ��� ��������Ʈ ��Ƽĳ��Ʈ�� ���� ��ɾ�:Broadcast(��ü����)6
DECLARE_MULTICAST_DELEGATE(FOnGrenadeStartDelegate)//����ź ������ �˸�
DECLARE_MULTICAST_DELEGATE(FOnGrenadeEndDelegate)//����ź ��ô�� �˸�
DECLARE_MULTICAST_DELEGATE(FOnWeaponSetDelegate)//����ź ������ �˸�
/**
 * 
 */
UCLASS()
class SHOOTER2_API UShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UShooterAnimInstance();//������ ����

private:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;//ƽ���� ȣ�� �Ǵ� �����Լ�, �̸� �������̵� �Ͽ� CurrentPawnSpeed�� �� ƽ���� ������Ʈ �Ѵ�.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//�� ��� ����
		bool bIsGunUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//�ɱ� ����
		bool bMyIsCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//���� ����
		bool bIsJumping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//���� ����
		bool bIsAiming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//������Ʈ ����
		bool bIsSprinting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//���� ������ ��
		float AimPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//���� ������ ��(���� ĳ���� �����̼�)
		FRotator CurRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//���� ������ ��(��Ʈ�ѷ��� �����̼�)
		FRotator ContRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//���� ������ ��(��ǥ �����̼�)
		FRotator FinRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//�㸮����
		float LowRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//���� �㸮����
		float CurLowRotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))//������
		UAnimMontage* Reload;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))//���⽺��
		UAnimMontage* WeaponSwap;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))//�޼� ����
		UAnimMontage* Left;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))//������ ����
		UAnimMontage* Right;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))//����ź ��ô
		UAnimMontage* ThrowGrenade;

	//���� ĳ����
	TWeakObjectPtr<class AShooter2Character> ShooterChracter;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//����
		float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//���ǵ�
		float MySpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//�����̴� ����
		float MoveForward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//�����̴� ����
		float MoveRight;

	void PlayAttackMontage();//���� ��Ÿ�� ����
	void PlayReloadMontage();//������ ��Ÿ�� ����
	void PlayWeaponSwapMontage();//���⽺�Ҹ�Ÿ��
	void PlayPunchMontage();//��ġ ��Ÿ��
	void PlayThrowGrenade();//����ź ��ô

	FOnWeaponSwapDelegate OnWeaponSwap;//���⽺�� �������̼�
	FOnAttackCheckDelegate OnAttackCheck;//�Ǽհ��� Ȯ�� �������̼�
	FOnGrenadeStartDelegate OnGrenadeStart;//����ź ������ �˸�
	FOnGrenadeEndDelegate OnGrenadeEnd;//����ź ��ô�� �˸�
	FOnWeaponSetDelegate OnWeaponSet;//���� ������ �˸�

	bool IsGunUsed();//�ѻ�� ���� ��ȯ
	void SetGunUsed(bool GunUsed);//�ѻ�� ���� ����
	bool MyIsCrouching();//�ɱ� ���� ��ȯ
	void SetCrouching(bool Crouching);//�ɱ� ���� ����
	bool IsAiming();//���� ���� ��ȯ
	void SetAiming(bool Aiming);//���� ���� ����
	bool IsSprinting();//�������� ���� ��ȯ
	void SetSprinting(bool Sprinting);//�������� ���� ����

	UFUNCTION()
		void AnimNotify_WeaponSwap();//���⽺��

	UFUNCTION()
		void AnimNotify_AttackCheck();//���� ý

	UFUNCTION()
		void AnimNotify_GrenadeStart();//����ź ����

	UFUNCTION()
		void AnimNotify_GrenadeEnd();//����ź ��

	UFUNCTION()
		void AnimNotify_WeaponSet();//���⼼��

	UFUNCTION(BlueprintImplementableEvent, Category = "Delta")//��� ����Ʈ���� �����ϴ� �Լ�
		FRotator RotateFromBP(FRotator Cont, FRotator Chara);


};
