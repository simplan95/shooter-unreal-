// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossClearTargetVector.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER2_API UBTTask_BossClearTargetVector : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_BossClearTargetVector();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;//태스크의 상태를 반환
};
