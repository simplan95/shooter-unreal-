// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "ShooterAIEnemyController.h"//AI origin ��Ʈ�ѷ� 
#include "ShooterAIController.h"//AI �̴Ͼ� ��Ʈ�ѷ� 
#include "ShooterAICharacter.h"//AI ĳ���� 
#include "Shooter2Character.h"//�÷��̾� ĳ���� 
#include "BehaviorTree/BlackboardComponent.h"//������ ������Ʈ 
#include "DrawDebugHelpers.h"//����� ����� ���� �ð�ȭ
#include "ShooterAIAnimInstance.h"//AI�ִ� �ν��Ͻ�

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");//����������
	Interval = 0.5f;//���͹��ð�
}

//AI �̴Ͼ� Ž��
void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AIController = Cast<AShooterAIController>(OwnerComp.GetAIOwner());//Owner AI ��Ʈ�ѷ� ��������
	AICharacter=Cast<AShooterAICharacter>(AIController->GetPawn());//Owner AI ĳ���� ��������

	FVector Box = FVector(25.0f,500.0f,500.0f);//Ž���ڽ�����
	float EndRange = 2000.0f;

	FVector Center = AICharacter->GetDtectPoint();

	TArray<FOverlapResult> OverLapResults;//TArray �ڷᱸ���� ������ ��� �ν��Ͻ��� ����
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, AICharacter);//�ݸ��� ����

	TArray<FHitResult> HitResults;

	bool bResult = AICharacter->GetWorld()->SweepMultiByObjectType(
		HitResults,
		Center,//Ž���� ������ ��ġ
		Center + AICharacter->GetActorForwardVector() * EndRange,//Ž���� ���� ��ġ
		FQuat::Identity,//Ž���� ����� ������ ȸ��
		ECollisionChannel::ECC_GameTraceChannel1,//�����浹 ������ ����� Ʈ���̽� ä�� ����
		FCollisionShape::MakeBox(Box),//Ž���� ����� �⺻ ���� ����, ��ü, ĸ��, �ڽ��� ���
		CollisionQueryParam
	);
	
	FVector TraceVec = AICharacter->GetActorForwardVector() * EndRange;//����׹ڽ� ����
	FVector DebugBoxCenter = AICharacter->GetDtectPoint() + TraceVec * 0.5f;//����׹ڽ� ����
	FQuat BoxRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();//����׹ڽ� ����
	
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

			bool HitLineResult = GetWorld()->LineTraceSingleByObjectType(HitLineRes, Center, TargetCharacter->GetTargetPoint(), ObjQueryLine, ColQuery);//���� Ʈ���̽��� ��ƮȮ�ΰ˻�

			if (!HitLineResult)//HitRes.bBlockingHit)//����Ʈ���̽��� ���� �� �׻��̿� ��� ��Ұ� ���� �� 
			{ 
				
				if (TargetCharacter && TargetCharacter->GetController()->IsPlayerController())//�÷��̾� ��Ʈ�ѷ� ������ �Ǵ�
				{
					AIController->TrackToTargetOutRange(HitResult.Actor->GetActorLocation());
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(AShooterAIEnemyController::TargetKey, TargetCharacter);//Ž���� ĳ������ ��ġ���� Ÿ��Ű ���� ����
					
					AIController->TrackToTarget();//Ÿ�� ���� Ȱ��ȭ
					if (AICharacter->GetDistanceTo(TargetCharacter) >= 1000.0f)//�����Ÿ� �̻� �־����� �޷���
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
				OwnerComp.GetBlackboardComponent()->ClearValue(AShooterAIEnemyController::TargetKey);//Ÿ���� Ŭ���� 
				AIController->StopTrack();//Ÿ������ ��Ȱ��ȭ
				if (!OwnerComp.GetBlackboardComponent()->IsVectorValueSet(AShooterAIEnemyController::TargetPosKey))//Ÿ����ġŰ�� ��ȿ���� ������ �ȱ� �ӵ���
				{
					AICharacter->Walk();
					AICharacter->GetAIAnim()->SetIsAiming(false);//���� Off
				}
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(AShooterAIEnemyController::TargetKey);//Ÿ���� Ŭ���� 
		AIController->StopTrack();//Ÿ������ ��Ȱ��ȭ
		if (!OwnerComp.GetBlackboardComponent()->IsVectorValueSet(AShooterAIEnemyController::TargetPosKey))//Ÿ����ġŰ�� ��ȿ���� ������ �ȱ� �ӵ���
		{
			AICharacter->Walk();
			AICharacter->GetAIAnim()->SetIsAiming(false);//���� Off
		}
		
	}
}