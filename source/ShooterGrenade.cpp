// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGrenade.h"
#include "GameFramework/ProjectileMovementComponent.h"//�߻�ü ������ ������Ʈ
#include "Components/SphereComponent.h"
#include "ShooterAICharacter.h"//AI ĳ����
#include "ShooterBossCharacter.h"//Ai ����ĳ����
#include "Shooter2Character.h"//ĳ����
#include "ShooterGripGrenade.h"//������ ����ź
#include "DrawDebugHelpers.h"//����� �׸���
#include "ShooterEnemyCharacter.h"//���ʹ� ĳ����

//����ź ����
AShooterGrenade::AShooterGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;//ƽ ��Ȱ��ȭ

	ProjMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));//����ź �޽�
	ProjMesh->SetCollisionProfileName(TEXT("GunMesh"));//�ݸ��� ����
	RootComponent = ProjMesh;//��Ʈ������Ʈ ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_Grenade(TEXT("/Game/GrenadeKit/Meshes/MK2_Base_SK.MK2_Base_SK"));
	if (SK_Grenade.Succeeded())
	{
		ProjMesh->SetSkeletalMesh(SK_Grenade.Object);
		ProjMesh->SetRelativeScale3D(FVector(0.7f));//����ź ũ�� ����
		ProjMesh->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5));
		ProjMesh->SetSimulatePhysics(true);//�Ѹ޽� �������� On
	}
	
	//
	ExplosionParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("PaticleComp"));//���� ��ƼŬ
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_Explosion(TEXT("/Game/MyAsset/Fire_Exp_00.Fire_Exp_00"));
	if (PT_Explosion.Succeeded())
	{
		ExplosionParticle=PT_Explosion.Object;
	}

	PinPullSound = CreateDefaultSubobject<USoundBase>(TEXT("Reload"));//�ɻ̴� ����
	static ConstructorHelpers::FObjectFinder<USoundBase>SB_PinPull(TEXT("/Game/MyAsset/SC_Grenade_PinPull.SC_Grenade_PinPull"));
	if (SB_PinPull.Succeeded())
	{
		PinPullSound = SB_PinPull.Object;
	}

	ExplosionSound = CreateDefaultSubobject<USoundBase>(TEXT("Fire"));//���� ����
	static ConstructorHelpers::FObjectFinder<USoundBase>SB_Explosion(TEXT("/Game/MyAsset/SC_Grenade_Explosion.SC_Grenade_Explosion"));
	if (SB_Explosion.Succeeded())
	{
		ExplosionSound = SB_Explosion.Object;
	}

	//�߻�ü ������ ����
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));//ProjectileMovement �ν��Ͻ� �ʱ�ȭ
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 1500.f;
	ProjectileMovement->MaxSpeed = 1500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;//���̸� �� �߻�ü�� �ӵ� ����� ��ġ�ϵ��� �����Ӹ��� ȸ���� ������Ʈ�˴ϴ�.
	ProjectileMovement->bShouldBounce = true;//�ٿ Ȱ��ȭ
}

void AShooterGrenade::Explosion()
{
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());//���Ļ��� Ȱ��ȭ
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionParticle, GetActorLocation());//��������Ʈ Ȱ��ȭ
	
	
	float Range = 370.0f;//���ݹ���
	FVector Center = GetActorLocation();//��������
	TArray<FOverlapResult> OverLapResults;//TArray �ڷᱸ���� ������ ��� �ν��Ͻ��� �����Ѵ�.
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);//

	FCollisionObjectQueryParams ObjQuery;//���� �޴� �ν��Ͻ� ����(�迭)
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel1);// ���ݹ޴� Ʈ���̽� ä�� ���� �߰� (ĳ����)
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel2);// ���ݹ޴� Ʈ���̽� ä�� ���� �߰� (AI)
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel3);// ���ݹ޴� Ʈ���̽� ä�� ���� �߰� (GunMesh)
	ObjQuery.AddObjectTypesToQuery(ECC_PhysicsBody);//�������ٵ� �߰�

	bool bResult = GetWorld()->OverlapMultiByObjectType(//������Ʈ Ÿ���� ��Ƽ ������� Ž��(������ ������ �浹�� ��ü���� ä���� �����Ͽ� Ž��)
		OverLapResults,
		Center,
		FQuat::Identity,
		ObjQuery,//Ž�� ������ ����� Ʈ���̽� ä�� ����
		FCollisionShape::MakeSphere(Range),//Ž���� ����� �⺻ ���� ����, ��ü, ĸ��, �ڽ��� ���
		CollisionQueryParam//
	);

	if (bResult)//��ȿ���� ��ȯ�� ��
	{
		FName CollisionName(TEXT("Ragdoll"));//���׵� ����
		FName CollisionName2(TEXT("GunMesh"));//�Ǹ޽� ����
		FName CollisionName3(TEXT("AIChara"));//AI ĳ���� ����
		FName CollisionName4(TEXT("ShooterChara"));//����ĳ�� ����
		FName CollisionName5(TEXT("AIMissile"));//AI �̻��� ����

		//for�� ó���� ������ ���͸� ��ȯ
		for (auto OverLapResult : OverLapResults)
		{

			FCollisionObjectQueryParams ObjQueryLine;//���� �޴� �ν��Ͻ� ����(�迭)
			ObjQueryLine.AddObjectTypesToQuery(ECC_GameTraceChannel4);// ���ݹ޴� Ʈ���̽� ä�� ���� �߰� (���)

			FCollisionQueryParams ColQuery;//���� ��ü �ν��Ͻ�
			ColQuery.AddIgnoredActor(this);//���� ��ü ���ʹ� ���� ����

			FHitResult HitRes;//��Ʈ Ȯ�� �ν��Ͻ�

			//���� Ʈ���̽��� ��ƮȮ�ΰ˻�
 			bool HitResult = GetWorld()->LineTraceSingleByObjectType(HitRes, Center, OverLapResult.GetActor()->GetActorLocation(), ObjQueryLine, ColQuery);
			
			if (!HitResult)//(!HitResult)//(HitRes.bBlockingHit)//����Ʈ���̽��� ���� ��� �׻��̿� ��� ��Ұ� ���� �� 
			{
				if (CollisionName == OverLapResult.GetComponent()->GetCollisionProfileName())//���׵� ���Ӱ� �浹 ����� ������
				{
					knockBack(OverLapResult, Center, 50000.0f);//���� �˹�ó��
				}
				else if (CollisionName2 == OverLapResult.GetComponent()->GetCollisionProfileName())//�Ǹ޽� ���Ӱ� �浹 ����� ������
				{
					knockBack(OverLapResult, Center, 1500.0f);//���� �˹�ó��
				}

				if (OverLapResult.Actor.IsValid())
				{
					if (CollisionName3 == OverLapResult.GetComponent()->GetCollisionProfileName())//�ݸ����� AI�϶�
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
					OverLapResult.Actor->TakeDamage(100.0f, DamageEvent, nullptr, this);
					
				}
			}
		
		}
	}
	else
	{
		//Donothing
	}
	Destroy();//���� Destroy
}

void AShooterGrenade::knockBack(FOverlapResult HitResult, FVector Origin, float Impulse)
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