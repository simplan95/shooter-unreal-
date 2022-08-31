// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGripGun.h"
#include "Sound/SoundCue.h"//사운드 큐

//플레이어가 쏠 총 구현
AShooterGripGun::AShooterGripGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));//총메시 초기화
	Gun->SetCollisionProfileName(TEXT("NoCollision"));//총 콜리전 설정
	RootComponent = Gun;//루트컴포넌트 지정

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_GUN(TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4"));
	if (SK_GUN.Succeeded())
	{
		Gun->SetSkeletalMesh(SK_GUN.Object);
	}

	MuzzleScene = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));//머즐 신
	MuzzleScene->SetupAttachment(RootComponent);//머즐신 루트컴포넌트에 부착
	MuzzleScene->SetRelativeLocation(FVector(0.0f, 60.0f, 10.5f));//머즐 신 위치조정
	MuzzleScene->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));//머즐 신 회전조정

	MyFireParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticle"));//발사 파티클
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_Fire(TEXT("/Game/MyAsset/PS_MuzzleFlash.PS_MuzzleFlash"));
	if (PT_Fire.Succeeded())
	{
		MyFireParticle->SetTemplate(PT_Fire.Object);
		MyFireParticle->bAutoActivate = false;//자동실행 false
		MyFireParticle->SetupAttachment(MuzzleScene);//파티클을 머즐 신에 부착
	}
	

	
	SingleEjectParticle= CreateDefaultSubobject<UParticleSystem>(TEXT("SingleEject"));//단발 이젝트
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_SingleEject(TEXT("/Game/MyAsset/PS_Shell_Single.PS_Shell_Single"));
	if (PT_SingleEject.Succeeded())
	{
		SingleEjectParticle=PT_SingleEject.Object;
	}

	ReloadSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Reload"));//재장전사운드
	ReloadSound->bAutoActivate = false;
	ReloadSound->AttachTo(RootComponent);
	ConstructorHelpers::FObjectFinder<USoundCue>MyReloadSound(TEXT("/Game/MyAsset/SC_Reload.SC_Reload"));
	if (MyReloadSound.Succeeded())
	{
		ReloadSound->SetSound(MyReloadSound.Object);
	}

	FireSound = CreateDefaultSubobject<USoundBase>(TEXT("Fire"));//총 발사 사운드
	static ConstructorHelpers::FObjectFinder<USoundBase>SB_Fire(TEXT("/Game/MyAsset/SC_Fire.SC_Fire"));
	if (SB_Fire.Succeeded())
	{
		FireSound = SB_Fire.Object;
	}

	ModeChangeSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ModeChange"));//모드 변경 사운드
	ModeChangeSound->SetupAttachment(RootComponent);
	ModeChangeSound->bAutoActivate = false;//자동실행 false
	static ConstructorHelpers::FObjectFinder<USoundCue>SC_ModeChange(TEXT("/Game/MyAsset/SC_ModeChange.SC_ModeChange"));
	if (SC_ModeChange.Succeeded())
	{
		ModeChangeSound->SetSound(SC_ModeChange.Object);
	}

	bSingleMode = true;//초기는 총 단발모드 활성화
}

void AShooterGripGun::FirePaticle()//발사 파티클 활성화
{
	MyFireParticle->Activate();
}

void AShooterGripGun::SingleEject()//단발 탄피
{
	//탄피 배출 파티클을 지정한 소켓에서작동
	FName EjectSocket(TEXT("MyEjectSoket"));
	UGameplayStatics::SpawnEmitterAttached(SingleEjectParticle, Gun, EjectSocket, FVector::ZeroVector, FRotator::ZeroRotator, FVector(0.3f));
}

void AShooterGripGun::PlayReloadSound()//재장전 사운드
{
	ReloadSound->Play();
}

void AShooterGripGun::PlayFireSound()//총 발사 사운드
{
	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
}

bool AShooterGripGun::GetMode()//무기 모드 반환
{
	return bSingleMode;
}

void AShooterGripGun::ModeChange()//무기 모드 변경
{
	bSingleMode = !bSingleMode;
	ModeChangeSound->Play();//모드 변경 사운트 실행
}

// Called when the game starts or when spawned
void AShooterGripGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShooterGripGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

