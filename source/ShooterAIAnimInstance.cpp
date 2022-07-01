// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIAnimInstance.h"
#include "Modules/ModuleManager.h"
#include "GameFramework/Character.h"//ĳ����
#include "GameFramework/PawnMovementComponent.h"//ĳ���Ϳ�����
#include "ShooterActorComponent.h"//ĳ���� ����
#include "ShooterAICharacter.h"//AI ĳ����
#include "ShooterAIController.h"//AI��Ʈ�ѷ�
#include "Shooter2Character.h"//Ÿ�� ĳ����

//AI ĳ���� �ִ��ν��Ͻ�
UShooterAIAnimInstance::UShooterAIAnimInstance()
{
	Direction = 0.0;//����
	MySpeed = 0.0f;//�ӵ�

	bIsJumping = false;//���������� ����
	bIsSprinting = false;//�������� ����
	bIsAiming = false;//���̹� ����

	static ConstructorHelpers::FObjectFinder<UAnimMontage>FIRE_HIP(TEXT("/Game/Mannequin/Animations/AM_Fire_Hip.AM_Fire_Hip"));
	if (FIRE_HIP.Succeeded())
	{
		Fire = FIRE_HIP.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>RELOAD(TEXT("/Game/Mannequin/Animations/AM_Reload.AM_Reload"));
	if (RELOAD.Succeeded())
	{
		Reload = RELOAD.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>HITREACT(TEXT("/Game/Mannequin/Animations/AI/AM_Hit.AM_Hit"));
	if (HITREACT.Succeeded())
	{
		HitReact = HITREACT.Object;
	}

	AimPitch = 0.0f;

	ContRotation = FRotator::ZeroRotator;
	FinRotation = FRotator::ZeroRotator;
	CurRotation = FRotator::ZeroRotator;

}


void UShooterAIAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//���ϸ��̼� �ý��� ƽ���� ���� ������ ���� ���� ������
	auto AIPawn = TryGetPawnOwner();
	if (!::IsValid(AIPawn)) return;//������ ����

	MySpeed = AIPawn->GetVelocity().Size();//�� �ӵ���
	Direction = CalculateDirection(AIPawn->GetVelocity(), AIPawn->GetActorRotation());//ĳ������ ���Ⱚ ��������

	ShooterAIChracter = Cast<AShooterAICharacter>(AIPawn);//AIĳ����
	ShooterAIChracterController = Cast<AShooterAIController>(ShooterAIChracter->GetController());

	if (ShooterAIChracter && ShooterAIChracter->CharacterStat->GetIsAlive())
	{
		bIsJumping = ShooterAIChracter->GetCharacterMovement()->IsFalling();//���� ������
	}

}

void UShooterAIAnimInstance::PlayAttackMontage()//���� ��Ÿ�� 
{
	if (!IsAnyMontagePlaying())
	{
		Montage_Play(Fire, 1.5f);
	}
}
void UShooterAIAnimInstance::PlayReloadMontage()//������ ��Ÿ�� 
{
	if (!IsAnyMontagePlaying())
	{
		Montage_Play(Reload, 1.0f);
	}
}
void UShooterAIAnimInstance::PlayHitReactMontage()//�ǰݸ�Ÿ��
{
	if (!IsAnyMontagePlaying())
	{
		Montage_Play(HitReact, 1.0f);
	}
}

void UShooterAIAnimInstance::AnimNotify_RestrictStart()//�������� On ��Ƽ����
{
	OnRestrictStart.Broadcast();
}

void UShooterAIAnimInstance::AnimNotify_RestrictEnd()//�������� Off ��Ƽ����
{
	OnRestrictEnd.Broadcast();

}

bool UShooterAIAnimInstance::GetIsSprinting() const//�޸��� ���� ��ȯ
{
	return bIsSprinting;
}
void UShooterAIAnimInstance::SetIsSprinting(bool Sprinting)//�޸��� ��� ����
{
	bIsSprinting = Sprinting;
}
bool UShooterAIAnimInstance::GetIsAiming() const//���� ���� ��ȯ
{
	return bIsAiming;
}
void UShooterAIAnimInstance::SetIsAiming(bool Aiming)//���� ����
{
	bIsAiming = Aiming;
}