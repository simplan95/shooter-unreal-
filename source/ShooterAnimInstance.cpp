// Fill out your copyright notice in the Description page of Project Settings.
#include "ShooterAnimInstance.h"
#include "Shooter2Character.h"
#include "Modules/ModuleManager.h"
#include "GameFramework/Character.h"//캐릭터
#include "GameFramework/PawnMovementComponent.h"//캐릭터움직임
#include "ShooterActorComponent.h"//캐릭터 스탯
#include "ShooterAICharacter.h"//AI 캐릭터

//플레이어 캐릭터 애니메이션 애님인스턴스
UShooterAnimInstance::UShooterAnimInstance()
{
	Direction = 0.0;//방향
	MySpeed = 0.0f;//속도
	bMyIsCrouching = false;//앉기 여부

	bIsJumping = false;//점프중인지 여부
	bIsGunUse = false;//총 사용 여부
	bIsAiming = false;//조준 여부
	bIsSprinting = false;//스프린팅 여부

	MoveForward = 0.0f;//이동 스케일 앞뒤
	MoveRight = 0.0f;//이동 스케일 좌우
	LowRotation = 0.0f;//허리회전 방향
	CurLowRotation = 0.0f;//현재 허리회전 방향
	
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

	auto Pawn = TryGetPawnOwner();//에니메이션 시스템 틱에서 폰에 접근해 폰의 정보를 가져오는 함수
	if (!::IsValid(Pawn)) return;//죽으면 리턴

	ShooterChracter = Cast<AShooter2Character>(Pawn);//캐릭터 선언으로 IsFalling() 사용

	//슈터 캐릭터, 슈터 캐릭터 살아있음
	if (ShooterChracter && ShooterChracter->CharacterStat->GetIsAlive())
	{
		MySpeed = Pawn->GetVelocity().Size();//폰 속도값
		Direction = CalculateDirection(Pawn->GetVelocity(), Pawn->GetActorRotation());//캐릭터의 방향값 가져오기

		bIsJumping = ShooterChracter->GetCharacterMovement()->IsFalling();//점프 중인지
		bMyIsCrouching = ShooterChracter->GetCharacterMovement()->IsCrouching();//앉기 중인지

		//달리는 중 또는 총 미장착 시 하반신 회전을 비활성
		if (IsSprinting() || !IsGunUsed()) LowRotation = 0.0f;

		//달리지 않는 중 이면서 총을 장착했을 때 허리회전 활성화
		if (IsGunUsed() && !IsSprinting())
		{
			if (MoveForward == 0.0f && MoveRight != 0.0f)//좌우 입력만 있을 경우
			{
				LowRotation = MoveRight * 80.0f;//허리회전은 -80~80

			}
			else if((MoveForward != 0.0f && MoveRight != 0.0f))//좌우 앞뒤가 동시에 입력될 경우(대각선이동)
			{
				LowRotation = MoveRight * 40.0f;//허리회전은 -40~40
			}
		}


		if (MoveForward == 0.0f && MoveRight != 0.0f)
		{
			MoveForward = 1.0f;//좌우 이동 방향 변경을 부드럽게
		}

		LowRotation = MoveForward * LowRotation;//뒤로 갈때는 허리방향을 반대로

		CurLowRotation = FMath::FInterpTo(CurLowRotation, LowRotation, 1.0f, 0.1f);

		//에임 오프셋
		if (IsGunUsed() && ShooterChracter->GetIsAttacking() || IsGunUsed() && IsAiming())//공격중 이거나 에임 중일 때 오프셋 활성화
		{
			ContRotation = RotateFromBP(ShooterChracter->GetControlRotation(), ShooterChracter->GetActorRotation());//BP에서 정의한 회전값을 가져오는 함수
			AimPitch = FMath::ClampAngle(FinRotation.Pitch, -90.0f, 90.0f);//Pitch축 범위
			CurRotation = FRotator(AimPitch, 0.0f, 0.0f);//현재 로테이션값
			FinRotation = FMath::RInterpTo(CurRotation, ContRotation, DeltaSeconds, 20.0f);//현재 로테이션을 타깃 로테이션으로 
		}
		else
		{
			AimPitch = FinRotation.Pitch;
			CurRotation = FRotator(AimPitch, 0.0f, 0.0f);//현재 로테이션값
			FinRotation = FMath::RInterpTo(CurRotation, FRotator::ZeroRotator , DeltaSeconds, 5.0f);//현재 로테이션을 타깃 로테이션으로 
		}
	
	}

}

void UShooterAnimInstance::PlayReloadMontage()//재장전 몽타주 
{
	if (bIsGunUse && !IsAnyMontagePlaying())
	{
		Montage_Play(Reload, 1.0f);
	}
}

void UShooterAnimInstance::PlayWeaponSwapMontage()//무기 장착/해제 몽타주
{
	if (!IsAnyMontagePlaying())
	{
		Montage_Play(WeaponSwap, 2.0f);
	}
}

void UShooterAnimInstance::PlayPunchMontage()//펀치 몽타주
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

void UShooterAnimInstance::PlayThrowGrenade()//수류탄 투척
{
	if (!IsAnyMontagePlaying())
	{
		Montage_Play(ThrowGrenade, 1.0f);
	}
}

void UShooterAnimInstance::AnimNotify_WeaponSwap()//무기 스왑 노티파이
{
	OnWeaponSwap.Broadcast();
}

void UShooterAnimInstance::AnimNotify_AttackCheck()//펀치 공격시 공격체크 노티파이
{
	OnAttackCheck.Broadcast();
}

void UShooterAnimInstance::AnimNotify_GrenadeStart()//수류탄투척 시작 노티파이
{
	OnGrenadeStart.Broadcast();
}

void UShooterAnimInstance::AnimNotify_GrenadeEnd()//수류탄투척 끝 노티파이
{
	OnGrenadeEnd.Broadcast();
}

void UShooterAnimInstance::AnimNotify_WeaponSet()//수류탄 투척 후 무기세팅 노티파이
{
	OnWeaponSet.Broadcast();
}

bool UShooterAnimInstance::IsGunUsed()//총 사용여부 반환
{
	return bIsGunUse;
}
void UShooterAnimInstance::SetGunUsed(bool GunUsed)//총사용 여부 세팅
{
	bIsGunUse = GunUsed;
}

bool UShooterAnimInstance::MyIsCrouching()//앉기 여부 반환
{
	return bMyIsCrouching;
}
void UShooterAnimInstance::SetCrouching(bool Crouching)//앉기 여부 세팅
{
	bMyIsCrouching = Crouching;
}

bool UShooterAnimInstance::IsAiming()//에임 여부 반환
{
	return bIsAiming;
}
void UShooterAnimInstance::SetAiming(bool Aiming)//에임 여부 세팅
{
	bIsAiming = Aiming;
}

bool UShooterAnimInstance::IsSprinting()//스프린팅 여부 반환
{
	return bIsSprinting;
}
void UShooterAnimInstance::SetSprinting(bool Sprinting)//스프린팅 여부 세팅
{
	bIsSprinting = Sprinting;
}