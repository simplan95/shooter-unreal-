// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsinAttackRange.h"
#include "ShooterAIController.h"//AI ��Ʈ�ѷ� ��
#include "Shooter2Character.h"//ĳ���� 
#include "BehaviorTree/BlackboardComponent.h"//������ ������Ʈ 
#include "ShooterAICharacter.h"//AIĳ����

//AIĳ������ ���ݹ��� �Ǵ�
UBTDecorator_IsinAttackRange::UBTDecorator_IsinAttackRange()
{
	NodeName = TEXT("EnableAttack");
}


bool UBTDecorator_IsinAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& Ownercomp, uint8* NodeMemory)const
{
	bool bResult = Super::CalculateRawConditionValue(Ownercomp, NodeMemory);

	auto ControllingPawn = Cast<AShooterAICharacter>(Ownercomp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn) return false;

	auto Target = Cast<AShooter2Character>(Ownercomp.GetBlackboardComponent()->GetValueAsObject(AShooterAIController::TargetKey));
	if (nullptr == Target) return false;

	FVector Distance = Target->GetActorLocation() - ControllingPawn->GetActorLocation();

	bResult = (Target->GetDistanceTo(ControllingPawn) <= 1500.0f);
	return bResult;
}