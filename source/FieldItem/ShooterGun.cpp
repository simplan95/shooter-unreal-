// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGun.h"
#include "Shooter2Character.h"
#include "ShooterActorComponent.h"//�Ѿ˰����� 

//�ʵ忡 �����ϴ� �� ������ 
AShooterGun::AShooterGun()
{
	PrimaryActorTick.bCanEverTick = false;

	//��, ����Ʈ ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_GUN(TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4"));//�� �޽�
	if (SK_GUN.Succeeded())
	{
		itemMesh->SetSkeletalMesh(SK_GUN.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundBase>SB_Fire(TEXT("/Game/MyAsset/SC_GetSound.SC_GetSound"));//�ݱ� ����
	if (SB_Fire.Succeeded())
	{
		getSound = SB_Fire.Object;
	}

	GunItemClass = AShooterGripGun::StaticClass();//���� Ŭ���� ���
}

void AShooterGun::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	itemMesh->OnComponentBeginOverlap.AddDynamic(this, &AShooterGun::OnCharacterOverLap);// ĳ���� ������ ó��
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
			itemMesh->SetHiddenInGame(true, true);//�� ����, USkeletalMeshComponent������ ��밡��
			SetActorEnableCollision(false);//�ݸ��� ��Ȱ��ȭ
			UGameplayStatics::PlaySoundAtLocation(this, getSound, GetActorLocation());//���ݱ� ���� ����
			ShooterCharacter->SetWeapon(NewWeapon);//ĳ���Ϳ� �Ѻ���
			ShooterCharacter->CharacterStat->SetMagazineCount(30);//���� �������� �ѿ� �Ѿ� �߰�
			Destroy();//���� Destroy
		}
		else if(!ShooterCharacter->CanSetWeapon() && ShooterCharacter->CharacterStat->GetMaxBullet() <150)//�̹� �÷��̾ ���⸦ �����ϰ� ���� ���
		{
			itemMesh->SetHiddenInGame(true, true);//�� ����, USkeletalMeshComponent������ ��밡��
			SetActorEnableCollision(false);//�ݸ��� ��Ȱ��ȭ
			UGameplayStatics::PlaySoundAtLocation(this, getSound, GetActorLocation());//���ݱ� ���� ����
			ShooterCharacter->CharacterStat->SetMaxBullet(30);//���� �Ѿ� �߰�
			if (ShooterCharacter->CharacterStat->GetMaxBullet() > 150) ShooterCharacter->CharacterStat->SetMaxBulletQuantity(150);//150���� �ѱ�� 150�߷� ����
			Destroy();//���� Destroy
		}
		
	}
}

void AShooterGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShooterGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

