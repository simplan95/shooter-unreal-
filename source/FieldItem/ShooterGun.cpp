// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGun.h"
#include "Shooter2Character.h"
#include "ShooterActorComponent.h"//총알관리용 

//필드에 존재하는 총 아이템 
AShooterGun::AShooterGun()
{
	PrimaryActorTick.bCanEverTick = false;

	//총, 이펙트 지정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_GUN(TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4"));//총 메시
	if (SK_GUN.Succeeded())
	{
		itemMesh->SetSkeletalMesh(SK_GUN.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundBase>SB_Fire(TEXT("/Game/MyAsset/SC_GetSound.SC_GetSound"));//줍기 사운드
	if (SB_Fire.Succeeded())
	{
		getSound = SB_Fire.Object;
	}

	GunItemClass = AShooterGripGun::StaticClass();//무기 클래스 등록
}

void AShooterGun::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	itemMesh->OnComponentBeginOverlap.AddDynamic(this, &AShooterGun::OnCharacterOverLap);// 캐릭터 오버랩 처리
}

void AShooterGun::OnCharacterOverLap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)//오버랩 이벤트를 처리하는 델리게이트 함수
{
	auto ShooterCharacter = Cast<AShooter2Character>(OtherActor);//플레이어 슈터캐릭 설정
	if (nullptr != GunItemClass && nullptr != ShooterCharacter)
	{
		if (ShooterCharacter->CanSetWeapon())//플레이어가 무기 장착이 가능할 경우
		{
			auto NewWeapon = GetWorld()->SpawnActor<AShooterGripGun>(GunItemClass, FVector::ZeroVector, FRotator::ZeroRotator);//총생성
			itemMesh->SetHiddenInGame(true, true);//총 숨김, USkeletalMeshComponent에서만 사용가능
			SetActorEnableCollision(false);//콜리전 비활성화
			UGameplayStatics::PlaySoundAtLocation(this, getSound, GetActorLocation());//총줍기 사운드 실행
			ShooterCharacter->SetWeapon(NewWeapon);//캐릭터에 총부착
			ShooterCharacter->CharacterStat->SetMagazineCount(30);//현재 소지중인 총에 총알 추가
			Destroy();//액터 Destroy
		}
		else if(!ShooterCharacter->CanSetWeapon() && ShooterCharacter->CharacterStat->GetMaxBullet() <150)//이미 플레이어가 무기를 장착하고 있을 경우
		{
			itemMesh->SetHiddenInGame(true, true);//총 숨김, USkeletalMeshComponent에서만 사용가능
			SetActorEnableCollision(false);//콜리전 비활성화
			UGameplayStatics::PlaySoundAtLocation(this, getSound, GetActorLocation());//총줍기 사운드 실행
			ShooterCharacter->CharacterStat->SetMaxBullet(30);//예비 총알 추가
			if (ShooterCharacter->CharacterStat->GetMaxBullet() > 150) ShooterCharacter->CharacterStat->SetMaxBulletQuantity(150);//150발을 넘길시 150발로 지정
			Destroy();//액터 Destroy
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

