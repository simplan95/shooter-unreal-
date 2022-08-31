// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossGrenade.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER2_API UBTTask_BossGrenade : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_BossGrenade();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;//태스크의 상태를 반환
};
