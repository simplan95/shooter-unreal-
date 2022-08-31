// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterBossMissile.h"
#include "Shooter2Character.h"//캐릭터
#include "DrawDebugHelpers.h"//디버그 그리기
#include "ShooterEnemyCharacter.h"//에너미 캐릭터
#include "EngineUtils.h"//반복자 사용
#include "ShooterActorComponent.h"//캐릭터 스탯
#include "GameFramework/Pawn.h"

//미사일
AShooterBossMissile::AShooterBossMissile()
{
	PrimaryActorTick.bCanEverTick = true;

	fRange = 300.0f;//폭발 범위
	fExplosionDamage = 30.0f;//폭발 데미지

	MissileComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MissileComp"));//미사일 캡슐 컴포넌트 초기화
	MissileComp->InitCapsuleSize(8.0f,40.0f);//캡슐크기 설정
	MissileComp->SetCollisionProfileName(TEXT("AIMissile"));//콜리전 설정
	RootComponent = MissileComp;//캡슐을 루트 컴포넌트로 지정
	
	MissileBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MISSILEBODY"));//미사일 바디
	MissileEject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MISSILEEJECT"));//미사일 추진

	FString AssetPath = TEXT("/Game/MyAsset/MissleLauncher/Mesh/StaticM_Missile.StaticM_Missile");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MISSILEBODY(*AssetPath);//미사일바디 불러오기
	if (SM_MISSILEBODY.Succeeded())
	{
		MissileBody->SetStaticMesh(SM_MISSILEBODY.Object);
		MissileBody->SetCollisionProfileName(TEXT("NoCollision"));//메시 콜리전 비활성
		MissileBody->SetupAttachment(RootComponent);//루트컴포넌트에 미사일 Body부착
		MissileBody->SetRelativeRotation(FRotator(90.0f,0.0f,0.0f));//메시회전 설정
	}

	FString AssetPath2 = TEXT("/Game/MyAsset/MissleLauncher/Mesh/StaticM_MissileExhaust.StaticM_MissileExhaust");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MISSILEEJECT(*AssetPath2);//미사일 이젝트 불러오기
	if (SM_MISSILEEJECT.Succeeded())
	{
		MissileEject->SetStaticMesh(SM_MISSILEEJECT.Object);
		MissileEject->SetCollisionProfileName(TEXT("NoCollision"));//메시 콜리전 비활성
		MissileEject->SetupAttachment(MissileBody);//미사일 Body에 Eject메시를 부착
	}

	FName MissileHeadSocket(TEXT("ExhaustSoket"));
	if (MissileBody->DoesSocketExist(MissileHeadSocket))//MissileBody 메시에 ExhaustSoket이름의 소켓이 있을 때
	{
		MissileEject->SetupAttachment(MissileBody, MissileHeadSocket);//미사일 Body에 Eject를 부착
	}

	ExplosionParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("ExplosionPaticle"));//폭파 파티클
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_Explosion(TEXT("/Game/MyAsset/PS_MissileExplosion.PS_MissileExplosion"));
	if (PT_Explosion.Succeeded())
	{
		ExplosionParticle = PT_Explosion.Object;
	}

	ExplosionSound = CreateDefaultSubobject<USoundBase>(TEXT("Explosion"));//폭파 사운드
	static ConstructorHelpers::FObjectFinder<USoundBase>SB_Explosion(TEXT("/Game/MyAsset/SC_MissileExplosion.SC_MissileExplosion"));
	if (SB_Explosion.Succeeded())
	{
		ExplosionSound = SB_Explosion.Object;
	}

	//델리게이트 바인딩
	OnActorBeginOverlap.AddDynamic(this, &AShooterBossMissile::MyOnBeginOverlap);//액터가 Overlap시

}

void AShooterBossMissile::MyOnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Explosion();//액터 겹침 발생시 폭발
}

float AShooterBossMissile::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	Destroy();//데미지를 입을 경우 Destroy
	
	return FinalDamage;
}

void AShooterBossMissile::MissileTracking()//미사일이 가장 가까이 있는 플레이어를 따라감
{
	FVector Between = TargetPlyaerCharacter->GetActorLocation() - GetActorLocation();//월드상 플레이어 캐릭터와 미사일 사이의 방향벡터를 구함

	FVector MissilePos = MoveToward(GetActorLocation(), TargetPlyaerCharacter->GetActorLocation(), 7.0f);//미사일이 이동할 벡터를 구함
	SetActorLocation(MissilePos);//미사일 이동

	FRotator RotToTarget = FRotationMatrix::MakeFromZ(Between).Rotator();
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), RotToTarget, GetWorld()->GetDeltaSeconds(), 15.0f));

	if (MissilePos == TargetPlyaerCharacter->GetActorLocation()) Destroy();
}

FVector AShooterBossMissile::MoveToward(FVector Start, FVector End, float Alpah)
{
	FVector a = End - Start;
	float magnitude = a.Size();//벡터의 magnitude구함
	if (magnitude <= Alpah || magnitude == 0.0f)
	{
		return End;
	}
	return Start + a / magnitude * Alpah;//시작지점에서 유닛벡터*Alpha값을 더한값을 반환
}

// Called when the game starts or when spawned
void AShooterBossMissile::BeginPlay()
{
	Super::BeginPlay();
	
	//이터레이터를 이용해 플레이어 폰을 찾아내어 가장 가까이 있는 플레이어 설정
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

	MissileTracking();//틱마다 미사일 트랙킹 활성화
}