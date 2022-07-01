// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BossClearTargetVector.h"
#include "ShooterBossAIController.h"//Boss AI컨트롤러
#include "BehaviorTree/BlackboardComponent.h"//블랙보드 컴포넌트 
#include "ShooterBossCharacter.h"//AI 캐릭터 
#include "Shooter2Character.h"//캐릭터 

//AI 해당 위치로 이동 후 해당 위치의 정보를 BlackBoard에서 지움
UBTTask_BossClearTargetVector::UBTTask_BossClearTargetVector()
{
	NodeName = TEXT("BossClearTargetVector");//태스크 네임노드설정
}

//태스크 상태 반환
EBTNodeResult::Type UBTTask_BossClearTargetVector::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AShooterBossCharacter>(OwnerComp.GetAIOwner()->GetPawn());//AI 폰 반환
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;

	if (ControllingPawn)
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(AShooterBossAIController::TargetPosKey);//타깃위치 클리어
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}