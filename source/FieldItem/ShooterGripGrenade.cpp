// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGripGrenade.h"
#include "Shooter2Character.h"//�÷��̾� ĳ����
#include "ShooterGripGun.h"//�÷��̾� ��
#include "ShooterActorComponent.h"//ĳ���� ����

//�ʵ忡 �����ϴ� ����ź ������
AShooterGripGrenade::AShooterGripGrenade()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_GRENADE(TEXT("/Game/GrenadeKit/Meshes/MK2_Grenade_SK.MK2_Grenade_SK"));//����ź �޽�
	if (SK_GRENADE.Succeeded())
	{
		itemMesh->SetSkeletalMesh(SK_GRENADE.Object);
		itemMesh->SetRelativeScale3D(FVector(1.5f,1.5f,1.5));//����ź ũ������
	}
	
	static ConstructorHelpers::FObjectFinder<USoundBase>SB_Get(TEXT("/Game/MyAsset/SC_GetSound.SC_GetSound"));//�ݱ� ����
	if (SB_Get.Succeeded())
	{
		getSound = SB_Get.Object;
	}

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

	itemMesh->OnComponentBeginOverlap.AddDynamic(this, &AShooterGripGrenade::OnCharacterOverLap);// ĳ���� ������ ó��
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
			itemMesh->SetHiddenInGame(true, true);//����ź ����, USkeletalMeshComponent������ ��밡��
			SetActorEnableCollision(false);//�ݸ��� ��Ȱ��ȭ
			UGameplayStatics::PlaySoundAtLocation(this, getSound, GetActorLocation());//ȹ����� ����
			Destroy();//���� Destroy
		}
	}
}