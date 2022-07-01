// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGripGrenade.h"
#include "Shooter2Character.h"
#include "ShooterActorComponent.h"//캐릭터 스탯

//필드에서 플레이어가 주울 수 있는 수류탄
AShooterGripGrenade::AShooterGripGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;//틱비활성화
	//변수초기화
	Grenade = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GREANDE"));//스켈레탈 메시
	GetSound = CreateDefaultSubobject<USoundBase>(TEXT("GETSOUND"));//획득 사운드

	//루트컴포 지정
	RootComponent = Grenade;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_GRENADE(TEXT("/Game/GrenadeKit/Meshes/MK2_Grenade_SK.MK2_Grenade_SK"));//수류탄 메시
	if (SK_GRENADE.Succeeded())
	{
		Grenade->SetSkeletalMesh(SK_GRENADE.Object);
		Grenade->SetRelativeScale3D(FVector(1.5f,1.5f,1.5));//수류탄 크기조정
		Grenade->SetSimulatePhysics(true);//수류탄 메시 물리적용 On
	}
	
	static ConstructorHelpers::FObjectFinder<USoundBase>SB_Get(TEXT("/Game/MyAsset/SC_GetSound.SC_GetSound"));//줍기 사운드
	if (SB_Get.Succeeded())
	{
		GetSound = SB_Get.Object;
	}
	
	//콜리전 지정
	Grenade->SetCollisionProfileName(TEXT("GunMesh"));
	Grenade->SetGenerateOverlapEvents(true);//수류탄 메시를 오버랩 이벤트를 활성화 시킴

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

	Grenade->OnComponentBeginOverlap.AddDynamic(this, &AShooterGripGrenade::OnCharacterOverLap);// 캐릭터 오버랩 처리
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
			Grenade->SetHiddenInGame(true, true);//수류탄 숨김, USkeletalMeshComponent에서만 사용가능
			SetActorEnableCollision(false);//콜리전 비활성화
			UGameplayStatics::PlaySoundAtLocation(this, GetSound, GetActorLocation());//획득사운드 실행
			Destroy();//액터 Destroy
		}
	}
}