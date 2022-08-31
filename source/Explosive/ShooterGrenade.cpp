// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGrenade.h"
#include "GameFramework/ProjectileMovementComponent.h"//�߻�ü ������ ������Ʈ
#include "Components/SphereComponent.h"
#include "Shooter2Character.h"//ĳ����
#include "ShooterGripGrenade.h"//������ ����ź
#include "DrawDebugHelpers.h"//����� �׸���
#include "ShooterEnemyCharacter.h"//���ʹ� ĳ����

//����ź 
AShooterGrenade::AShooterGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;//ƽ ��Ȱ��ȭ

	fRange = 350.0f;//���� ����
	fExplosionDamage = 100.0f;//���� ������

	//�޽�, ��ƼŬ ����
	ExplosionMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));//����ź �޽�
	ExplosionMesh->SetCollisionProfileName(TEXT("GunMesh"));//�ݸ��� ����
	RootComponent = ExplosionMesh;//��Ʈ������Ʈ ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_Grenade(TEXT("/Game/GrenadeKit/Meshes/MK2_Base_SK.MK2_Base_SK"));
	if (SK_Grenade.Succeeded())
	{
		ExplosionMesh->SetSkeletalMesh(SK_Grenade.Object);
		ExplosionMesh->SetRelativeScale3D(FVector(0.7f));//����ź ũ�� ����
		ExplosionMesh->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5));
		ExplosionMesh->SetSimulatePhysics(true);// �������� On
	}
	
	//
	ExplosionParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("PaticleComp"));//���� ��ƼŬ
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_Explosion(TEXT("/Game/MyAsset/Fire_Exp_00.Fire_Exp_00"));
	if (PT_Explosion.Succeeded())
	{
		ExplosionParticle=PT_Explosion.Object;
	}

	ExplosionSound = CreateDefaultSubobject<USoundBase>(TEXT("Fire"));//���� ����
	static ConstructorHelpers::FObjectFinder<USoundBase>SB_Explosion(TEXT("/Game/MyAsset/SC_Grenade_Explosion.SC_Grenade_Explosion"));
	if (SB_Explosion.Succeeded())
	{
		ExplosionSound = SB_Explosion.Object;
	}

	//ProjectileMovement ����
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = ExplosionCollisionComp;
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;//���̸� �� �߻�ü�� �ӵ� ����� ��ġ�ϵ��� �����Ӹ��� ȸ���� ������Ʈ�˴ϴ�.
	ProjectileMovement->bShouldBounce = true;//�ٿ Ȱ��ȭ
}

// Called when the game starts or when spawned
void AShooterGrenade::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(GrenadeTimer, this, &AShooterGrenade::Explosion, 5.0f, false);//����ź Ÿ�̸Ӵ� 5�ʷ� ���� 5���� ����ź ����
}

// Called every frame
void AShooterGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}