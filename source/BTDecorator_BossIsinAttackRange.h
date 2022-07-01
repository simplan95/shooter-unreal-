// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_BossIsinAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER2_API UBTDecorator_BossIsinAttackRange : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_BossIsinAttackRange();

protected://조건달성을 확인
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& Ownercomp, uint8* NodeMemory) const override;
};
