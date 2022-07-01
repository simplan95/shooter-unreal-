// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearTargetVector.h"
#include "ShooterAIController.h"//AI��Ʈ�ѷ�
#include "BehaviorTree/BlackboardComponent.h"//������ ������Ʈ 
#include "ShooterAICharacter.h"//AI ĳ���� 
#include "Shooter2Character.h"//ĳ���� 

//AI �ش� ��ġ�� �̵� �� �ش� ��ġ�� ������ BlackBoard���� ����
UBTTask_ClearTargetVector::UBTTask_ClearTargetVector()
{
	NodeName = TEXT("ClearTargetVector");//�½�ũ ���ӳ�弳��
}

//�½�ũ ���� ��ȯ
EBTNodeResult::Type UBTTask_ClearTargetVector::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AShooterAICharacter>(OwnerComp.GetAIOwner()->GetPawn());//AI �� ��ȯ
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;

	if (ControllingPawn)
	{
		ControllingPawn->Walk();//��ġ�� �����ϸ� ��ũ �ӵ���
		OwnerComp.GetBlackboardComponent()->ClearValue(AShooterAIController::TargetPosKey);//Ÿ����ġ Ŭ����
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}