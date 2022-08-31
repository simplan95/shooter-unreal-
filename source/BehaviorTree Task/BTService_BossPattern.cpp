// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_BossPattern.h"
#include "ShooterBossCharacter.h"//보스캐릭
#include "ShooterBossAIController.h"//보스 깐트롤러
#include "Shooter2Character.h"//타깃캐릭터 인클루드
#include "BehaviorTree/BlackboardComponent.h"//블랙보드 컴포넌트 인클루딩
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"//블랙보드 키 올타입 인클루딩

UBTService_BossPattern::UBTService_BossPattern()
{
	NodeName = TEXT("BossPattern");//노드네임지정
	Interval = 5.0f;//인터벌시간
}

//보스 패턴 설정
void UBTService_BossPattern::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	BossAIController = Cast<AShooterBossAIController>(OwnerComp.GetAIOwner());//Owner AI 컨트롤러 가져오기
	BossAICharacter = Cast<AShooterBossCharacter>(BossAIController->GetPawn());//Owner AI 캐릭터 가져오기

	int32 i = FMath::RandRange(1, 10);
	if (i < 5)
	{
		BossAIController->BossPattern = EBossState::BS_MISSILE;
		UE_LOG(Shooter2, Error, TEXT("Missile!!"));
	}
	else if (5 <= i && i < 8)
	{
		BossAIController->BossPattern = EBossState::BS_FIRE;
		UE_LOG(Shooter2, Error, TEXT("Fire!!"));
	}
	else if (8 <= i && i <= 10)
	{
		BossAIController->BossPattern = EBossState::BS_GRENADE;
		UE_LOG(Shooter2, Error, TEXT("Grenade!!"));
	}
	
	//i의 결과값에 따라 Enum값을 지정
	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(BossAIController->BossStateBlackBoardKey, (uint8)BossAIController->BossPattern);
}