// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGrenade.h"
#include "GameFramework/ProjectileMovementComponent.h"//발사체 움직임 컴포넌트
#include "Components/SphereComponent.h"
#include "Shooter2Character.h"//캐릭터
#include "ShooterGripGrenade.h"//아이템 수류탄
#include "DrawDebugHelpers.h"//디버그 그리기
#include "ShooterEnemyCharacter.h"//에너미 캐릭터

//수류탄 
AShooterGrenade::AShooterGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;//틱 비활성화

	fRange = 350.0f;//폭발 범위
	fExplosionDamage = 100.0f;//폭발 데미지

	//메시, 파티클 설정
	ExplosionMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));//수류탄 메시
	ExplosionMesh->SetCollisionProfileName(TEXT("GunMesh"));//콜리전 설정
	RootComponent = ExplosionMesh;//루트컴포넌트 지정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_Grenade(TEXT("/Game/GrenadeKit/Meshes/MK2_Base_SK.MK2_Base_SK"));
	if (SK_Grenade.Succeeded())
	{
		ExplosionMesh->SetSkeletalMesh(SK_Grenade.Object);
		ExplosionMesh->SetRelativeScale3D(FVector(0.7f));//수류탄 크기 지정
		ExplosionMesh->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5));
		ExplosionMesh->SetSimulatePhysics(true);// 물리적용 On
	}
	
	//
	ExplosionParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("PaticleComp"));//폭파 파티클
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_Explosion(TEXT("/Game/MyAsset/Fire_Exp_00.Fire_Exp_00"));
	if (PT_Explosion.Succeeded())
	{
		ExplosionParticle=PT_Explosion.Object;
	}

	ExplosionSound = CreateDefaultSubobject<USoundBase>(TEXT("Fire"));//폭파 사운드
	static ConstructorHelpers::FObjectFinder<USoundBase>SB_Explosion(TEXT("/Game/MyAsset/SC_Grenade_Explosion.SC_Grenade_Explosion"));
	if (SB_Explosion.Succeeded())
	{
		ExplosionSound = SB_Explosion.Object;
	}

	//ProjectileMovement 설정
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = ExplosionCollisionComp;
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;//참이면 이 발사체는 속도 방향과 일치하도록 프레임마다 회전이 업데이트됩니다.
	ProjectileMovement->bShouldBounce = true;//바운스 활성화
}

// Called when the game starts or when spawned
void AShooterGrenade::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(GrenadeTimer, this, &AShooterGrenade::Explosion, 5.0f, false);//수류탄 타이머는 5초로 설정 5초후 수류탄 폭발
}

// Called every frame
void AShooterGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}