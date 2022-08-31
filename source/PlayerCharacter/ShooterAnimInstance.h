// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "Animation/AnimInstance.h"
#include "ShooterAnimInstance.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnWeaponSwapDelegate)//무기 스왑관련 델리게이트 멀티캐스트로 선언 명령어:Broadcast(전체파일)6
DECLARE_MULTICAST_DELEGATE(FOnAttackCheckDelegate)//맨손 공격관련 델리게이트 멀티캐스트로 선언 명령어:Broadcast(전체파일)6
DECLARE_MULTICAST_DELEGATE(FOnGrenadeStartDelegate)//수류탄 시작을 알림
DECLARE_MULTICAST_DELEGATE(FOnGrenadeEndDelegate)//수류탄 투척을 알림
DECLARE_MULTICAST_DELEGATE(FOnWeaponSetDelegate)//수류탄 세팅을 알림
/**
 * 
 */
UCLASS()
class SHOOTER2_API UShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UShooterAnimInstance();//생성자 선언

private:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;//틱마다 호출 되는 가상함수, 이를 오버라이드 하여 CurrentPawnSpeed를 매 틱마다 업데이트 한다.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//총 사용 여부
		bool bIsGunUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//앉기 여부
		bool bMyIsCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//점프 여부
		bool bIsJumping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//에임 여부
		bool bIsAiming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//스프린트 여부
		bool bIsSprinting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//에임 오프셋 축
		float AimPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//에임 오프셋 축(현재 캐릭터 로테이션)
		FRotator CurRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//에임 오프셋 축(컨트롤러의 로테이션)
		FRotator ContRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//에임 오프셋 축(목표 로테이션)
		FRotator FinRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//허리방향
		float LowRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//현재 허리방향
		float CurLowRotation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))//재장전
		UAnimMontage* Reload;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))//무기스왑
		UAnimMontage* WeaponSwap;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))//왼손 공격
		UAnimMontage* Left;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))//오른손 공격
		UAnimMontage* Right;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))//수류탄 투척
		UAnimMontage* ThrowGrenade;

	//슈터 캐릭터
	TWeakObjectPtr<class AShooter2Character> ShooterChracter;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//방향
		float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//스피드
		float MySpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//움직이는 방향
		float MoveForward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//움직이는 방향
		float MoveRight;

	void PlayAttackMontage();//공격 몽타주 실행
	void PlayReloadMontage();//재장전 몽타주 실행
	void PlayWeaponSwapMontage();//무기스왑몽타주
	void PlayPunchMontage();//펀치 몽타주
	void PlayThrowGrenade();//수류탄 투척

	FOnWeaponSwapDelegate OnWeaponSwap;//무기스왑 델리게이션
	FOnAttackCheckDelegate OnAttackCheck;//맨손공격 확인 델리게이션
	FOnGrenadeStartDelegate OnGrenadeStart;//수류탄 시작을 알림
	FOnGrenadeEndDelegate OnGrenadeEnd;//수류탄 투척을 알림
	FOnWeaponSetDelegate OnWeaponSet;//무기 세팅을 알림

	bool IsGunUsed();//총사용 여부 반환
	void SetGunUsed(bool GunUsed);//총사용 여부 세팅
	bool MyIsCrouching();//앉기 여부 반환
	void SetCrouching(bool Crouching);//앉기 여부 세팅
	bool IsAiming();//에임 여부 반환
	void SetAiming(bool Aiming);//에임 여부 세팅
	bool IsSprinting();//스프린팅 여부 반환
	void SetSprinting(bool Sprinting);//스프린팅 여부 세팅

	UFUNCTION()
		void AnimNotify_WeaponSwap();//무기스왑

	UFUNCTION()
		void AnimNotify_AttackCheck();//어택 첵

	UFUNCTION()
		void AnimNotify_GrenadeStart();//수류탄 시작

	UFUNCTION()
		void AnimNotify_GrenadeEnd();//수류탄 끝

	UFUNCTION()
		void AnimNotify_WeaponSet();//무기세팅

	UFUNCTION(BlueprintImplementableEvent, Category = "Delta")//블루 프린트에서 정의하는 함수
		FRotator RotateFromBP(FRotator Cont, FRotator Chara);


};
