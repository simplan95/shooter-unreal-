// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterAIController.h"
#include "BehaviorTree/BehaviorTree.h"//�����̺�� Ʈ�� ��Ŭ���
#include "BehaviorTree/BlackboardData.h"//������ �ιɷ��
#include "BehaviorTree/BlackboardComponent.h"//������ ������Ʈ ��Ŭ���

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"//������ Ű ��Ÿ�� ��Ŭ���
#include "Shooter2Character.h"//�÷��̾� ĳ����
#include "ShooterAICharacter.h"//AIĳ����
#include "ShooterAIAnimInstance.h"//AI�ִ�

//AI �̴Ͼ� ��Ʈ�ѷ�
AShooterAIController::AShooterAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));//������ ������Ʈ �ʱ�ȭ
	
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

	AICharacter = Cast<AShooterAICharacter>(InPawn);//AI ĳ���͸� ���� ����(AShooterAICharacter �Լ��� �������� ������)
	AIAnim = Cast<UShooterAIAnimInstance>(AICharacter->GetMesh()->GetAnimInstance());//�ִ� �ν��Ͻ�

	BlackboardComp->InitializeBlackboard(*BTAsset->BlackboardAsset);//������ �����͸� �ʱ�ȭ
}


void AShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//AI�� ���� Ȱ��ȭ�� Ÿ��ĳ���� �������� ȸ��
	if (bIsTracking && TargetCharacter != nullptr)
	{
		FVector CharLoc = AICharacter->GetActorLocation();
		FVector DirToTarget = TargetCharacter->GetActorLocation() - CharLoc;
		DirToTarget.Z = 0.0f;//��ġ ȸ���� ��ȿ�� �ϱ����� Z�� 0����
		FRotator RotToTarget = FRotationMatrix::MakeFromX(DirToTarget).Rotator();
		//AI ĳ���Ͱ� ����� Ȯ���� �� ����� ���� ȸ�� ��ġ���� ��ȿ
		AICharacter->SetActorRotation(FMath::RInterpTo(AICharacter->GetActorRotation(), RotToTarget, GetWorld()->GetDeltaSeconds(), 15.0f));

		//AIĳ���Ͱ� Ÿ���� ���� AimPitch�� ����
		FVector Pitch = TargetCharacter->GetActorLocation() - CharLoc;//�÷��̾�ĳ���͸� �ٶ󺸴� ����
		FRotator PitchRotation = FRotationMatrix::MakeFromX(Pitch).Rotator();
		AIAnim->AimPitch = FMath::ClampAngle(AIAnim->FinRotation.Pitch, -90.0f, 90.0f);//Pitch�� ����
		AIAnim->CurRotation = FRotator(AIAnim->AimPitch, 0.0f, 0.0f);//���� �����̼ǰ�
		AIAnim->FinRotation = FMath::RInterpTo(AIAnim->CurRotation, PitchRotation, DeltaTime, 20.0f);//���� �����̼��� Ÿ�� �����̼����� 
	}
	else
	{
		AIAnim->AimPitch = AIAnim->FinRotation.Pitch;
		AIAnim->CurRotation = FRotator(AIAnim->AimPitch, 0.0f, 0.0f);//���� �����̼ǰ�
		AIAnim->FinRotation = FMath::RInterpTo(AIAnim->CurRotation, FRotator::ZeroRotator, DeltaTime, 5.0f);//���� �����̼��� Ÿ�� �����̼����� 
	}

}

