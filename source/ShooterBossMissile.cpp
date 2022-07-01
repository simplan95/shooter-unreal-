// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterBossMissile.h"
#include "ShooterAICharacter.h"//AI ĳ����
#include "ShooterBossCharacter.h"//Ai ����ĳ����
#include "Shooter2Character.h"//ĳ����
#include "DrawDebugHelpers.h"//����� �׸���
#include "EngineUtils.h"//�ݺ��� ���
#include "ShooterActorComponent.h"//ĳ���� ����
#include "GameFramework/Pawn.h"
#include "ShooterEnemyCharacter.h"//���ʹ� ĳ����


//������ �߻��� �̻���
AShooterBossMissile::AShooterBossMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MissileComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MissileComp"));//�̻��� ĸ�� ������Ʈ �ʱ�ȭ
	MissileComp->InitCapsuleSize(8.0f,40.0f);//ĸ��ũ�� ����
	MissileComp->SetCollisionProfileName(TEXT("AIMissile"));//�ݸ��� ����
	RootComponent = MissileComp;//ĸ���� ��Ʈ ���ʳ�Ʈ�� ����
	
	MissileBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MISSILEBODY"));//�̻��� �ٵ�
	MissileEject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MISSILEEJECT"));//�̻��� ����

	FString AssetPath = TEXT("/Game/MyAsset/MissleLauncher/Mesh/StaticM_Missile.StaticM_Missile");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MISSILEBODY(*AssetPath);//�̻��Ϲٵ� �ҷ�����
	if (SM_MISSILEBODY.Succeeded())
	{
		MissileBody->SetStaticMesh(SM_MISSILEBODY.Object);
		MissileBody->SetCollisionProfileName(TEXT("NoCollision"));//�޽ô� �ݸ��� ��Ȱ��
		MissileBody->SetupAttachment(RootComponent);//��Ʈ������Ʈ�� �̻��� Body����
		MissileBody->SetRelativeRotation(FRotator(90.0f,0.0f,0.0f));//�޽�ȸ�� ����
	}

	FString AssetPath2 = TEXT("/Game/MyAsset/MissleLauncher/Mesh/StaticM_MissileExhaust.StaticM_MissileExhaust");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MISSILEEJECT(*AssetPath2);//�̻��� ����Ʈ �ҷ�����
	if (SM_MISSILEEJECT.Succeeded())
	{
		MissileEject->SetStaticMesh(SM_MISSILEEJECT.Object);
		MissileEject->SetCollisionProfileName(TEXT("NoCollision"));//�޽ô� �ݸ��� ��Ȱ��
		MissileEject->SetupAttachment(MissileBody);//�̻��� Body�� Eject�޽ø� ����
	}

	FName MissileHeadSocket(TEXT("ExhaustSoket"));
	if (MissileBody->DoesSocketExist(MissileHeadSocket))//MissileBody �޽ÿ� ExhaustSoket�̸��� ������ ���� ��
	{
		MissileEject->SetupAttachment(MissileBody, MissileHeadSocket);//�̻��� Body�� Eject�� ����
	}

	ExplosionParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("ExplosionPaticle"));//���� ��ƼŬ
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_Explosion(TEXT("/Game/MyAsset/PS_MissileExplosion.PS_MissileExplosion"));
	if (PT_Explosion.Succeeded())
	{
		ExplosionParticle = PT_Explosion.Object;
	}

	ExplosionSound = CreateDefaultSubobject<USoundBase>(TEXT("Explosion"));//���� ����
	static ConstructorHelpers::FObjectFinder<USoundBase>SB_Explosion(TEXT("/Game/MyAsset/SC_MissileExplosion.SC_MissileExplosion"));
	if (SB_Explosion.Succeeded())
	{
		ExplosionSound = SB_Explosion.Object;
	}

	//��������Ʈ ���ε�
	OnActorBeginOverlap.AddDynamic(this, &AShooterBossMissile::MyOnBeginOverlap);//���Ͱ� Overlap��

	CharacterStat = CreateDefaultSubobject<UShooterActorComponent>(TEXT("CharacterStat"));//ĳ���� ���� Ŭ����

}

