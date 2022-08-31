// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsinAttackRange.h"
#include "ShooterAIController.h"//AI 미니언 컨트롤러 
#include "ShooterAIEnemyController.h"//AI origin 컨트롤러 
#include "Shooter2Character.h"//캐릭터 인클루드
#include "BehaviorTree/BlackboardComponent.h"//블랙보드 컴포넌트 
#include "ShooterAICharacter.h"//AI캐릭터

UBTDecorator_IsinAttackRange::UBTDecorator_IsinAttackRange()
{
	NodeName = TEXT("EnableAttack");
}

//AI 공격 범위에 있는지 확인
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