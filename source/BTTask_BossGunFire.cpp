// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BossGunFire.h"
#include "ShooterBossCharacter.h"//보스캐릭터
#include "ShooterBossAIController.h"//보스 컨트롤러
#include "Shooter2Character.h"//타깃 캐릭터
#include "ShooterGripGun.h"//장착 아이템 총
#include "DrawDebugHelpers.h"//디버그 그리기

//AI보스 캐릭터의 총 발사
UBTTask_BossGunFire::UBTTask_BossGunFire()
{
	NodeName = TEXT("BossFire");//태스크 네임노드설정
}

//태스크의 상태를 반환
EBTNodeResult::Type UBTTask_BossGunFire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto AICharacter = Cast<AShooterBossCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == AICharacter) return EBTNodeResult::Failed;

	AICharacter->Fire();
	return EBTNodeResult::Succeeded;//
}