void AShooterBossMissile::MyOnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Explosion();//���� ��ħ �߻��� ����
}

float AShooterBossMissile::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	Destroy();//�������� ���� ��� Destroy
	
	return FinalDamage;
}

void AShooterBossMissile::Explosion()
{
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());//���߻��� ���
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionParticle, GetActorLocation());//���� ��ƼŬ���

	float Range = 300.0f;//���ݹ���
	FVector Center = GetActorLocation();//��������
	TArray<FOverlapResult> OverLapResults;//TArray �ڷᱸ���� ������ ��� �ν��Ͻ��� �����Ѵ�.
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);//�ݸ��� ����

	FCollisionObjectQueryParams ObjQuery;//���� �޴� �ν��Ͻ� ����(�迭)
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel1);// ���ݹ޴� Ʈ���̽� ä�� ���� �߰� (ĳ����)
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel2);// ���ݹ޴� Ʈ���̽� ä�� ���� �߰� (AI)
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel3);// ���ݹ޴� Ʈ���̽� ä�� ���� �߰� (GunMesh)
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel6);// ���ݹ޴� Ʈ���̽� ä�� ���� �߰� (AIMissile)
	ObjQuery.AddObjectTypesToQuery(ECC_PhysicsBody);//�������ٵ� �߰�

	bool bResult = GetWorld()->OverlapMultiByObjectType(//������Ʈ Ÿ���� ��Ƽ ������� Ž��(������ ������ �浹�� ��ü�鿡 ä���� �����Ͽ� Ž��)
		OverLapResults,
		Center,
		FQuat::Identity,
		ObjQuery,//Ž�� ������ ����� Ʈ���̽� ä�� ����
		FCollisionShape::MakeSphere(Range),//Ž���� ����� �⺻ ���� ����, ��ü, ĸ��, �ڽ��� ���
		CollisionQueryParam//�ݸ��� ����
	);

	if (bResult)//��ȿ���� ��ȯ�� ��
	{
		for (auto OverLapResult : OverLapResults)//for�� ó���� ������ ���͸� ��ȯ
		{

			FCollisionObjectQueryParams ObjQueryLine;//���� �޴� �ν��Ͻ� ����(�迭)
			ObjQueryLine.AddObjectTypesToQuery(ECC_GameTraceChannel4);// ���ݹ޴� Ʈ���̽� ä�� ���� �߰� (���)

			FCollisionQueryParams ColQuery;//���� ��ü �ν��Ͻ�
			ColQuery.AddIgnoredActor(this);//���� ��ü ���ʹ� ���� ����

			FHitResult HitRes;//��Ʈ Ȯ�� �ν��Ͻ�

			//���� Ʈ���̽��� ��ƮȮ�ΰ˻�
			bool HitResult = GetWorld()->LineTraceSingleByObjectType(HitRes, Center, OverLapResult.GetActor()->GetActorLocation(), ObjQueryLine, ColQuery);

			FName CollisionName(TEXT("Ragdoll"));//���׵� ����
			FName CollisionName2(TEXT("GunMesh"));//�Ǹ޽� ����
			FName CollisionName3(TEXT("AIChara"));//AI ĳ���� ����
			FName CollisionName4(TEXT("ShooterChara"));//���� ĳ�� ����
			FName CollisionName5(TEXT("AIMissile"));//AI �̻��� ����

			if (!HitResult)//HitRes.bBlockingHit)//����Ʈ���̽��� �������� �� �� ���̿� ��� ��Ұ� ���� �� 
			{
				if (CollisionName == OverLapResult.GetComponent()->GetCollisionProfileName())//���׵� �ݸ����� ���
				{
					knockBack(OverLapResult, Center, 50000.0f);
				}
				else if (CollisionName2 == OverLapResult.GetComponent()->GetCollisionProfileName())//�Ǹ޽� �����ݸ����� ���
				{
					knockBack(OverLapResult, Center, 2000.0f);
				}

				if (OverLapResult.Actor.IsValid())
				{
					if (CollisionName3 == OverLapResult.GetComponent()->GetCollisionProfileName())//AI �ݸ����� ���
					{
						auto HitAI = Cast<AShooterEnemyCharacter>(OverLapResult.GetActor());
						HitAI->SetDamagePoint(GetActorLocation());//�̻��� ��ġ�� ��� ���� ������ ����Ʈ�� ����
					}
					else if (CollisionName4 == OverLapResult.GetComponent()->GetCollisionProfileName())//�÷��̾� ĳ������ ���
					{
						auto HitChara = Cast<AShooter2Character>(OverLapResult.GetActor());
						HitChara->SetDamagePoint(GetActorLocation());//�̻��� ��ġ�� ��� ���� ������ ����Ʈ�� ����
					}

					FDamageEvent  DamageEvent;

					//������ ����. ������ ����,����� ����,���ݸ�� ������,��������� ����
					OverLapResult.Actor->TakeDamage(CharacterStat->GetAIBossMissileDamage(), DamageEvent, nullptr, this);

				}
			}

		}
	}
	else
	{
		//Donothing
	}

	Destroy();//���� ���� ���� Destroy
}

