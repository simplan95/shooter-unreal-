// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"

#include "ShooterAIAnimInstance.h"//AI애님인스턴스
#include "ShooterAIController.h"//AI 컨트롤러
#include "ShooterAICharacter.h"//AI 캐릭터
#include "Shooter2Character.h"//타깃 캐릭터
#include "ShooterGripGun.h"//장착 아이템 총
#include "DrawDebugHelpers.h"//디버그 그리기

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");//태스크 네임노드설정
}

//AI 미니언 공격
EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto AICharacter = Cast<AShooterAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == AICharacter) return EBTNodeResult::Failed;

	AICharacter->GetAIAnim()->SetIsAiming(true);//에이밍 활성화
	AICharacter->Walk();//걷기속도 유지
	if (AICharacter->GetCanAttack())//히트한 상태가 아닐시 공격가능
	{
		AICharacter->Fire();
	}
	return EBTNodeResult::Succeeded;
}