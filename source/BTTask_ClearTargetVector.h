// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ClearTargetVector.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER2_API UBTTask_ClearTargetVector : public UBTTaskNode
{
	GENERATED_BODY()
	

public:
	UBTTask_ClearTargetVector();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;//태스크의 상태를 반환
};
