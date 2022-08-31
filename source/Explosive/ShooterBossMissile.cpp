// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterBossMissile.h"
#include "Shooter2Character.h"//ĳ����
#include "DrawDebugHelpers.h"//����� �׸���
#include "ShooterEnemyCharacter.h"//���ʹ� ĳ����
#include "EngineUtils.h"//�ݺ��� ���
#include "ShooterActorComponent.h"//ĳ���� ����
#include "GameFramework/Pawn.h"

//�̻���
AShooterBossMissile::AShooterBossMissile()
{
	PrimaryActorTick.bCanEverTick = true;

	fRange = 300.0f;//���� ����
	fExplosionDamage = 30.0f;//���� ������

	MissileComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MissileComp"));//�̻��� ĸ�� ������Ʈ �ʱ�ȭ
	MissileComp->InitCapsuleSize(8.0f,40.0f);//ĸ��ũ�� ����
	MissileComp->SetCollisionProfileName(TEXT("AIMissile"));//�ݸ��� ����
	RootComponent = MissileComp;//ĸ���� ��Ʈ ������Ʈ�� ����
	
	MissileBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MISSILEBODY"));//�̻��� �ٵ�
	MissileEject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MISSILEEJECT"));//�̻��� ����

	FString AssetPath = TEXT("/Game/MyAsset/MissleLauncher/Mesh/StaticM_Missile.StaticM_Missile");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MISSILEBODY(*AssetPath);//�̻��Ϲٵ� �ҷ�����
	if (SM_MISSILEBODY.Succeeded())
	{
		MissileBody->SetStaticMesh(SM_MISSILEBODY.Object);
		MissileBody->SetCollisionProfileName(TEXT("NoCollision"));//�޽� �ݸ��� ��Ȱ��
		MissileBody->SetupAttachment(RootComponent);//��Ʈ������Ʈ�� �̻��� Body����
		MissileBody->SetRelativeRotation(FRotator(90.0f,0.0f,0.0f));//�޽�ȸ�� ����
	}

	FString AssetPath2 = TEXT("/Game/MyAsset/MissleLauncher/Mesh/StaticM_MissileExhaust.StaticM_MissileExhaust");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MISSILEEJECT(*AssetPath2);//�̻��� ����Ʈ �ҷ�����
	if (SM_MISSILEEJECT.Succeeded())
	{
		MissileEject->SetStaticMesh(SM_MISSILEEJECT.Object);
		MissileEject->SetCollisionProfileName(TEXT("NoCollision"));//�޽� �ݸ��� ��Ȱ��
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

// Called when the game starts or when spawned
void AShooterBossMissile::BeginPlay()
{
	Super::BeginPlay();
	
	//���ͷ����͸� �̿��� �÷��̾� ���� ã�Ƴ��� ���� ������ �ִ� �÷��̾� ����
	for (FConstPawnIterator CharaIter = GetWorld()->GetPawnIterator(); CharaIter; ++CharaIter)
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