// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_BossDetect.h"
#include "ShooterBossCharacter.h"//보스캐릭
#include "ShooterBossAIController.h"//보스 캐릭터 컨트롤러
#include "Shooter2Character.h"//타깃캐릭터 
#include "BehaviorTree/BlackboardComponent.h"//블랙보드 컴포넌트 
#include "DrawDebugHelpers.h"//디버그 드로잉 범위 시각화

UBTService_BossDetect::UBTService_BossDetect()
{
	NodeName = TEXT("BossDetect");//노드네임지정
	Interval = 0.5f;//인터벌시간
}

//보스 탐지 실행
void UBTService_BossDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	BossAIController = Cast<AShooterBossAIController>(OwnerComp.GetAIOwner());//Owner AI 컨트롤러 가져오기
	BossAICharacter = Cast<AShooterBossCharacter>(BossAIController->GetPawn());//Owner AI 캐릭터 가져오기

	UWorld* World = BossAICharacter->GetWorld();
	FVector Center = BossAICharacter->GetActorLocation();
	float DetectRadius = 2500.0f;//탐지범위
	if (nullptr == World) return;

	TArray<FOverlapResult> OverLapResults;//TArray 자료구조로 오버랩 결과 인스턴스를 저장한다.
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, BossAICharacter);//콜리전 선언

	bool bResult = World->OverlapMultiByObjectType(//오브젝트 타입을 멀티 방식으로 탐지(생성할 지역에 탐색할 오브젝트타입을 검사)
		OverLapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,//탐색 감지에 사용할 트레이스 채널 정보
		FCollisionShape::MakeSphere(DetectRadius),//탐색에 사용할 기본 도형 정보, 구체, 캡슐, 박스를 사용
		CollisionQueryParam//콜리전 선언
	);

	if (bResult)//플레이어 판단 및 추적
	{
		for (auto OverLapResult : OverLapResults)
		{
			AShooter2Character* TargetCharacter = Cast<AShooter2Character>(OverLapResult.GetActor());//오버랩 당한 액터
			if (TargetCharacter && TargetCharacter->GetController()->IsPlayerController())//플레이어 컨트롤러 인지를 판단
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AShooterAIEnemyController::TargetKey, TargetCharacter);//탐지한 캐릭터의 위치값을 타겟키 값에 삽입
				BossAIController->TrackToTarget();//타겟 추적 활성화
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(AShooterAIEnemyController::TargetKey);//타깃을 클리어 
		BossAIController->StopTrack();//타겟추적 비활성화
	}
}