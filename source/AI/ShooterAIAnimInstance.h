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

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;//틱마다 호출 되는 가상함수, 이를 오버라이드 하여 CurrentPawnSpeed를 매 틱마다 업데이트 한다.

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//방향
		float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//스피드
		float MySpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//점프 여부
		bool bIsJumping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//스프린트 여부
		bool bIsSprinting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//에임 여부
		bool bIsAiming;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))//공격 몽타주
		UAnimMontage* Fire;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))//재장전
		UAnimMontage* Reload;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))//히트리액션
		UAnimMontage* HitReact;

	UPROPERTY()//AI슈터 캐릭터
		class AShooterAICharacter* ShooterAIChracter;

	UPROPERTY()//AI 컨트롤러
		class AShooterAIController* ShooterAIChracterController;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//에임 오프셋 축
		float AimPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//에임 오프셋 축
		FRotator CurRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//에임 오프셋 축
		FRotator ContRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))//에임 오프셋 축
		FRotator FinRotation;

	void PlayAttackMontage();//공격 몽타주 실행
	void PlayReloadMontage();//재장전 몽타주 실행
	void PlayHitReactMontage();//재장전 몽타주 실행

	bool GetIsSprinting() const;//달리기 여부 반환
	void SetIsSprinting(bool Sprinting);//달리기 모드 세팅
	bool GetIsAiming() const;//조준 여부 반환
	void SetIsAiming(bool Aiming);//조준 세팅

	FOnRestrictStartDelegate OnRestrictStart;
	FOnRestrictEndDelegate OnRestrictEnd;

	UFUNCTION()
		void AnimNotify_RestrictStart();//피격 애니 시작
	UFUNCTION()
		void AnimNotify_RestrictEnd();//피격 애니 끝
};
