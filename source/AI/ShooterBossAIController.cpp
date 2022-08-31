// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterBossAIController.h"
#include "BehaviorTree/BehaviorTree.h"//비헤이비어 트리 
#include "BehaviorTree/BlackboardData.h"//블랙보드 
#include "BehaviorTree/BlackboardComponent.h"//블랙보드 컴포넌트 

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"//블랙보드 키 올타입 
#include "Shooter2Character.h"//플레이어 캐릭터
#include "ShooterBossCharacter.h"//보스 캐릭터

const FName AShooterBossAIController::AttackPattern(TEXT("AttackEnum"));//공격패턴

//AI 보스 컨트롤러
AShooterBossAIController::AShooterBossAIController()//생성자
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));//블랙보드 컴포넌트 초기화

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/MyAsset/BOSS/BB_BossBlackBoard.BB_BossBlackBoard"));//블랙보드 에셋
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/MyAsset/BOSS/BT_BossTree.BT_BossTree"));//행동트리 에셋
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

}


void AShooterBossAIController::OnPossess(APawn* InPawn)//컨트롤러가 폰에 빙의시
{
	Super::OnPossess(InPawn);

	AICharacter = Cast<AShooterBossCharacter>(InPawn);//AI 캐릭터를 설정 빙의(AShooterBossCharacter 함수를 쓰기위해 설정함)

	BlackboardComp->InitializeBlackboard(*BTAsset->BlackboardAsset);//블랙보드 데이터를 초기화

	//Key 값 설정
	BossStateBlackBoardKey = BlackboardComp->GetKeyID(AttackPattern);//언리얼 에디터에 설정한 블랙보드 변수를 가져옴
}


void AShooterBossAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//AI가 추적 활성화시 타깃캐릭터 방향으로 회전
	if (bIsTracking && TargetCharacter != nullptr)
	{
		FVector CharLoc = AICharacter->GetActorLocation();//보스캐릭터 위친
		FVector DirToTarget = TargetCharacter->GetActorLocation() - CharLoc;//타깃 캐릭터 방향
		DirToTarget.Z = 0.0f;//피치 회전은 무효로 하기위해 Z를 0으로
		FRotator RotToTarget = FRotationMatrix::MakeFromX(DirToTarget).Rotator();
		//AI 캐릭터가 대상을 확인할 시 대상을 향해 회전 피치축은 무효
		AICharacter->SetActorRotation(FMath::RInterpTo(AICharacter->GetActorRotation(), RotToTarget, GetWorld()->GetDeltaSeconds(), 15.0f));
	}

}

bool AShooterBossAIController::IsAIRunning() const//AI 구동여부 반환
{
	return bIsAIRunning;
}