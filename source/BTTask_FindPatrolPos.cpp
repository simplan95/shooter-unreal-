// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "ShooterAIController.h"//AI��Ʈ�ѷ�
#include "BehaviorTree/BlackboardComponent.h"//������ ������Ʈ ��Ŭ���
#include "NavigationSystem.h"//�׺� �ý���

//AI ĳ������ ���� �̵� ���� ����
UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");//�½�ũ ���ӳ�弳��
}

//�½�ũ ���� ��ȯ
EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();//AI �� ��ȯ
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());//���� ������ġ ���� Ȯ��
	if (nullptr == NavSystem) return EBTNodeResult::Failed;

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AShooterAIController::HomePosKey);//���� ����

	FNavLocation NextPatrol;//���� ��ġ

	//�ش� AI �ֺ� (���� 800) ���� ��ġ ��ȯ
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 800.0f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AShooterAIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}