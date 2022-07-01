// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_BossIsinAttackRange.h"
#include "ShooterBossAIController.h"//Boss AI 컨트롤러 
#include "Shooter2Character.h"//캐릭터 
#include "BehaviorTree/BlackboardComponent.h"//블랙보드 컴포넌트 
#include "ShooterBossCharacter.h"//Boss AI캐릭터


//AI캐릭터의 공격범위 판단
UBTDecorator_BossIsinAttackRange::UBTDecorator_BossIsinAttackRange()
{
	NodeName = TEXT("BossEnableAttack");
}


bool UBTDecorator_BossIsinAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& Ownercomp, uint8* NodeMemory)const
{
	bool bResult = Super::CalculateRawConditionValue(Ownercomp, NodeMemory);

	auto ControllingPawn = Cast<AShooterBossCharacter>(Ownercomp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn) return false;

	auto Target = Cast<AShooter2Character>(Ownercomp.GetBlackboardComponent()->GetValueAsObject(AShooterBossAIController::TargetKey));
	if (nullptr == Target) return false;

	bResult = (Target->GetDistanceTo(ControllingPawn) <= 2000.0f);
	return bResult;
}