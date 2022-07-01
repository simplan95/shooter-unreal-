// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGripGrenade.h"
#include "Shooter2Character.h"
#include "ShooterActorComponent.h"//ĳ���� ����

//�ʵ忡�� �÷��̾ �ֿ� �� �ִ� ����ź
AShooterGripGrenade::AShooterGripGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;//ƽ��Ȱ��ȭ
	//�����ʱ�ȭ
	Grenade = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GREANDE"));//���̷�Ż �޽�
	GetSound = CreateDefaultSubobject<USoundBase>(TEXT("GETSOUND"));//ȹ�� ����

	//��Ʈ���� ����
	RootComponent = Grenade;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_GRENADE(TEXT("/Game/GrenadeKit/Meshes/MK2_Grenade_SK.MK2_Grenade_SK"));//����ź �޽�
	if (SK_GRENADE.Succeeded())
	{
		Grenade->SetSkeletalMesh(SK_GRENADE.Object);
		Grenade->SetRelativeScale3D(FVector(1.5f,1.5f,1.5));//����ź ũ������
		Grenade->SetSimulatePhysics(true);//����ź �޽� �������� On
	}
	
	static ConstructorHelpers::FObjectFinder<USoundBase>SB_Get(TEXT("/Game/MyAsset/SC_GetSound.SC_GetSound"));//�ݱ� ����
	if (SB_Get.Succeeded())
	{
		GetSound = SB_Get.Object;
	}
	
	//�ݸ��� ����
	Grenade->SetCollisionProfileName(TEXT("GunMesh"));
	Grenade->SetGenerateOverlapEvents(true);//����ź �޽ø� ������ �̺�Ʈ�� Ȱ��ȭ ��Ŵ

	GrenadeItemClass = AShooterGripGrenade::StaticClass();//Ŭ���� ���
}

// Called when the game starts or when spawned
void AShooterGripGrenade::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShooterGripGrenade::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Grenade->OnComponentBeginOverlap.AddDynamic(this, &AShooterGripGrenade::OnCharacterOverLap);// ĳ���� ������ ó��
}
	

// Called every frame
void AShooterGripGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShooterGripGrenade::OnCharacterOverLap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)//������ �̺�Ʈ�� ó���ϴ� ��������Ʈ �Լ�
{
	auto ShooterCharacter = Cast<AShooter2Character>(OtherActor);//�÷��̾� ����ĳ�� ����
	if (nullptr != GrenadeItemClass && nullptr != ShooterCharacter)
	{
		if (ShooterCharacter->CharacterStat->GetGrenadeCount() >= 0 && ShooterCharacter->CharacterStat->GetGrenadeCount() < 5)
		{
			ShooterCharacter->CharacterStat->SetGrenadeCount(1);//�÷��̾� ����ź 1�� ���� �� ��ε� ĳ��Ʈ
			Grenade->SetHiddenInGame(true, true);//����ź ����, USkeletalMeshComponent������ ��밡��
			SetActorEnableCollision(false);//�ݸ��� ��Ȱ��ȭ
			UGameplayStatics::PlaySoundAtLocation(this, GetSound, GetActorLocation());//ȹ����� ����
			Destroy();//���� Destroy
		}
	}
}