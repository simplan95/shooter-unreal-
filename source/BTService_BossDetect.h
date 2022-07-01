// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "BehaviorTree/BTService.h"
#include "BTService_BossDetect.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER2_API UBTService_BossDetect : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_BossDetect();

protected://매 틱마다 호출
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;

	class AShooterBossAIController* BossAIController;
	class AShooterBossCharacter* BossAICharacter;

};
