// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGripGrenade.h"
#include "Shooter2Character.h"//플레이어 캐릭터
#include "ShooterGripGun.h"//플레이어 총
#include "ShooterActorComponent.h"//캐릭터 스탯

//필드에 존재하는 수류탄 아이템
AShooterGripGrenade::AShooterGripGrenade()
{
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_GRENADE(TEXT("/Game/GrenadeKit/Meshes/MK2_Grenade_SK.MK2_Grenade_SK"));//수류탄 메시
	if (SK_GRENADE.Succeeded())
	{
		itemMesh->SetSkeletalMesh(SK_GRENADE.Object);
		itemMesh->SetRelativeScale3D(FVector(1.5f,1.5f,1.5));//수류탄 크기조정
	}
	
	static ConstructorHelpers::FObjectFinder<USoundBase>SB_Get(TEXT("/Game/MyAsset/SC_GetSound.SC_GetSound"));//줍기 사운드
	if (SB_Get.Succeeded())
	{
		getSound = SB_Get.Object;
	}

	GrenadeItemClass = AShooterGripGrenade::StaticClass();//클래스 등록
}

// Called when the game starts or when spawned
void AShooterGripGrenade::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShooterGripGrenade::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	itemMesh->OnComponentBeginOverlap.AddDynamic(this, &AShooterGripGrenade::OnCharacterOverLap);// 캐릭터 오버랩 처리
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
)//오버랩 이벤트를 처리하는 델리게이트 함수
{
	auto ShooterCharacter = Cast<AShooter2Character>(OtherActor);//플레이어 슈터캐릭 설정
	if (nullptr != GrenadeItemClass && nullptr != ShooterCharacter)
	{
		if (ShooterCharacter->CharacterStat->GetGrenadeCount() >= 0 && ShooterCharacter->CharacterStat->GetGrenadeCount() < 5)
		{
			ShooterCharacter->CharacterStat->SetGrenadeCount(1);//플레이어 수류탄 1개 증가 및 브로드 캐스트
			itemMesh->SetHiddenInGame(true, true);//수류탄 숨김, USkeletalMeshComponent에서만 사용가능
			SetActorEnableCollision(false);//콜리전 비활성화
			UGameplayStatics::PlaySoundAtLocation(this, getSound, GetActorLocation());//획득사운드 실행
			Destroy();//액터 Destroy
		}
	}
}