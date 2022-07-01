// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_BossPattern.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER2_API UBTService_BossPattern : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_BossPattern();

	class AShooterBossAIController* BossAIController;
	class AShooterBossCharacter* BossAICharacter;

protected://매 틱마다 호출
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;
};
