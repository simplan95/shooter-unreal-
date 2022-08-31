// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"

#include "ShooterAIAnimInstance.h"//AI�ִ��ν��Ͻ�
#include "ShooterAIController.h"//AI ��Ʈ�ѷ�
#include "ShooterAICharacter.h"//AI ĳ����
#include "Shooter2Character.h"//Ÿ�� ĳ����
#include "ShooterGripGun.h"//���� ������ ��
#include "DrawDebugHelpers.h"//����� �׸���

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");//�½�ũ ���ӳ�弳��
}

//AI �̴Ͼ� ����
EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto AICharacter = Cast<AShooterAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == AICharacter) return EBTNodeResult::Failed;

	AICharacter->GetAIAnim()->SetIsAiming(true);//���̹� Ȱ��ȭ
	AICharacter->Walk();//�ȱ�ӵ� ����
	if (AICharacter->GetCanAttack())//��Ʈ�� ���°� �ƴҽ� ���ݰ���
	{
		AICharacter->Fire();
	}
	return EBTNodeResult::Succeeded;
}