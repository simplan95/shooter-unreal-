// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BossClearTargetVector.h"
#include "ShooterBossAIController.h"//Boss AI��Ʈ�ѷ�
#include "BehaviorTree/BlackboardComponent.h"//������ ������Ʈ 
#include "ShooterBossCharacter.h"//AI ĳ���� 
#include "Shooter2Character.h"//ĳ���� 

//AI �ش� ��ġ�� �̵� �� �ش� ��ġ�� ������ BlackBoard���� ����
UBTTask_BossClearTargetVector::UBTTask_BossClearTargetVector()
{
	NodeName = TEXT("BossClearTargetVector");//�½�ũ ���ӳ�弳��
}

//�½�ũ ���� ��ȯ
EBTNodeResult::Type UBTTask_BossClearTargetVector::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AShooterBossCharacter>(OwnerComp.GetAIOwner()->GetPawn());//AI �� ��ȯ
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;

	if (ControllingPawn)
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(AShooterBossAIController::TargetPosKey);//Ÿ����ġ Ŭ����
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}