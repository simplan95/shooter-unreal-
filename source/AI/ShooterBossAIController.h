// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "ShooterAIEnemyController.h"
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
class SHOOTER2_API AShooterBossAIController : public AShooterAIEnemyController
{
	GENERATED_BODY()

public:
	AShooterBossAIController();

	//빙의
	virtual void OnPossess(APawn* InPawn) override;

	//틱
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY()
		class AShooterBossCharacter* AICharacter;//AI가 조종하는 캐릭터
	
public:
	UPROPERTY()
	EBossState BossPattern;//보스 패턴

	FBlackboard::FKey BossStateBlackBoardKey;//블랙보드 키를 에디터에서 가져오는 구조체

	static const FName AttackPattern;
	bool IsAIRunning() const;//AI구동여부 확인
	bool bIsAIRunning = false;//AI 가동 여부
};
