// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Missle.h"
#include "ShooterBossCharacter.h"//����ĳ����
#include "ShooterBossAIController.h"//���� ��Ʈ�ѷ�
#include "DrawDebugHelpers.h"//����� �׸���

UBTTask_Missle::UBTTask_Missle()
{
	NodeName = TEXT("BossMissle");//�½�ũ ���ӳ�弳��
}

//AI ���� �̻��� �߻�
EBTNodeResult::Type UBTTask_Missle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto AICharacter = Cast<AShooterBossCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == AICharacter) return EBTNodeResult::Failed;

	AICharacter->Missile();
	return EBTNodeResult::Succeeded;//
}