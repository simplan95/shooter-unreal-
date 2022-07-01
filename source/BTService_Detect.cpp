// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "ShooterAIController.h"//AI 컨트롤러 
#include "ShooterAICharacter.h"//AI 캐릭터 
#include "Shooter2Character.h"//캐릭터 인
#include "BehaviorTree/BlackboardComponent.h"//블랙보드 컴포넌트 
#include "DrawDebugHelpers.h"//디버그 드로잉 헤더파일 인클루딩 범위 시각화
#include "ShooterAIAnimInstance.h"//AI애님 인스턴스

//AI 캐릭터의 플레이어 캐릭터 탐지
UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");//노드네임지정
	Interval = 0.5f;//인터벌시간
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AIController = Cast<AShooterAIController>(OwnerComp.GetAIOwner());//Owner AI 컨트롤러 가져오기
	AICharacter=Cast<AShooterAICharacter>(AIController->GetPawn());//Owner AI 캐릭터 가져오기

	FVector Box = FVector(100.0f,600.0f,600.0f);//탐지박스영역
	float EndRange = 2000.0f;

	UWorld* World = AICharacter->GetWorld();
	FVector Center = AICharacter->GetDtectPoint();

	TArray<FOverlapResult> OverLapResults;//TArray 자료구조로 오버랩 결과 인스턴스를 저장한다.
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, AICharacter);//콜리전 선언

	TArray<FHitResult> HitResults;

	bool bResult = World->SweepMultiByObjectType(
		HitResults,
		Center,//탐색을 시작할 위치
		Center + AICharacter->GetActorForwardVector() * EndRange,//탐색을 끝낼 위치
		FQuat::Identity,//탐색에 사용할 도형의 회전
		ECollisionChannel::ECC_GameTraceChannel1,//물리충돌 감지에 사용할 트레이스 채널 정보
		FCollisionShape::MakeBox(Box),//탐색에 사용할 기본 도형 정보, 구체, 캡슐, 박스를 사용
		CollisionQueryParam
	);
	
	if (bResult)//플레이어 판단 및 추적(박스 크기 만큼 탐지후 결과를 반환)
	{
		for (auto HitResult : HitResults)//탐지된 플레이어들을 하나씩 검사
		{
			FCollisionObjectQueryParams ObjQueryLine;//공격 받는 인스턴스 집합(배열)
			ObjQueryLine.AddObjectTypesToQuery(ECC_GameTraceChannel4);//트레이스 채널 설정 추가 (블록)

			FCollisionQueryParams ColQuery;//실행 주체 인스턴스
			ColQuery.AddIgnoredActor(AICharacter);//실행 주체 액터는 공격 무시

			FHitResult HitLineRes;//히트 확인 인스턴스
			AShooter2Character* TargetCharacter = Cast<AShooter2Character>(HitResult.GetActor());//오버랩 당한 액터를 플레이어 캐릭터로변환

			//라인 트레이스로 히트확인검사
			bool HitLineResult = GetWorld()->LineTraceSingleByObjectType(HitLineRes, Center, TargetCharacter->GetTargetPoint(), ObjQueryLine, ColQuery);

			if (!HitLineResult)//HitRes.bBlockingHit)//라인트레이스를 했을 대 그사이에 블록 요소가 없을 시 
			{ 
				//플레이어 컨트롤러 인지를 판단
				if (TargetCharacter && TargetCharacter->GetController()->IsPlayerController())
				{
					AIController->TrackToTargetOutRange(HitResult.Actor->GetActorLocation());

					//탐지한 캐릭터의 위치값을 타겟키 값에 삽입
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(AShooterAIController::TargetKey, TargetCharacter);
					
					AIController->TrackToTarget();//타겟 추적 활성화
					if (AICharacter->GetDistanceTo(TargetCharacter) >= 1000.0f)//일정거리 이상 멀어질시 달리기
					{
						AICharacter->Run();//AI 달리기 속도로
						AICharacter->GetAIAnim()->SetIsAiming(false);//에임 Off
					}
					else
					{
						AICharacter->Walk();//AI 걷기 속도로
						AICharacter->GetAIAnim()->SetIsAiming(true);//에임 On
					}
					UE_LOG(Shooter2, Error, TEXT("Detected!!"));
				}
			}
			else if (HitLineResult)
			{

				OwnerComp.GetBlackboardComponent()->ClearValue(AShooterAIController::TargetKey);//타깃을 클리어
				AIController->StopTrack();//타겟추적 비활성화

				//타깃위치키가 유효하지 않으면 걷기 속도로
				if (!OwnerComp.GetBlackboardComponent()->IsVectorValueSet(AShooterAIController::TargetPosKey))
				{
					AICharacter->Walk();
					AICharacter->GetAIAnim()->SetIsAiming(false);//에임 Off
				}
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(AShooterAIController::TargetKey);//타깃을 클리어
		AIController->StopTrack();//타겟추적 비활성화

		//타깃위치키가 유효하지 않으면 걷기 속도로
		if (!OwnerComp.GetBlackboardComponent()->IsVectorValueSet(AShooterAIController::TargetPosKey))
		{
			AICharacter->Walk();
			AICharacter->GetAIAnim()->SetIsAiming(false);//에임 Off
		}
		
	}
}