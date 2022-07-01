// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_BossDetect.h"
#include "ShooterBossCharacter.h"//����ĳ��
#include "ShooterBossAIController.h"//���� 
#include "Shooter2Character.h"//Ÿ��ĳ���� 
#include "BehaviorTree/BlackboardComponent.h"//�������� ������Ʈ 
#include "DrawDebugHelpers.h"//����� ����� ������� ��Ŭ��� ���� �ð�ȭ

//���� ĳ������ �÷��̾� ĳ���� Ž��
UBTService_BossDetect::UBTService_BossDetect()
{
	NodeName = TEXT("BossDetect");//����������
	Interval = 0.5f;//���͹��ð�
}

void UBTService_BossDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	BossAIController = Cast<AShooterBossAIController>(OwnerComp.GetAIOwner());//Owner AI ��Ʈ�ѷ� ��������
	BossAICharacter = Cast<AShooterBossCharacter>(BossAIController->GetPawn());//Owner AI ĳ���� ��������

	UWorld* World = BossAICharacter->GetWorld();
	FVector Center = BossAICharacter->GetActorLocation();
	float DetectRadius = 2500.0f;//Ž������
	if (nullptr == World) return;

	TArray<FOverlapResult> OverLapResults;//TArray �ڷᱸ���� ������ ��� �ν��Ͻ��� �����Ѵ�.
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, BossAICharacter);//�ݸ��� ����

	bool bResult = World->OverlapMultiByObjectType(//������Ʈ Ÿ���� ��Ƽ ������� Ž��(������ ������ Ž���� ������ƮŸ���� �˻�)
		OverLapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,//Ž�� ������ ����� Ʈ���̽� ä�� ����
		FCollisionShape::MakeSphere(DetectRadius),//Ž���� ����� �⺻ ���� ����, ��ü, ĸ��, �ڽ��� ���
		CollisionQueryParam//�ݸ��� ����
	);

	if (bResult)//�÷��̾� �Ǵ� �� ����
	{
		for (auto OverLapResult : OverLapResults)
		{
			AShooter2Character* TargetCharacter = Cast<AShooter2Character>(OverLapResult.GetActor());//������ ���� ����
			if (TargetCharacter && TargetCharacter->GetController()->IsPlayerController())//�÷��̾� ��Ʈ�ѷ� ������ �Ǵ�
			{
				//Ž���� ĳ������ ��ġ���� Ÿ��Ű ���� ����
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AShooterBossAIController::TargetKey, TargetCharacter);
				BossAIController->TrackToTarget();//Ÿ�� ���� Ȱ��ȭ
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(AShooterBossAIController::TargetPosKey);//Ÿ���� Ŭ����
		BossAIController->StopTrack();//Ÿ������ ��Ȱ��ȭ
	}
}