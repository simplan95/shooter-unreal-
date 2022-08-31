// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterBossAIController.h"
#include "BehaviorTree/BehaviorTree.h"//�����̺�� Ʈ�� 
#include "BehaviorTree/BlackboardData.h"//������ 
#include "BehaviorTree/BlackboardComponent.h"//������ ������Ʈ 

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"//������ Ű ��Ÿ�� 
#include "Shooter2Character.h"//�÷��̾� ĳ����
#include "ShooterBossCharacter.h"//���� ĳ����

const FName AShooterBossAIController::AttackPattern(TEXT("AttackEnum"));//��������

//AI ���� ��Ʈ�ѷ�
AShooterBossAIController::AShooterBossAIController()//������
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));//������ ������Ʈ �ʱ�ȭ

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/MyAsset/BOSS/BB_BossBlackBoard.BB_BossBlackBoard"));//������ ����
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/MyAsset/BOSS/BT_BossTree.BT_BossTree"));//�ൿƮ�� ����
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

}


void AShooterBossAIController::OnPossess(APawn* InPawn)//��Ʈ�ѷ��� ���� ���ǽ�
{
	Super::OnPossess(InPawn);

	AICharacter = Cast<AShooterBossCharacter>(InPawn);//AI ĳ���͸� ���� ����(AShooterBossCharacter �Լ��� �������� ������)

	BlackboardComp->InitializeBlackboard(*BTAsset->BlackboardAsset);//������ �����͸� �ʱ�ȭ

	//Key �� ����
	BossStateBlackBoardKey = BlackboardComp->GetKeyID(AttackPattern);//�𸮾� �����Ϳ� ������ ������ ������ ������
}


void AShooterBossAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//AI�� ���� Ȱ��ȭ�� Ÿ��ĳ���� �������� ȸ��
	if (bIsTracking && TargetCharacter != nullptr)
	{
		FVector CharLoc = AICharacter->GetActorLocation();//����ĳ���� ��ģ
		FVector DirToTarget = TargetCharacter->GetActorLocation() - CharLoc;//Ÿ�� ĳ���� ����
		DirToTarget.Z = 0.0f;//��ġ ȸ���� ��ȿ�� �ϱ����� Z�� 0����
		FRotator RotToTarget = FRotationMatrix::MakeFromX(DirToTarget).Rotator();
		//AI ĳ���Ͱ� ����� Ȯ���� �� ����� ���� ȸ�� ��ġ���� ��ȿ
		AICharacter->SetActorRotation(FMath::RInterpTo(AICharacter->GetActorRotation(), RotToTarget, GetWorld()->GetDeltaSeconds(), 15.0f));
	}

}

bool AShooterBossAIController::IsAIRunning() const//AI �������� ��ȯ
{
	return bIsAIRunning;
}