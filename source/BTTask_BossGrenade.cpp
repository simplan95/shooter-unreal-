// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BossGrenade.h"
#include "ShooterBossCharacter.h"//보스캐릭터
#include "ShooterBossAIController.h"//보스 컨트롤러
#include "Shooter2Character.h"//타깃 캐릭터
#include "DrawDebugHelpers.h"//디버그 그리기

//AI 보스 캐릭터의 수류탄 발사
UBTTask_BossGrenade::UBTTask_BossGrenade()
{
	NodeName = TEXT("BossGrenade");//태스크 네임노드설정
}

//태스크의 상태를 반환
EBTNodeResult::Type UBTTask_BossGrenade::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto AICharacter = Cast<AShooterBossCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == AICharacter) return EBTNodeResult::Failed;

	AICharacter->Grenade();
	return EBTNodeResult::Succeeded;//
}