// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGripGun.h"
#include "Sound/SoundCue.h"//���� ť

//�÷��̾ �� �� ����
AShooterGripGun::AShooterGripGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));//�Ѹ޽� �ʱ�ȭ
	Gun->SetCollisionProfileName(TEXT("NoCollision"));//�� �ݸ��� ����
	RootComponent = Gun;//��Ʈ������Ʈ ����

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_GUN(TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4"));
	if (SK_GUN.Succeeded())
	{
		Gun->SetSkeletalMesh(SK_GUN.Object);
	}

	MuzzleScene = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));//���� ��
	MuzzleScene->SetupAttachment(RootComponent);//����� ��Ʈ������Ʈ�� ����
	MuzzleScene->SetRelativeLocation(FVector(0.0f, 60.0f, 10.5f));//���� �� ��ġ����
	MuzzleScene->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));//���� �� ȸ������

	MyFireParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticle"));//�߻� ��ƼŬ
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_Fire(TEXT("/Game/MyAsset/PS_MuzzleFlash.PS_MuzzleFlash"));
	if (PT_Fire.Succeeded())
	{
		MyFireParticle->SetTemplate(PT_Fire.Object);
		MyFireParticle->bAutoActivate = false;//�ڵ����� false
		MyFireParticle->SetupAttachment(MuzzleScene);//��ƼŬ�� ���� �ſ� ����
	}
	

	
	SingleEjectParticle= CreateDefaultSubobject<UParticleSystem>(TEXT("SingleEject"));//�ܹ� ����Ʈ
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_SingleEject(TEXT("/Game/MyAsset/PS_Shell_Single.PS_Shell_Single"));
	if (PT_SingleEject.Succeeded())
	{
		SingleEjectParticle=PT_SingleEject.Object;
	}

	ReloadSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Reload"));//����������
	ReloadSound->bAutoActivate = false;
	ReloadSound->AttachTo(RootComponent);
	ConstructorHelpers::FObjectFinder<USoundCue>MyReloadSound(TEXT("/Game/MyAsset/SC_Reload.SC_Reload"));
	if (MyReloadSound.Succeeded())
	{
		ReloadSound->SetSound(MyReloadSound.Object);
	}

	FireSound = CreateDefaultSubobject<USoundBase>(TEXT("Fire"));//�� �߻� ����
	static ConstructorHelpers::FObjectFinder<USoundBase>SB_Fire(TEXT("/Game/MyAsset/SC_Fire.SC_Fire"));
	if (SB_Fire.Succeeded())
	{
		FireSound = SB_Fire.Object;
	}

	ModeChangeSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ModeChange"));//��� ���� ����
	ModeChangeSound->SetupAttachment(RootComponent);
	ModeChangeSound->bAutoActivate = false;//�ڵ����� false
	static ConstructorHelpers::FObjectFinder<USoundCue>SC_ModeChange(TEXT("/Game/MyAsset/SC_ModeChange.SC_ModeChange"));
	if (SC_ModeChange.Succeeded())
	{
		ModeChangeSound->SetSound(SC_ModeChange.Object);
	}

	bSingleMode = true;//�ʱ�� �� �ܹ߸�� Ȱ��ȭ
}

void AShooterGripGun::FirePaticle()//�߻� ��ƼŬ Ȱ��ȭ
{
	MyFireParticle->Activate();
}

void AShooterGripGun::SingleEject()//�ܹ� ź��
{
	//ź�� ���� ��ƼŬ�� ������ ���Ͽ����۵�
	FName EjectSocket(TEXT("MyEjectSoket"));
	UGameplayStatics::SpawnEmitterAttached(SingleEjectParticle, Gun, EjectSocket, FVector::ZeroVector, FRotator::ZeroRotator, FVector(0.3f));
}

void AShooterGripGun::PlayReloadSound()//������ ����
{
	ReloadSound->Play();
}

void AShooterGripGun::PlayFireSound()//�� �߻� ����
{
	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
}

bool AShooterGripGun::GetMode()//���� ��� ��ȯ
{
	return bSingleMode;
}

void AShooterGripGun::ModeChange()//���� ��� ����
{
	bSingleMode = !bSingleMode;
	ModeChangeSound->Play();//��� ���� ���Ʈ ����
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

