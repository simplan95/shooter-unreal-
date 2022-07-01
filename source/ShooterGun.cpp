// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGun.h"

#include "Shooter2Character.h"
#include "ShooterGripGun.h"
#include "ShooterActorComponent.h"//�Ѿ˰����� 

// Sets default values

//�ʵ忡�� �÷��̾ �ֿ� �� �ִ� ��
AShooterGun::AShooterGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�ν��Ͻ� �ʱ�ȭ
	Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GUN"));//���̷�Ż �޽�
	GetSound = CreateDefaultSubobject<USoundBase>(TEXT("Fire"));//�� ȹ�� ����

	RootComponent = Gun;//��Ʈ������Ʈ ����

	//��, ����Ʈ ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_GUN(TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4"));//�� �޽�
	if (SK_GUN.Succeeded())
	{
		Gun->SetSkeletalMesh(SK_GUN.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundBase>SB_Fire(TEXT("/Game/MyAsset/SC_GetSound.SC_GetSound"));//�ݱ� ����
	if (SB_Fire.Succeeded())
	{
		GetSound = SB_Fire.Object;
	}

	Gun->SetRelativeLocationAndRotation(FVector(0.0f, -5.0f, 10.0f), FRotator(0.0f, 0.0f, 90.0f));//�� �޽� ��ġ����
	Gun->SetSimulatePhysics(true);//�Ѹ޽� �������� On
	Gun->SetCollisionProfileName(TEXT("GunMesh"));//�޽� �ݸ��� ����
	Gun->SetGenerateOverlapEvents(true);//�� �޽ø� ������ �̺�Ʈ�� Ȱ��ȭ ��Ŵ

	GunItemClass = AShooterGripGun::StaticClass();//���� Ŭ���� ���
}

void AShooterGun::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Gun->OnComponentBeginOverlap.AddDynamic(this, &AShooterGun::OnCharacterOverLap);// ĳ���� ������ ó��
}

void AShooterGun::OnCharacterOverLap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)//������ �̺�Ʈ�� ó���ϴ� ��������Ʈ �Լ�
{
	auto ShooterCharacter = Cast<AShooter2Character>(OtherActor);//�÷��̾� ����ĳ�� ����
	if (nullptr != GunItemClass && nullptr != ShooterCharacter)
	{
		if (ShooterCharacter->CanSetWeapon())//�÷��̾ ���� ������ ������ ���
		{
			auto NewWeapon = GetWorld()->SpawnActor<AShooterGripGun>(GunItemClass, FVector::ZeroVector, FRotator::ZeroRotator);//�ѻ���
			Gun->SetHiddenInGame(true, true);//�� ����, USkeletalMeshComponent������ ��밡��
			SetActorEnableCollision(false);//�ݸ��� ��Ȱ��ȭ
			UGameplayStatics::PlaySoundAtLocation(this, GetSound, GetActorLocation());//���ݱ� ���� ����
			ShooterCharacter->SetWeapon(NewWeapon);//ĳ���Ϳ� �Ѻ���
			ShooterCharacter->CharacterStat->SetMagazineCount(30);//���� �������� �ѿ� �Ѿ� �߰�
			Destroy();//���� Destroy
		}
		else if(!ShooterCharacter->CanSetWeapon() && ShooterCharacter->CharacterStat->GetMaxBullet() <150)//�̹� �÷��̾ ���⸦ �����ϰ� ���� ���
		{
			Gun->SetHiddenInGame(true, true);//�� ����, USkeletalMeshComponent������ ��밡��
			SetActorEnableCollision(false);//�ݸ��� ��Ȱ��ȭ
			UGameplayStatics::PlaySoundAtLocation(this, GetSound, GetActorLocation());//���ݱ� ���� ����
			ShooterCharacter->CharacterStat->SetMaxBullet(30);//���� �Ѿ� �߰�
			if (ShooterCharacter->CharacterStat->GetMaxBullet() > 150) ShooterCharacter->CharacterStat->SetMaxBulletQuantity(150);//150���� �ѱ�� 150�߷� ����
			Destroy();//���� Destroy
		}
		
	}
}

// Called when the game starts or when spawned
void AShooterGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShooterGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

