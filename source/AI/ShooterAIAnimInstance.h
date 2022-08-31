// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Shooter2.h"
#include "Animation/AnimInstance.h"
#include "ShooterAIAnimInstance.generated.h"

UCLASS()
class SHOOTER2_API UShooterAIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UShooterAIAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;//ƽ���� ȣ�� �Ǵ� �����Լ�, �̸� �������̵� �Ͽ� CurrentPawnSpeed�� �� ƽ���� ������Ʈ �Ѵ�.

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//����
		float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//���ǵ�
		float MySpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//���� ����
		bool bIsJumping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//������Ʈ ����
		bool bIsSprinting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//���� ����
		bool bIsAiming;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))//���� ��Ÿ��
		UAnimMontage* Fire;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))//������
		UAnimMontage* Reload;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))//��Ʈ���׼�
		UAnimMontage* HitReact;

	UPROPERTY()//AI���� ĳ����
		class AShooterAICharacter* ShooterAIChracter;

	UPROPERTY()//AI ��Ʈ�ѷ�
		class AShooterAIController* ShooterAIChracterController;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//���� ������ ��
		float AimPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//���� ������ ��
		FRotator CurRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//���� ������ ��
		FRotator ContRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//���� ������ ��
		FRotator FinRotation;

	void PlayAttackMontage();//���� ��Ÿ�� ����
	void PlayReloadMontage();//������ ��Ÿ�� ����
	void PlayHitReactMontage();//������ ��Ÿ�� ����

	bool GetIsSprinting() const;//�޸��� ���� ��ȯ
	void SetIsSprinting(bool Sprinting);//�޸��� ��� ����
	bool GetIsAiming() const;//���� ���� ��ȯ
	void SetIsAiming(bool Aiming);//���� ����

	FOnRestrictStartDelegate OnRestrictStart;
	FOnRestrictEndDelegate OnRestrictEnd;

	UFUNCTION()
		void AnimNotify_RestrictStart();//�ǰ� �ִ� ����
	UFUNCTION()
		void AnimNotify_RestrictEnd();//�ǰ� �ִ� ��
};