void AShooterBossMissile::MissileTracking()//�̻����� ���� ������ �ִ� �÷��̾ ����
{
	FVector Between = TargetPlyaerCharacter->GetActorLocation() - GetActorLocation();//����� �÷��̾� ĳ���Ϳ� �̻��� ������ ���⺤�͸� ����

	FVector MissilePos = MoveToward(GetActorLocation(), TargetPlyaerCharacter->GetActorLocation(), 7.0f);//�̻����� �̵��� ���͸� ����
	SetActorLocation(MissilePos);//�̻��� �̵�

	FRotator RotToTarget = FRotationMatrix::MakeFromZ(Between).Rotator();
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), RotToTarget, GetWorld()->GetDeltaSeconds(), 15.0f));

	if (MissilePos == TargetPlyaerCharacter->GetActorLocation()) Destroy();


}

FVector AShooterBossMissile::MoveToward(FVector Start, FVector End, float Alpah)
{
	FVector a = End - Start;
	float magnitude = a.Size();//������ magnitude����
	if (magnitude <= Alpah || magnitude == 0.0f)
	{
		return End;
	}
	return Start + a / magnitude * Alpah;//������������ ���ֺ���*Alpha���� ���Ѱ��� ��ȯ
}

void AShooterBossMissile::knockBack(FOverlapResult HitResult, FVector Origin, float Impulse)
{
	// �÷��̾�� Ÿ���� ������ ����
	FVector ImpulseDirection = HitResult.GetActor()->GetActorLocation() - Origin;//�о��� ���� ����(���� -����)
	ImpulseDirection.Normalize();//���� ����ȭ

	//�浹 ����
	float ImpulseStrength = Impulse;

	FVector FinalImpulse = ImpulseDirection * ImpulseStrength;//���޽� ����

	HitResult.GetComponent()->SetPhysicsLinearVelocity(FVector::ZeroVector);
	HitResult.GetComponent()->AddImpulseAtLocation(FinalImpulse, HitResult.GetActor()->GetActorLocation());//�ش���ġ���� ���޽� ��ŭ �о
}

// Called when the game starts or when spawned
void AShooterBossMissile::BeginPlay()
{
	Super::BeginPlay();
	
	for (FConstPawnIterator CharaIter = GetWorld()->GetPawnIterator(); CharaIter; ++CharaIter)//���ͷ����͸� �̿��� �÷��̾� ���� ã�Ƴ��� ���� ������ �ִ� �÷��̾� ����
	{
		if (CharaIter->Get()->IsPlayerControlled())
		{
			APawn* TempTarget = CharaIter->Get();

			if (fMinimalLen > TempTarget->GetDistanceTo(this))
			{
				fMinimalLen = TempTarget->GetDistanceTo(this);
				TargetPlyaerCharacter = TempTarget;
			}
		}
	}
	
}

// Called every frame
void AShooterBossMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MissileTracking();//ƽ���� �̻��� Ʈ��ŷ Ȱ��ȭ
}
