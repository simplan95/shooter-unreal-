// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIEnemyController.h"
#include "Shooter2Character.h"//�÷��̾� ĳ����
#include "BehaviorTree/BehaviorTree.h"//�����̺�� Ʈ�� 
#include "BehaviorTree/BlackboardData.h"//������ 
#include "BehaviorTree/BlackboardComponent.h"//������ ������Ʈ 

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"//������ Ű ��Ÿ�� ��Ŭ���

const FName AShooterAIEnemyController::HomePosKey(TEXT("HomeKey"));//������ �����̸� 
const FName AShooterAIEnemyController::PatrolPosKey(TEXT("PatrolKey"));//������ �����̸� 
const FName AShooterAIEnemyController::TargetKey(TEXT("TargetObj"));//������ �����̸� 
const FName AShooterAIEnemyController::TargetPosKey(TEXT("TargetVec"));//������ �����̸� 

//AI ĳ���� ��Ʈ�ѷ�(origin)
AShooterAIEnemyController::AShooterAIEnemyController()//������
{
}


void AShooterAIEnemyController::OnPossess(APawn* InPawn)//��Ʈ�ѷ��� ���� ���ǽ�
{
	Super::OnPossess(InPawn);
}


void AShooterAIEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShooterAIEnemyController::RunAI()//AI ����
{

	if (UseBlackboard(BBAsset, Blackboard))//������ ��� 
	{
		RunBehaviorTree(BTAsset);//�ൿƮ�� ����
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());//���� NPC�� ��ġ�� Ȩ�����ǿ� ����
	}
}

void AShooterAIEnemyController::StopAI()//AI ����
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

void AShooterAIEnemyController::TrackToTarget()//Ÿ�� Ʈ��ŷ Ȱ��ȭ
{
	TargetCharacter = Cast<AShooter2Character>(Blackboard->GetValue<UBlackboardKeyType_Object>(TargetKey));
	if (TargetCharacter != nullptr)
	{
		bIsTracking = true;
	}
}

void AShooterAIEnemyController::StopTrack()//Ÿ�� Ʈ��ŷ ��Ȱ��ȭ
{
	bIsTracking = false;
	TargetCharacter = nullptr;
}

void AShooterAIEnemyController::TrackToTargetOutRange(FVector TargetLocation)//ĳ���� Ž������ �ۿ��� Ÿ���� �Ѿư�
{
	Blackboard->SetValueAsVector(AShooterAIEnemyController::TargetPosKey, TargetLocation);//Ž���� ĳ������ ��ġ���� Ÿ��Ű ���� ����
}