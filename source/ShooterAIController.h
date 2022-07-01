// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Shooter2.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"
/**
 *
 */
UCLASS()
class SHOOTER2_API AShooterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AShooterAIController();

	//빙의
	virtual void OnPossess(APawn* InPawn) override;

public:
	//틱
	virtual void Tick(float DeltaTime) override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName TargetPosKey;

	void RunAI();//AI 구동
	void StopAI();//AI 정지

	bool bIsTracking;//추적중인지

private:
	UPROPERTY()
		class UBehaviorTree* BTAsset;//비헤이비어트리 선언

	UPROPERTY()
		class UBlackboardData* BBAsset;//블랙보드 선언
	
	UPROPERTY()
		class UBlackboardComponent* BlackboardComp;//블랙보드 컴포넌트

	UPROPERTY()
		class AShooterAICharacter* AICharacter;//AI가 조종하는 캐릭터

	UPROPERTY()
		class UShooterAIAnimInstance* AIAnim;//AI가 조종하는 캐릭터

	UPROPERTY()
		class AShooter2Character* TargetCharacter;//플레이어가 조종하는 캐릭터



public:
	//타겟 추격
	UFUNCTION(BlueprintCallable, Category = Behavior)//BlueprintCallable 블루프린트 기반 태스크에서 호출 가능
		void TrackToTarget();

	//타겟 추적 중지
	UFUNCTION(BlueprintCallable, Category = Behavior)
		void StopTrack();

	//범위밖 타깃 추적
	UFUNCTION(BlueprintCallable, Category = Behavior)
		void TrackToTargetOutRange(FVector TargetLocation);

};
