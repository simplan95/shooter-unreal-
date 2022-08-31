// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsinAttackRange.h"
#include "ShooterAIController.h"//AI �̴Ͼ� ��Ʈ�ѷ� 
#include "ShooterAIEnemyController.h"//AI origin ��Ʈ�ѷ� 
#include "Shooter2Character.h"//ĳ���� ��Ŭ���
#include "BehaviorTree/BlackboardComponent.h"//������ ������Ʈ 
#include "ShooterAICharacter.h"//AIĳ����

UBTDecorator_IsinAttackRange::UBTDecorator_IsinAttackRange()
{
	NodeName = TEXT("EnableAttack");
}

//AI ���� ������ �ִ��� Ȯ��
bool UBTDecorator_IsinAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& Ownercomp, uint8* NodeMemory)const
{
	bool bResult = Super::CalculateRawConditionValue(Ownercomp, NodeMemory);

	auto ControllingPawn = Cast<AShooterEnemyCharacter>(Ownercomp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn) return false;

	auto Target = Cast<AShooter2Character>(Ownercomp.GetBlackboardComponent()->GetValueAsObject(AShooterAIEnemyController::TargetKey));
	if (nullptr == Target) return false;

	bResult = (Target->GetDistanceTo(ControllingPawn) <= distance);
	return bResult;
}