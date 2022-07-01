// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BossGrenade.h"
#include "ShooterBossCharacter.h"//����ĳ����
#include "ShooterBossAIController.h"//���� ��Ʈ�ѷ�
#include "Shooter2Character.h"//Ÿ�� ĳ����
#include "DrawDebugHelpers.h"//����� �׸���

//AI ���� ĳ������ ����ź �߻�
UBTTask_BossGrenade::UBTTask_BossGrenade()
{
	NodeName = TEXT("BossGrenade");//�½�ũ ���ӳ�弳��
}

//�½�ũ�� ���¸� ��ȯ
EBTNodeResult::Type UBTTask_BossGrenade::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto AICharacter = Cast<AShooterBossCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == AICharacter) return EBTNodeResult::Failed;

	AICharacter->Grenade();
	return EBTNodeResult::Succeeded;//
}