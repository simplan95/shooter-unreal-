// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterExplosive.h"
#include "Shooter2Character.h"//ĳ����
#include "DrawDebugHelpers.h"//����� �׸���
#include "ShooterEnemyCharacter.h"//���ʹ� ĳ����

//���� ������Ʈ Origin Ŭ����
AShooterExplosive::AShooterExplosive()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	fRange = 350.0f;//���� ����
	fExplosionDamage = 100.0f;//���� ������

}

// Called when the game starts or when spawned
void AShooterExplosive::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShooterExplosive::Explosion()
{
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());//���Ļ��� Ȱ��ȭ
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionParticle, GetActorLocation());//��������Ʈ Ȱ��ȭ

	FVector Center = GetActorLocation();//��������
	TArray<FOverlapResult> OverLapResults;//TArray �ڷᱸ���� ������ ��� �ν��Ͻ��� �����Ѵ�.
	FCollisionObjectQueryParams ObjQuery;//���� �޴� �ν��Ͻ� ����(�迭)
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);//���� ��ü ���ʹ� ���� ����

	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel1);// ���ݹ޴� Ʈ���̽� ä�� ���� �߰� (ĳ����)
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel2);// ���ݹ޴� Ʈ���̽� ä�� ���� �߰� (AI)
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel3);// ���ݹ޴� Ʈ���̽� ä�� ���� �߰� (GunMesh)
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel6);// ���ݹ޴� Ʈ���̽� ä�� ���� �߰� (AIMissile)
	ObjQuery.AddObjectTypesToQuery(ECC_PhysicsBody);//�������ٵ� �߰�
	

	bool bResult = GetWorld()->OverlapMultiByObjectType(//������Ʈ Ÿ���� ��Ƽ ������� Ž��(������ ������ �浹�� ��ü���� ä���� �����Ͽ� Ž��)
		OverLapResults,
		Center,
		FQuat::Identity,
		ObjQuery,//Ž�� ������ ����� Ʈ���̽� ä�� ����
		FCollisionShape::MakeSphere(fRange),//Ž���� ����� �⺻ ���� ����, ��ü, ĸ��, �ڽ��� ���
		CollisionQueryParam//
	);

	if (bResult)//��ȿ���� ��ȯ�� ��
	{
		FName CollisionName(TEXT("Ragdoll"));//���׵� ����
		FName CollisionName2(TEXT("GunMesh"));//�Ǹ޽� ����
		FName CollisionName3(TEXT("AIChara"));//AI ĳ���� ����
		FName CollisionName4(TEXT("ShooterChara"));//����ĳ�� ����
		FName CollisionName5(TEXT("AIMissile"));//AI �̻��� ����

		for (auto OverLapResult : OverLapResults)//for�� ó���� ������ ���͸� ��ȯ
		{
			FCollisionObjectQueryParams ObjQueryLine;//���� �޴� �ν��Ͻ� ����(�迭)
			ObjQueryLine.AddObjectTypesToQuery(ECC_GameTraceChannel4);// ���ݹ޴� Ʈ���̽� ä�� ���� �߰� (���)

			FCollisionQueryParams ColQuery;//���� ��ü �ν��Ͻ�
			ColQuery.AddIgnoredActor(this);//���� ��ü ���ʹ� ���� ����

			FHitResult HitRes;//��Ʈ Ȯ�� �ν��Ͻ�

			bool IsBlocked = false;
			//���� Ʈ���̽��� ��ƮȮ�ΰ˻�
			IsBlocked = GetWorld()->LineTraceSingleByObjectType(HitRes, Center, OverLapResult.GetActor()->GetActorLocation(), ObjQueryLine, ColQuery);

			if (!IsBlocked)//(!HitResult)//(HitRes.bBlockingHit)//����Ʈ���̽��� ���� ��� �׻��̿� ��� ��Ұ� ���� �� 
			{
				if (OverLapResult.Actor.IsValid())
				{
					if (CollisionName == OverLapResult.GetComponent()->GetCollisionProfileName())//���׵� ���Ӱ� �浹 ����� ������
					{
						knockBack(OverLapResult, Center, 50000.0f);//���� �˹�ó��
					}
					else if (CollisionName2 == OverLapResult.GetComponent()->GetCollisionProfileName())//�Ǹ޽� ���Ӱ� �浹 ����� ������
					{
						knockBack(OverLapResult, Center, 1500.0f);//���� �˹�ó��
					}
					else if (CollisionName3 == OverLapResult.GetComponent()->GetCollisionProfileName())//�ݸ����� AI�϶�
					{
						auto HitAI = Cast<AShooterEnemyCharacter>(OverLapResult.GetActor());
						HitAI->SetDamagePoint(GetActorLocation());//����ź ��ġ�� ��� ���� ������ ����Ʈ�� ����

					}
					else if (CollisionName4 == OverLapResult.GetComponent()->GetCollisionProfileName())//�ݸ����� �÷��̾� ĳ�����϶�
					{
						auto HitChara = Cast<AShooter2Character>(OverLapResult.GetActor());
						HitChara->SetDamagePoint(GetActorLocation());//����ź ��ġ�� ��� ���� ������ ����Ʈ�� ����
					}

					FDamageEvent  DamageEvent;

					//��� ���� ������ó�� (������ ����,����� ����,���ݸ�� ������,��������� ���� )
					OverLapResult.Actor->TakeDamage(fExplosionDamage, DamageEvent, GetWorld()->GetFirstPlayerController(), this);
				}
			}
		}
	}
	else
	{
		//DoNothing
	}

	Destroy();//���� Destroy
}

void AShooterExplosive::knockBack(FOverlapResult HitResult, FVector Origin, float Impulse)
{
	// �÷��̾�� Ÿ���� ������ ����
	FVector ImpulseDirection = HitResult.GetActor()->GetActorLocation() - Origin;//�о��� ���� ����(���� -����)
	ImpulseDirection.Normalize();

	//�浹 ����
	float ImpulseStrength = Impulse;

	FVector FinalImpulse = ImpulseDirection * ImpulseStrength;//���޽� ���� ����

	HitResult.GetComponent()->SetPhysicsLinearVelocity(FVector::ZeroVector);
	HitResult.GetComponent()->AddImpulseAtLocation(FinalImpulse, HitResult.GetActor()->GetActorLocation());//�ش���ġ���� ���޽� ��ŭ �о
}

// Called every frame
void AShooterExplosive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
