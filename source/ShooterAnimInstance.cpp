// Fill out your copyright notice in the Description page of Project Settings.
#include "ShooterAnimInstance.h"
#include "Shooter2Character.h"
#include "Modules/ModuleManager.h"
#include "GameFramework/Character.h"//ĳ����
#include "GameFramework/PawnMovementComponent.h"//ĳ���Ϳ�����
#include "ShooterActorComponent.h"//ĳ���� ����
#include "ShooterAICharacter.h"//AI ĳ����

//�÷��̾� ĳ���� �ִϸ��̼� �ִ��ν��Ͻ�
UShooterAnimInstance::UShooterAnimInstance()
{
	Direction = 0.0;//����
	MySpeed = 0.0f;//�ӵ�
	bMyIsCrouching = false;//�ɱ� ����

	bIsJumping = false;//���������� ����
	bIsGunUse = false;//�� ��� ����
	bIsAiming = false;//���� ����
	bIsSprinting = false;//�������� ����

	MoveForward = 0.0f;//�̵� ������ �յ�
	MoveRight = 0.0f;//�̵� ������ �¿�
	LowRotation = 0.0f;//�㸮ȸ�� ����
	CurLowRotation = 0.0f;//���� �㸮ȸ�� ����
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage>RELOAD(TEXT("/Game/Mannequin/Animations/AM_Reload.AM_Reload"));
	if (RELOAD.Succeeded())
	{
		Reload = RELOAD.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>WEAPONSWAP(TEXT("/Game/Mannequin/Animations/AM_Equip.AM_Equip"));
	if (WEAPONSWAP.Succeeded())
	{
		WeaponSwap = WEAPONSWAP.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>Leftpunch(TEXT("/Game/Mannequin/Animations/AM_Left.AM_Left"));
	if (Leftpunch.Succeeded())
	{
		Left = Leftpunch.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>RightPunch(TEXT("/Game/Mannequin/Animations/AM_Right.AM_Right"));
	if (RightPunch.Succeeded())
	{
		Right = RightPunch.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>Grenade(TEXT("/Game/Mannequin/Animations/AM_Grenade.AM_Grenade"));
	if (Grenade.Succeeded())
	{
		ThrowGrenade = Grenade.Object;
	}

	AimPitch = 0.0f;

	ContRotation = FRotator::ZeroRotator;
	FinRotation = FRotator::ZeroRotator;
	CurRotation = FRotator::ZeroRotator;

}


void UShooterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();//���ϸ��̼� �ý��� ƽ���� ���� ������ ���� ������ �������� �Լ�
	if (!::IsValid(Pawn)) return;//������ ����

	ShooterChracter = Cast<AShooter2Character>(Pawn);//ĳ���� �������� IsFalling() ���

	//���� ĳ����, ���� ĳ���� �������
	if (ShooterChracter && ShooterChracter->CharacterStat->GetIsAlive())
	{
		MySpeed = Pawn->GetVelocity().Size();//�� �ӵ���
		Direction = CalculateDirection(Pawn->GetVelocity(), Pawn->GetActorRotation());//ĳ������ ���Ⱚ ��������

		bIsJumping = ShooterChracter->GetCharacterMovement()->IsFalling();//���� ������
		bMyIsCrouching = ShooterChracter->GetCharacterMovement()->IsCrouching();//�ɱ� ������

		//�޸��� �� �Ǵ� �� ������ �� �Ϲݽ� ȸ���� ��Ȱ��
		if (IsSprinting() || !IsGunUsed()) LowRotation = 0.0f;

		//�޸��� �ʴ� �� �̸鼭 ���� �������� �� �㸮ȸ�� Ȱ��ȭ
		if (IsGunUsed() && !IsSprinting())
		{
			if (MoveForward == 0.0f && MoveRight != 0.0f)//�¿� �Է¸� ���� ���
			{
				LowRotation = MoveRight * 80.0f;//�㸮ȸ���� -80~80

			}
			else if((MoveForward != 0.0f && MoveRight != 0.0f))//�¿� �յڰ� ���ÿ� �Էµ� ���(�밢���̵�)
			{
				LowRotation = MoveRight * 40.0f;//�㸮ȸ���� -40~40
			}
		}


		if (MoveForward == 0.0f && MoveRight != 0.0f)
		{
			MoveForward = 1.0f;//�¿� �̵� ���� ������ �ε巴��
		}

		LowRotation = MoveForward * LowRotation;//�ڷ� ������ �㸮������ �ݴ��

		CurLowRotation = FMath::FInterpTo(CurLowRotation, LowRotation, 1.0f, 0.1f);

		//���� ������
		if (IsGunUsed() && ShooterChracter->GetIsAttacking() || IsGunUsed() && IsAiming())//������ �̰ų� ���� ���� �� ������ Ȱ��ȭ
		{
			ContRotation = RotateFromBP(ShooterChracter->GetControlRotation(), ShooterChracter->GetActorRotation());//BP���� ������ ȸ������ �������� �Լ�
			AimPitch = FMath::ClampAngle(FinRotation.Pitch, -90.0f, 90.0f);//Pitch�� ����
			CurRotation = FRotator(AimPitch, 0.0f, 0.0f);//���� �����̼ǰ�
			FinRotation = FMath::RInterpTo(CurRotation, ContRotation, DeltaSeconds, 20.0f);//���� �����̼��� Ÿ�� �����̼����� 
		}
		else
		{
			AimPitch = FinRotation.Pitch;
			CurRotation = FRotator(AimPitch, 0.0f, 0.0f);//���� �����̼ǰ�
			FinRotation = FMath::RInterpTo(CurRotation, FRotator::ZeroRotator , DeltaSeconds, 5.0f);//���� �����̼��� Ÿ�� �����̼����� 
		}
	
	}

}

void UShooterAnimInstance::PlayReloadMontage()//������ ��Ÿ�� 
{
	if (bIsGunUse && !IsAnyMontagePlaying())
	{
		Montage_Play(Reload, 1.0f);
	}
}

void UShooterAnimInstance::PlayWeaponSwapMontage()//���� ����/���� ��Ÿ��
{
	if (!IsAnyMontagePlaying())
	{
		Montage_Play(WeaponSwap, 2.0f);
	}
}

void UShooterAnimInstance::PlayPunchMontage()//��ġ ��Ÿ��
{
	if (!IsAnyMontagePlaying())
	{
		bool Select = FMath::RandBool();
		if (Select)
		{
			Montage_Play(Left, 1.0f);
		}
		else
		{
			Montage_Play(Right, 1.0f);
		}
		
	}
}

void UShooterAnimInstance::PlayThrowGrenade()//����ź ��ô
{
	if (!IsAnyMontagePlaying())
	{
		Montage_Play(ThrowGrenade, 1.0f);
	}
}

void UShooterAnimInstance::AnimNotify_WeaponSwap()//���� ���� ��Ƽ����
{
	OnWeaponSwap.Broadcast();
}

void UShooterAnimInstance::AnimNotify_AttackCheck()//��ġ ���ݽ� ����üũ ��Ƽ����
{
	OnAttackCheck.Broadcast();
}

void UShooterAnimInstance::AnimNotify_GrenadeStart()//����ź��ô ���� ��Ƽ����
{
	OnGrenadeStart.Broadcast();
}

void UShooterAnimInstance::AnimNotify_GrenadeEnd()//����ź��ô �� ��Ƽ����
{
	OnGrenadeEnd.Broadcast();
}

void UShooterAnimInstance::AnimNotify_WeaponSet()//����ź ��ô �� ���⼼�� ��Ƽ����
{
	OnWeaponSet.Broadcast();
}

bool UShooterAnimInstance::IsGunUsed()//�� ��뿩�� ��ȯ
{
	return bIsGunUse;
}
void UShooterAnimInstance::SetGunUsed(bool GunUsed)//�ѻ�� ���� ����
{
	bIsGunUse = GunUsed;
}

bool UShooterAnimInstance::MyIsCrouching()//�ɱ� ���� ��ȯ
{
	return bMyIsCrouching;
}
void UShooterAnimInstance::SetCrouching(bool Crouching)//�ɱ� ���� ����
{
	bMyIsCrouching = Crouching;
}

bool UShooterAnimInstance::IsAiming()//���� ���� ��ȯ
{
	return bIsAiming;
}
void UShooterAnimInstance::SetAiming(bool Aiming)//���� ���� ����
{
	bIsAiming = Aiming;
}

bool UShooterAnimInstance::IsSprinting()//�������� ���� ��ȯ
{
	return bIsSprinting;
}
void UShooterAnimInstance::SetSprinting(bool Sprinting)//�������� ���� ����
{
	bIsSprinting = Sprinting;
}