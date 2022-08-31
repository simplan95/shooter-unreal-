// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Detect.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER2_API UBTService_Detect : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_Detect();

protected://매 틱마다 호출
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;

	class AShooterAIController* AIController;
	class AShooterAICharacter* AICharacter;
};
