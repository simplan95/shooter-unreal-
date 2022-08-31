// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearTargetVector.h"
#include "ShooterAIController.h"//AI��Ʈ�ѷ�
#include "ShooterAIEnemyController.h"//AI origin ��Ʈ�ѷ� 
#include "BehaviorTree/BlackboardComponent.h"//������ ������Ʈ 
#include "ShooterAICharacter.h"//AI ĳ���� ��Ŭ���
#include "Shooter2Character.h"//ĳ���� ��Ŭ���

UBTTask_ClearTargetVector::UBTTask_ClearTargetVector()
{
	NodeName = TEXT("ClearTargetVector");//�½�ũ ���ӳ�弳��
}

//AI ĳ���� Ÿ�� ��ġ Ŭ����
EBTNodeResult::Type UBTTask_ClearTargetVector::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AShooterEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());//AI �� ��ȯ
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;

	if (ControllingPawn)
	{
		if (auto MinionPawn = Cast<AShooterAICharacter>(OwnerComp.GetAIOwner()->GetPawn()))
		{
			MinionPawn->Walk();//��ġ�� �����ϸ� ��ũ �ӵ���
		}
		OwnerComp.GetBlackboardComponent()->ClearValue(AShooterAIEnemyController::TargetPosKey);//Ÿ����ġ Ŭ����
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}