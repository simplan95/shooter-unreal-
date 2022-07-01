// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"//블랙보드 키 올타입 
#include "ShooterBossAIController.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EBossState : uint8//보스 공격패턴
{
	BS_FIRE = 0 UMETA(DisplayName = "Fire"),
	BS_MISSILE = 1 UMETA(DisplayName = "Missile"),
	BS_GRENADE = 2 UMETA(DisplayName = "Grenade"),
};

UCLASS()
class SHOOTER2_API AShooterBossAIController : public AAIController
{
	GENERATED_BODY()

	//빙의
	virtual void OnPossess(APawn* InPawn) override;

	//틱
	virtual void Tick(float DeltaTime) override;

public:
	AShooterBossAIController();
	
private:
	UPROPERTY()
		class UBehaviorTree* BTAsset;//행동트리 

	UPROPERTY()
		class UBlackboardData* BBAsset;//블랙보드 

	UPROPERTY()
		class UBlackboardComponent* BlackboardComp;//블랙보드 컴포넌트

	UPROPERTY()
		class AShooterBossCharacter* AICharacter;//AI가 조종하는 캐릭터

	UPROPERTY()
		class AShooter2Character* TargetCharacter;//플레이어가 조종하는 캐릭터
	
public:
	//타겟 추적
	UFUNCTION(BlueprintCallable, Category = Behavior)//BlueprintCallable 블루프린트 기반 태스크에서 호출 가능
		void TrackToTarget();

	//타겟 추적 중지
	UFUNCTION(BlueprintCallable, Category = Behavior)
		void StopTrack();

	//타겟 추적 중지
	UFUNCTION(BlueprintCallable, Category = Behavior)
		void TrackToTargetOutRange(FVector TargetLocation);

	UPROPERTY()
	EBossState BossPattern;//보스 패턴

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName TargetPosKey;
	static const FName AttackPattern;

	void RunAI();//AI 구동
	void StopAI();//AI 정지
	bool IsAIRunning() const;//AI구동여부 확인

	bool bIsAIRunning = false;//AI 가동 여부
	bool bIsTracking;//플레이어 추적여부

	FBlackboard::FKey BossStateBlackBoardKey;//블랙보드 키를 에디터에서 가져오는 구조체
};
