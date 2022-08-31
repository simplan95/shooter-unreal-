// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_BossPattern.h"
#include "ShooterBossCharacter.h"//����ĳ��
#include "ShooterBossAIController.h"//���� ��Ʈ�ѷ�
#include "Shooter2Character.h"//Ÿ��ĳ���� ��Ŭ���
#include "BehaviorTree/BlackboardComponent.h"//������ ������Ʈ ��Ŭ���
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"//������ Ű ��Ÿ�� ��Ŭ���

UBTService_BossPattern::UBTService_BossPattern()
{
	NodeName = TEXT("BossPattern");//����������
	Interval = 5.0f;//���͹��ð�
}

//���� ���� ����
void UBTService_BossPattern::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	BossAIController = Cast<AShooterBossAIController>(OwnerComp.GetAIOwner());//Owner AI ��Ʈ�ѷ� ��������
	BossAICharacter = Cast<AShooterBossCharacter>(BossAIController->GetPawn());//Owner AI ĳ���� ��������

	int32 i = FMath::RandRange(1, 10);
	if (i < 5)
	{
		BossAIController->BossPattern = EBossState::BS_MISSILE;
		UE_LOG(Shooter2, Error, TEXT("Missile!!"));
	}
	else if (5 <= i && i < 8)
	{
		BossAIController->BossPattern = EBossState::BS_FIRE;
		UE_LOG(Shooter2, Error, TEXT("Fire!!"));
	}
	else if (8 <= i && i <= 10)
	{
		BossAIController->BossPattern = EBossState::BS_GRENADE;
		UE_LOG(Shooter2, Error, TEXT("Grenade!!"));
	}
	
	//i�� ������� ���� Enum���� ����
	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(BossAIController->BossStateBlackBoardKey, (uint8)BossAIController->BossPattern);
}