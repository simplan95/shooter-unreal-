// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIAnimInstance.h"
#include "Modules/ModuleManager.h"
#include "GameFramework/Character.h"//캐릭터
#include "GameFramework/PawnMovementComponent.h"//캐릭터움직임
#include "ShooterActorComponent.h"//캐릭터 스탯
#include "ShooterAICharacter.h"//AI 캐릭터
#include "ShooterAIController.h"//AI 컨트롤러

//AI 미니언 캐릭터 애님 인스턴스
UShooterAIAnimInstance::UShooterAIAnimInstance()
{
	Direction = 0.0;//방향
	MySpeed = 0.0f;//속도

	bIsJumping = false;//점프중인지 여부
	bIsSprinting = false;//스프린팅 여부
	bIsAiming = false;//에이밍 여부

	//총 발사 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage>FIRE_HIP(TEXT("/Game/Mannequin/Animations/AM_Fire_Hip.AM_Fire_Hip"));
	if (FIRE_HIP.Succeeded())
	{
		Fire = FIRE_HIP.Object;
	}

	//재장전 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage>RELOAD(TEXT("/Game/Mannequin/Animations/AM_Reload.AM_Reload"));
	if (RELOAD.Succeeded())
	{
		Reload = RELOAD.Object;
	}

	//피격 몽타주
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

	auto AIPawn = TryGetPawnOwner();//에니메이션 시스템 틱에서 폰에 접근해 폰의 정보를 가져오는 함수
	if (!::IsValid(AIPawn)) return;//죽으면 리턴

	MySpeed = AIPawn->GetVelocity().Size();//폰 속도값
	Direction = CalculateDirection(AIPawn->GetVelocity(), AIPawn->GetActorRotation());//캐릭터의 방향값 가져오기

	ShooterAIChracter = Cast<AShooterAICharacter>(AIPawn);//AI캐릭터
	ShooterAIChracterController = Cast<AShooterAIController>(ShooterAIChracter->GetController());

	if (ShooterAIChracter && ShooterAIChracter->CharacterStat->GetIsAlive())
	{
		bIsJumping = ShooterAIChracter->GetCharacterMovement()->IsFalling();//점프 중인지
	}

}

void UShooterAIAnimInstance::PlayAttackMontage()//공격 몽타주 
{
	if (!IsAnyMontagePlaying())
	{
		Montage_Play(Fire, 1.5f);
	}
}
void UShooterAIAnimInstance::PlayReloadMontage()//재장전 몽타주 
{
	if (!IsAnyMontagePlaying())
	{
		Montage_Play(Reload, 1.0f);
	}
}
void UShooterAIAnimInstance::PlayHitReactMontage()//피격몽타주
{
	if (!IsAnyMontagePlaying())
	{
		Montage_Play(HitReact, 1.0f);
	}
}

void UShooterAIAnimInstance::AnimNotify_RestrictStart()//공격제한 On 노티파이
{
	OnRestrictStart.Broadcast();
}

void UShooterAIAnimInstance::AnimNotify_RestrictEnd()//공격제한 Off 노티파이
{
	OnRestrictEnd.Broadcast();

}

bool UShooterAIAnimInstance::GetIsSprinting() const//달리기 여부 반환
{
	return bIsSprinting;
}
void UShooterAIAnimInstance::SetIsSprinting(bool Sprinting)//달리기 모드 세팅
{
	bIsSprinting = Sprinting;
}
bool UShooterAIAnimInstance::GetIsAiming() const//조준 여부 반환
{
	return bIsAiming;
}
void UShooterAIAnimInstance::SetIsAiming(bool Aiming)//조준 세팅
{
	bIsAiming = Aiming;
}