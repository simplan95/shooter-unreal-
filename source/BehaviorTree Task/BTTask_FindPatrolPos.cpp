// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "ShooterAIController.h"//AI컨트롤러
#include "ShooterAIEnemyController.h"//AI origin 컨트롤러 
#include "BehaviorTree/BlackboardComponent.h"//블랙보드 컴포넌트 
#include "NavigationSystem.h"//네비 시스템

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");//태스크 네임노드설정
}

//AI 정찰 위치 선정
EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();//AI 폰 반환
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());//폰의 현재위치 정보 확인
	if (nullptr == NavSystem) return EBTNodeResult::Failed;

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AShooterAIEnemyController::HomePosKey);//폰의 원점

	FNavLocation NextPatrol;//다음 위치

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 800.0f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AShooterAIEnemyController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}