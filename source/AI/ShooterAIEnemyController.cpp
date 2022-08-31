// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIEnemyController.h"
#include "Shooter2Character.h"//플레이어 캐릭터
#include "BehaviorTree/BehaviorTree.h"//비헤이비어 트리 
#include "BehaviorTree/BlackboardData.h"//블랙보드 
#include "BehaviorTree/BlackboardComponent.h"//블랙보드 컴포넌트 

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"//블랙보드 키 올타입 인클루딩

const FName AShooterAIEnemyController::HomePosKey(TEXT("HomeKey"));//블랙보드 변수이름 
const FName AShooterAIEnemyController::PatrolPosKey(TEXT("PatrolKey"));//블랙보드 변수이름 
const FName AShooterAIEnemyController::TargetKey(TEXT("TargetObj"));//블랙보드 변수이름 
const FName AShooterAIEnemyController::TargetPosKey(TEXT("TargetVec"));//블랙보드 변수이름 

//AI 캐릭터 컨트롤러(origin)
AShooterAIEnemyController::AShooterAIEnemyController()//생성자
{
}


void AShooterAIEnemyController::OnPossess(APawn* InPawn)//컨트롤러가 폰에 빙의시
{
	Super::OnPossess(InPawn);
}


void AShooterAIEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShooterAIEnemyController::RunAI()//AI 구동
{

	if (UseBlackboard(BBAsset, Blackboard))//블랙보드 사용 
	{
		RunBehaviorTree(BTAsset);//행동트리 가동
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());//현재 NPC의 위치를 홈포지션에 대입
	}
}

void AShooterAIEnemyController::StopAI()//AI 정지
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

void AShooterAIEnemyController::TrackToTarget()//타깃 트래킹 활성화
{
	TargetCharacter = Cast<AShooter2Character>(Blackboard->GetValue<UBlackboardKeyType_Object>(TargetKey));
	if (TargetCharacter != nullptr)
	{
		bIsTracking = true;
	}
}

void AShooterAIEnemyController::StopTrack()//타깃 트래킹 비활성화
{
	bIsTracking = false;
	TargetCharacter = nullptr;
}

void AShooterAIEnemyController::TrackToTargetOutRange(FVector TargetLocation)//캐릭터 탐지범위 밖에서 타깃을 쫓아감
{
	Blackboard->SetValueAsVector(AShooterAIEnemyController::TargetPosKey, TargetLocation);//탐지한 캐릭터의 위치값을 타겟키 값에 삽입
}