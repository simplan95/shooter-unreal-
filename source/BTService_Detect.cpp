// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "ShooterAIController.h"//AI ��Ʈ�ѷ� 
#include "ShooterAICharacter.h"//AI ĳ���� 
#include "Shooter2Character.h"//ĳ���� ��
#include "BehaviorTree/BlackboardComponent.h"//������ ������Ʈ 
#include "DrawDebugHelpers.h"//����� ����� ������� ��Ŭ��� ���� �ð�ȭ
#include "ShooterAIAnimInstance.h"//AI�ִ� �ν��Ͻ�

//AI ĳ������ �÷��̾� ĳ���� Ž��
UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");//����������
	Interval = 0.5f;//���͹��ð�
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AIController = Cast<AShooterAIController>(OwnerComp.GetAIOwner());//Owner AI ��Ʈ�ѷ� ��������
	AICharacter=Cast<AShooterAICharacter>(AIController->GetPawn());//Owner AI ĳ���� ��������

	FVector Box = FVector(100.0f,600.0f,600.0f);//Ž���ڽ�����
	float EndRange = 2000.0f;

	UWorld* World = AICharacter->GetWorld();
	FVector Center = AICharacter->GetDtectPoint();

	TArray<FOverlapResult> OverLapResults;//TArray �ڷᱸ���� ������ ��� �ν��Ͻ��� �����Ѵ�.
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, AICharacter);//�ݸ��� ����

	TArray<FHitResult> HitResults;

	bool bResult = World->SweepMultiByObjectType(
		HitResults,
		Center,//Ž���� ������ ��ġ
		Center + AICharacter->GetActorForwardVector() * EndRange,//Ž���� ���� ��ġ
		FQuat::Identity,//Ž���� ����� ������ ȸ��
		ECollisionChannel::ECC_GameTraceChannel1,//�����浹 ������ ����� Ʈ���̽� ä�� ����
		FCollisionShape::MakeBox(Box),//Ž���� ����� �⺻ ���� ����, ��ü, ĸ��, �ڽ��� ���
		CollisionQueryParam
	);
	
	if (bResult)//�÷��̾� �Ǵ� �� ����(�ڽ� ũ�� ��ŭ Ž���� ����� ��ȯ)
	{
		for (auto HitResult : HitResults)//Ž���� �÷��̾���� �ϳ��� �˻�
		{
			FCollisionObjectQueryParams ObjQueryLine;//���� �޴� �ν��Ͻ� ����(�迭)
			ObjQueryLine.AddObjectTypesToQuery(ECC_GameTraceChannel4);//Ʈ���̽� ä�� ���� �߰� (���)

			FCollisionQueryParams ColQuery;//���� ��ü �ν��Ͻ�
			ColQuery.AddIgnoredActor(AICharacter);//���� ��ü ���ʹ� ���� ����

			FHitResult HitLineRes;//��Ʈ Ȯ�� �ν��Ͻ�
			AShooter2Character* TargetCharacter = Cast<AShooter2Character>(HitResult.GetActor());//������ ���� ���͸� �÷��̾� ĳ���ͷκ�ȯ

			//���� Ʈ���̽��� ��ƮȮ�ΰ˻�
			bool HitLineResult = GetWorld()->LineTraceSingleByObjectType(HitLineRes, Center, TargetCharacter->GetTargetPoint(), ObjQueryLine, ColQuery);

			if (!HitLineResult)//HitRes.bBlockingHit)//����Ʈ���̽��� ���� �� �׻��̿� ��� ��Ұ� ���� �� 
			{ 
				//�÷��̾� ��Ʈ�ѷ� ������ �Ǵ�
				if (TargetCharacter && TargetCharacter->GetController()->IsPlayerController())
				{
					AIController->TrackToTargetOutRange(HitResult.Actor->GetActorLocation());

					//Ž���� ĳ������ ��ġ���� Ÿ��Ű ���� ����
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(AShooterAIController::TargetKey, TargetCharacter);
					
					AIController->TrackToTarget();//Ÿ�� ���� Ȱ��ȭ
					if (AICharacter->GetDistanceTo(TargetCharacter) >= 1000.0f)//�����Ÿ� �̻� �־����� �޸���
					{
						AICharacter->Run();//AI �޸��� �ӵ���
						AICharacter->GetAIAnim()->SetIsAiming(false);//���� Off
					}
					else
					{
						AICharacter->Walk();//AI �ȱ� �ӵ���
						AICharacter->GetAIAnim()->SetIsAiming(true);//���� On
					}
					UE_LOG(Shooter2, Error, TEXT("Detected!!"));
				}
			}
			else if (HitLineResult)
			{

				OwnerComp.GetBlackboardComponent()->ClearValue(AShooterAIController::TargetKey);//Ÿ���� Ŭ����
				AIController->StopTrack();//Ÿ������ ��Ȱ��ȭ

				//Ÿ����ġŰ�� ��ȿ���� ������ �ȱ� �ӵ���
				if (!OwnerComp.GetBlackboardComponent()->IsVectorValueSet(AShooterAIController::TargetPosKey))
				{
					AICharacter->Walk();
					AICharacter->GetAIAnim()->SetIsAiming(false);//���� Off
				}
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(AShooterAIController::TargetKey);//Ÿ���� Ŭ����
		AIController->StopTrack();//Ÿ������ ��Ȱ��ȭ

		//Ÿ����ġŰ�� ��ȿ���� ������ �ȱ� �ӵ���
		if (!OwnerComp.GetBlackboardComponent()->IsVectorValueSet(AShooterAIController::TargetPosKey))
		{
			AICharacter->Walk();
			AICharacter->GetAIAnim()->SetIsAiming(false);//���� Off
		}
		
	}
}