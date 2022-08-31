// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearTargetVector.h"
#include "ShooterAIController.h"//AI컨트롤러
#include "ShooterAIEnemyController.h"//AI origin 컨트롤러 
#include "BehaviorTree/BlackboardComponent.h"//블랙보드 컴포넌트 
#include "ShooterAICharacter.h"//AI 캐릭터 인클루드
#include "Shooter2Character.h"//캐릭터 인클루드

UBTTask_ClearTargetVector::UBTTask_ClearTargetVector()
{
	NodeName = TEXT("ClearTargetVector");//태스크 네임노드설정
}

//AI 캐릭터 타깃 위치 클리어
EBTNodeResult::Type UBTTask_ClearTargetVector::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AShooterEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());//AI 폰 반환
	if (nullptr == ControllingPawn) return EBTNodeResult::Failed;

	if (ControllingPawn)
	{
		if (auto MinionPawn = Cast<AShooterAICharacter>(OwnerComp.GetAIOwner()->GetPawn()))
		{
			MinionPawn->Walk();//위치에 도달하면 워크 속도로
		}
		OwnerComp.GetBlackboardComponent()->ClearValue(AShooterAIEnemyController::TargetPosKey);//타깃위치 클리어
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}