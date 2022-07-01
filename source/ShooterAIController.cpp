// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterAIController.h"
#include "BehaviorTree/BehaviorTree.h"//비헤이비어 트리 
#include "BehaviorTree/BlackboardData.h"//블랙보드 
#include "BehaviorTree/BlackboardComponent.h"//블랙보드 컴포넌트 

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"//블랙보드 키 올타입 
#include "Shooter2Character.h"//플레이어 캐릭터
#include "ShooterAICharacter.h"//AI캐릭터
#include "ShooterAIAnimInstance.h"//AI애님

//AI 캐릭터를 컨트롤하는 컨트롤러

const FName AShooterAIController::HomePosKey(TEXT("HomeKey"));
const FName AShooterAIController::PatrolPosKey(TEXT("PatrolKey"));
const FName AShooterAIController::TargetKey(TEXT("TargetObj"));
const FName AShooterAIController::TargetPosKey(TEXT("TargetVec"));

AShooterAIController::AShooterAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));//블랙보드 컴포넌트 초기화

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/MyAsset/AI/BB_AIBlackBoard.BB_AIBlackBoard"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/MyAsset/AI/BT_AIBehaviorTree.BT_AIBehaviorTree"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

}


void AShooterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AICharacter = Cast<AShooterAICharacter>(InPawn);//AI 캐릭터를 설정 빙의(AShooterAICharacter 함수를 쓰기위해 설정함)
	AIAnim = Cast<UShooterAIAnimInstance>(AICharacter->GetMesh()->GetAnimInstance());//애님 인스턴스

	BlackboardComp->InitializeBlackboard(*BTAsset->BlackboardAsset);//블랙보드 데이터를 초기화
}


void AShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//AI가 추적 활성화시 타깃캐릭터 방향으로 회전
	if (bIsTracking && TargetCharacter != nullptr)
	{
		FVector CharLoc = AICharacter->GetActorLocation();
		FVector DirToTarget = TargetCharacter->GetActorLocation() - CharLoc;
		DirToTarget.Z = 0.0f;//피치 회전은 무효로 하기위해 Z를 0으로
		FRotator RotToTarget = FRotationMatrix::MakeFromX(DirToTarget).Rotator();
		//AI 캐릭터가 대상을 확인할 시 대상을 향해 회전 피치축은 무효
		AICharacter->SetActorRotation(FMath::RInterpTo(AICharacter->GetActorRotation(), RotToTarget, GetWorld()->GetDeltaSeconds(), 15.0f));

		//AI캐릭터가 타깃을 향해 AimPitch를 추적
		FVector Pitch = TargetCharacter->GetActorLocation() - CharLoc;//플레이어캐릭터를 바라보는 방향
		FRotator PitchRotation = FRotationMatrix::MakeFromX(Pitch).Rotator();
		AIAnim->AimPitch = FMath::ClampAngle(AIAnim->FinRotation.Pitch, -90.0f, 90.0f);//Pitch축 범위
		AIAnim->CurRotation = FRotator(AIAnim->AimPitch, 0.0f, 0.0f);//현재 로테이션값
		AIAnim->FinRotation = FMath::RInterpTo(AIAnim->CurRotation, PitchRotation, DeltaTime, 20.0f);//현재 로테이션을 타깃 로테이션으로 
	}
	else
	{
		AIAnim->AimPitch = AIAnim->FinRotation.Pitch;
		AIAnim->CurRotation = FRotator(AIAnim->AimPitch, 0.0f, 0.0f);//현재 로테이션값
		AIAnim->FinRotation = FMath::RInterpTo(AIAnim->CurRotation, FRotator::ZeroRotator, DeltaTime, 5.0f);//현재 로테이션을 타깃 로테이션으로 
	}

}
void AShooterAIController::RunAI()//AI 구동
{
	if (UseBlackboard(BBAsset, BlackboardComp))//블랙보드 사용 
	{
		RunBehaviorTree(BTAsset);
		BlackboardComp->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());//현재 NPC의 위치를 홈포지션에 대입
	}
}

void AShooterAIController::StopAI()//AI 정지
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}


void AShooterAIController::TrackToTarget()//타깃 추격시작
{
	TargetCharacter = Cast<AShooter2Character>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(TargetKey));
	if (TargetCharacter != nullptr)
	{
		bIsTracking = true;
	}
}

void AShooterAIController::StopTrack()//추격 중지
{
	bIsTracking = false;
	TargetCharacter = nullptr;
}

void AShooterAIController::TrackToTargetOutRange(FVector TargetLocation)//탐지 범위 밖에 있는 플레이어 추적
{
	GetBlackboardComponent()->SetValueAsVector(AShooterAIController::TargetPosKey, TargetLocation);//탐지한 캐릭터의 위치값을 타겟키 값에 삽입
	AICharacter->Run();//지정위치까지 달려감
}
