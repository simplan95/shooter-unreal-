// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGun.h"

#include "Shooter2Character.h"
#include "ShooterGripGun.h"
#include "ShooterActorComponent.h"//총알관리용 

// Sets default values

//필드에서 플레이어가 주울 수 있는 총
AShooterGun::AShooterGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//인스턴스 초기화
	Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GUN"));//스켈레탈 메시
	GetSound = CreateDefaultSubobject<USoundBase>(TEXT("Fire"));//총 획득 사운드

	RootComponent = Gun;//루트컴포넌트 지정

	//총, 이펙트 지정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_GUN(TEXT("/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4"));//총 메시
	if (SK_GUN.Succeeded())
	{
		Gun->SetSkeletalMesh(SK_GUN.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundBase>SB_Fire(TEXT("/Game/MyAsset/SC_GetSound.SC_GetSound"));//줍기 사운드
	if (SB_Fire.Succeeded())
	{
		GetSound = SB_Fire.Object;
	}

	Gun->SetRelativeLocationAndRotation(FVector(0.0f, -5.0f, 10.0f), FRotator(0.0f, 0.0f, 90.0f));//총 메시 위치조정
	Gun->SetSimulatePhysics(true);//총메시 물리적용 On
	Gun->SetCollisionProfileName(TEXT("GunMesh"));//메시 콜리전 지정
	Gun->SetGenerateOverlapEvents(true);//건 메시를 오버랩 이벤트를 활성화 시킴

	GunItemClass = AShooterGripGun::StaticClass();//무기 클래스 등록
}

void AShooterGun::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Gun->OnComponentBeginOverlap.AddDynamic(this, &AShooterGun::OnCharacterOverLap);// 캐릭터 오버랩 처리
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
			Gun->SetHiddenInGame(true, true);//총 숨김, USkeletalMeshComponent에서만 사용가능
			SetActorEnableCollision(false);//콜리전 비활성화
			UGameplayStatics::PlaySoundAtLocation(this, GetSound, GetActorLocation());//총줍기 사운드 실행
			ShooterCharacter->SetWeapon(NewWeapon);//캐릭터에 총부착
			ShooterCharacter->CharacterStat->SetMagazineCount(30);//현재 소지중인 총에 총알 추가
			Destroy();//액터 Destroy
		}
		else if(!ShooterCharacter->CanSetWeapon() && ShooterCharacter->CharacterStat->GetMaxBullet() <150)//이미 플레이어가 무기를 장착하고 있을 경우
		{
			Gun->SetHiddenInGame(true, true);//총 숨김, USkeletalMeshComponent에서만 사용가능
			SetActorEnableCollision(false);//콜리전 비활성화
			UGameplayStatics::PlaySoundAtLocation(this, GetSound, GetActorLocation());//총줍기 사운드 실행
			ShooterCharacter->CharacterStat->SetMaxBullet(30);//예비 총알 추가
			if (ShooterCharacter->CharacterStat->GetMaxBullet() > 150) ShooterCharacter->CharacterStat->SetMaxBulletQuantity(150);//150발을 넘길시 150발로 지정
			Destroy();//액터 Destroy
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

