// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Missle.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER2_API UBTTask_Missle : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_Missle();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;//�½�ũ�� ���¸� ��ȯ
};
