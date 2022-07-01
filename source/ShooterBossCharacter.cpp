// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterBossCharacter.h"
#include "ShooterBossAIController.h"//Boss AI
#include "ShooterGripGun.h"//장착 아이템 총
#include "DrawDebugHelpers.h"//디버그 그리기
#include "Runtime/Engine/Public/TimerManager.h"//타이머 매니저
#include "ShooterActorComponent.h"//캐릭터 스탯
#include "Components/WidgetComponent.h"//UI위젯 헤더파일 
#include "ShooterCharacterWidget.h"//내 캐릭터 위젯
#include "ShooterGrenade.h"//수류탄
#include "ShooterBossMissile.h"//미사일
#include "ShooterAICharacter.h"//AI캐릭터
#include "ShooterGameState.h"//게임 스테이트 (보스가 죽었을 때 클리어 설정)
#include "Shooter2GameMode.h"//게임모드

#include "ShooterCharacterPlayerController.h"//플레이어컨트롤러
#include "Shooter2Character.h"//플레이어 캐릭터
#include "ShooterHUDWidget.h"//HUD위젯

//AI 보스 캐릭터의 설정 및 기능구현
AShooterBossCharacter::AShooterBossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DeadTimer = 3.0f;//데드 타이머 시간(보스 사망시)
	WidgetDeadTimer = 3.0f;//위젯 데드 타이머 시간(위젯의 지속시간)
	StandSpeed = 400.0f;//속도

	AIControllerClass = AShooterBossAIController::StaticClass();//AI Control 클래스를 지정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;//스폰되거나 월드에 있을 때 자동으로 AI컨트롤러로 빙의

	// 충돌 캡슐의 크기 설정
	GetCapsuleComponent()->InitCapsuleSize(160.0f, 160.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("AIChara"));//콜리전 설정

	// 캐릭터 움직임 구성(회전을 부드럽게)
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;//부드러운 회전
	GetCharacterMovement()->bOrientRotationToMovement = true; // 캐릭터가 입력 방향으로 회전합니다	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // 이 회전 속도로
	GetCharacterMovement()->JumpZVelocity = 450.0f;//점프 높이
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->bCanWalkOffLedges = true;//앉았을 때 절벽에서 떨어지기 가능
	GetCharacterMovement()->MaxWalkSpeed = StandSpeed;//캐릭터 스피드 지정

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_Body(TEXT("/Game/MyAsset/BOSS/BossMesh.BossMesh"));//캐릭터 스킨 메시
	if (SK_Body.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Body.Object);
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));//메시는 콜리전 비활성
	}

	ImpactParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("ImpactParticle"));//총알 임팩트 파티클
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_Impact(TEXT("/Game/MyAsset/PS_Impact.PS_Impact"));
	if (PT_Impact.Succeeded())
	{
		ImpactParticle = PT_Impact.Object;
	}

	PitanSound = CreateDefaultSubobject<USoundBase>(TEXT("Pitan"));//피탄 사운드
	static ConstructorHelpers::FObjectFinder<USoundBase>SB_PitanSound(TEXT("/Game/MyAsset/SC_Pitan.SC_Pitan"));
	if (SB_PitanSound.Succeeded())
	{
		PitanSound = SB_PitanSound.Object;
	}

	HP=1500.0f
	CharacterStat->SetCurrentHP(HP);//HP스탯지정
	CharacterStat->SetMaxHP(HP);//max HP스탯지정

	HPBarWidget->SetupAttachment(GetMesh());//위젯을 메쉬에 부착

	BossMissileScene = CreateDefaultSubobject<USceneComponent>(TEXT("BossMissileScene"));//미사일 소환 신
	BossMissileScene->SetupAttachment(RootComponent);//루트 컴포넌트에 부착
	BossMissileScene->SetRelativeLocation(FVector(50.0f,40.0f,130.0f));//미사일 소환신 위치설정

	MissileFireParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MissileFireParticle"));//발사 파티클
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_FireMissile(TEXT("/Game/MyAsset/PS_MissileFlash.PS_MissileFlash"));
	if (PT_FireMissile.Succeeded())
	{
		MissileFireParticle->SetTemplate(PT_FireMissile.Object);
		MissileFireParticle->bAutoActivate = false;//자동실행 Off
		MissileFireParticle->SetupAttachment(BossMissileScene);//파티클을 머즐 신에 부착
	}
	

	MissileFireSound = CreateDefaultSubobject<USoundBase>(TEXT("MissileFireSound"));//미사일 발사 사운드
	static ConstructorHelpers::FObjectFinder<USoundBase>SB_FireMissile(TEXT("/Game/MyAsset/SC_MissileLaunch.SC_MissileLaunch"));
	if (SB_FireMissile.Succeeded())
	{
		MissileFireSound = SB_FireMissile.Object;
	}

	MissileHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MISSILEHEAD"));//미사일 발사대 몸체
	MissileLauncher = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MISSILELAUNCCHER"));//미사일 발사대

	FString AssetPath = TEXT("/Game/MyAsset/MissleLauncher/Mesh/StaticM_MissileLauncherTurret_Body.StaticM_MissileLauncherTurret_Body");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_TURRETHEAD(*AssetPath);//미사일 발사대 몸체 메시 불러오기
	if (SM_TURRETHEAD.Succeeded())
	{
		MissileHead->SetStaticMesh(SM_TURRETHEAD.Object);
		MissileHead->SetCollisionProfileName(TEXT("NoCollision"));//메시는 콜리전 비활성
		MissileHead->SetRelativeScale3D(FVector(0.8f,0.8f,0.6f));//메시크기 설정
	}

	FString AssetPath2 = TEXT("/Game/MyAsset/MissleLauncher/Mesh/StaticM_MissileLauncherTurret_Launcher.StaticM_MissileLauncherTurret_Launcher");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_TURRETLAUNCHER(*AssetPath2);//미사일 발사대 메시 불러오기
	if (SM_TURRETLAUNCHER.Succeeded())
	{
		MissileLauncher->SetStaticMesh(SM_TURRETLAUNCHER.Object);
		MissileLauncher->SetCollisionProfileName(TEXT("NoCollision"));//메시는 콜리전 비활성
	}

	FName MissileHeadSocket(TEXT("MissileHead"));
	if (MissileHead->DoesSocketExist(MissileHeadSocket))//미사일 발사대 몸체 메시에 해당 소켓이 존재할경우
	{
		MissileLauncher->SetupAttachment(MissileHead, MissileHeadSocket);//발사대를 발사대 몸체에 부착
	}

	FName MissileSocket(TEXT("MissileSoket"));
	MissileHead->SetupAttachment(GetMesh(), MissileSocket);//미사일런처를 보스 소켓에 부착

	for (int i = 0; i < 2; ++i)//수류탄 생성 신 초기화
	{
		//신 초기화
		FString SceneName = "Scene" + FString::FromInt(i);
		FName SceneID = FName(*SceneName);
		USceneComponent* thisScene = CreateDefaultSubobject<USceneComponent>(SceneID);
		check(thisScene);

		BossGrenadeScene.Add(thisScene);//BossGrenadeScene 배열 구조체에 신을 추가
	}

	float SpawnPoint = -50.0f;
	for (auto Scene : BossGrenadeScene)//각각 수류탄 생성 신의 위치를 설정
	{
		Scene->SetupAttachment(RootComponent);
		Scene->SetRelativeLocation(FVector(50.0f, SpawnPoint, 140.0f));
		SpawnPoint -= 10.0f;
	}


}

// Called when the game starts or when spawned
void AShooterBossCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	ShooterAIController = Cast<AShooterBossAIController>(GetController());
	

	//총생성 및 무기부착
	FName WeaponSocket(TEXT("GunSoket"));
	NewWeapon = GetWorld()->SpawnActor<AShooterGripGun>(FVector::ZeroVector, FRotator::ZeroRotator);//총생성
	NewWeapon->SetActorScale3D(FVector(2.5f));
	NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);//해당 컴포넌트를 해당 소켓에 부착
	NewWeapon->SetOwner(this);//소유자는 본인

	SetCharacterState(ECharacterState::READY);//보스의 상태 설정

}

void AShooterBossCharacter::SetCharacterState(ECharacterState NewState)
{
	CurrentState = NewState;//현재 스테이트 설정

	switch (CurrentState)
	{
		case ECharacterState::READY://준비상태
		{
			SetActorHiddenInGame(false);//액터 보임
			HPBarWidget->SetHiddenInGame(true);//HP 위젯 숨김

			CharacterStat->OnHPIsZero.AddLambda([this]()->void {//HP 0시 델리게이트를 사용하여 DEAD 스테이트로 설정
				SetCharacterState(ECharacterState::DEAD);
				});


			break;
		}
		case ECharacterState::DEAD://죽음 상태
		{
			CharacterDead();//캐릭터 죽음
			auto PlayerController = Cast<AShooterCharacterPlayerController>(this->LastHitBy);//본인을 마지막으로 때린 컨트롤러를 반환
			auto GameMode = Cast<AShooter2GameMode>(GetWorld()->GetAuthGameMode());//게임모드를 가져옴
			GameMode->AddScore(PlayerController);//해당 플레이어 점수 추가
			GameMode->ShooterGameState->SetGameCleared();//게임 클리어 선언

			//3초뒤 게임 클리어 화면 표시
			GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
					//게임완료시 이터레이터를 이용하여 플레이어 폰 및 NPC 폰 비활성화
					for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++)
					{
						(*It)->TurnOff();
					}

					//게임완료시 이터레이터를 이용하여 플레이어 컨트롤러를 찾아 결과화면 출력
					for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
					{
						const auto ABPlayerController = Cast<AShooterCharacterPlayerController>(It->Get());
						if (nullptr != ABPlayerController)
						{
							ABPlayerController->ShowResultUI();
						}
					}
				}), DeadTimer, false);
			break;
		}
	}
}

ECharacterState AShooterBossCharacter::GetCharacterState() const//현재 캐릭터 상태 반환
{
	return CurrentState;
}

void AShooterBossCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	HPBarWidget->InitWidget();//위젯 초기화 필요(블루프린트 위젯 부모도 UShooterCharacterWidget로 변경 필요) 
	auto CharacterWidget = Cast<UShooterCharacterWidget>(HPBarWidget->GetUserWidgetObject());//UWidgetComponent를 UShooterCharacterWidget로 캐스팅
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);//위젯과 스탯 클래스를 바인딩
	}
}

float AShooterBossCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!ShooterAIController->IsAIRunning())//AI가 비가동일시 가동 
	{
		ShooterAIController->RunAI();//AI 가동
		ShooterAIController->bIsAIRunning = true;
	}
	
	HPBarWidget->SetHiddenInGame(false);//HP 위젯 보임

	GetWorld()->GetTimerManager().SetTimer(WidgetDeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
		HPBarWidget->SetHiddenInGame(true);//타이머 시간 뒤 HP 위젯 숨김
		}), WidgetDeadTimer, false);

	ShooterAIController->TrackToTargetOutRange(DamageCauser->GetActorLocation());//공격을 받으면 공격을 한 대상위치로 이동

	CharacterStat->SetDamage(FinalDamage);//피격시 데미지를 셋함

	return FinalDamage;
}

void AShooterBossCharacter::Fire()//총알 발사
{
	NewWeapon->PlayFireSound();//발사 사운드 실행
	NewWeapon->FirePaticle();//발사 파티클 실행
	NewWeapon->SingleEject();//발사시 탄피 이젝트 파티클실행

	auto TargetCharacter = Cast<AShooter2Character>(GetWorld()->GetFirstPlayerController()->GetPawn());//타깃임시생성
	
	//TargetCharacter->Controll

	FVector Start = NewWeapon->MuzzleScene->GetComponentLocation();//라인트레이스를 수행할 시작지점 (무기 머즐신)
	float RandX = FMath::FRandRange(-50.0f, 50.0f);
	float RandY = FMath::FRandRange(-50.0f, 50.0f);
	float RandZ = FMath::FRandRange(-50.0f, 50.0f);
	//라인 트레이스를 수행할 끝지점(해당 플레이어의 위치값)
	FVector End = FVector(TargetCharacter->GetActorLocation().X + RandX, TargetCharacter->GetActorLocation().Y + RandY, TargetCharacter->GetActorLocation().Z + 20.0f + RandZ);

	FCollisionObjectQueryParams ObjQuery;//공격 받는 인스턴스
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel1);// 공격받는 트레이스 채널 설정 추가 (캐릭터)
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel2);// 공격받는 트레이스 채널 설정 추가 (AI)
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel4);// 공격받는 트레이스 채널 설정 추가 (블록)
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel6);// 공격받는 트레이스 채널 설정 추가 (AIMissile)
	ObjQuery.AddObjectTypesToQuery(ECC_PhysicsBody);//피직스바디 추가

	FCollisionQueryParams ColQuery;//실행 주체 인스턴스
	ColQuery.AddIgnoredActor(this);//실행 주체 액터는 공격 무시

	FHitResult HitRes;//히트 확인 인스턴스

	bool Result = GetWorld()->LineTraceSingleByObjectType(HitRes, Start, End, ObjQuery, ColQuery);//라인 트레이스로 히트확인검사


	if (HitRes.bBlockingHit)//결과 인스턴스 유효할 시 
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, HitRes.ImpactPoint);//히트지점에 총알 피탄 파티클 생성
		UGameplayStatics::PlaySoundAtLocation(this, PitanSound, HitRes.ImpactPoint);//피탄 사운드실행

		FName CollisionName(TEXT("Ragdoll"));//래그돌 네임
		FName CollisionName2(TEXT("AIChara"));//AI캐릭터 네임
		FName CollisionName3(TEXT("ShooterChara"));//플레이어 캐릭터 네임

		if (CollisionName == HitRes.GetComponent()->GetCollisionProfileName() && HitRes.Actor.IsValid())//래그돌 네임과 충돌 결과가 같을시
		{
			// 플레이어와 타깃의 방향을 설정
			FVector ImpulseDirection = HitRes.GetActor()->GetActorLocation() - GetActorLocation();//밀어질 방향 설정(벡터 -연산)
			ImpulseDirection.Normalize();

			//충돌 세기
			float ImpulseStrength = 5000.0;

			FVector FinalImpulse = ImpulseDirection * ImpulseStrength;//임펄스 설정

			HitRes.GetComponent()->SetPhysicsLinearVelocity(FVector::ZeroVector);
			HitRes.GetComponent()->AddImpulseAtLocation(FinalImpulse, HitRes.GetActor()->GetActorLocation());//해당위치에서 임펄스 만큼 밀어냄
		}

		if (HitRes.Actor.IsValid())//유효한 액터는 데미지를 전달하고 내 위치를 상대 데미지 포인트에 지정
		{
			if (CollisionName2 == HitRes.GetComponent()->GetCollisionProfileName())//콜리전이 AI일때
			{
				auto HitAI = Cast<AShooterAICharacter>(HitRes.GetActor());
				HitAI->SetDamagePoint(GetActorLocation());//보스위치를 상대 액터 데미지 포인트에 지정
				
			}
			else if (CollisionName3 == HitRes.GetComponent()->GetCollisionProfileName())//콜리전이 캐릭터일때 일때
			{
				auto PlayerChara = Cast<AShooter2Character>(HitRes.GetActor());
				PlayerChara->SetDamagePoint(GetActorLocation());//보스위치를 상대 액터 데미지 포인트에 지정
			}
			FDamageEvent  DamageEvent;
			HitRes.Actor->TakeDamage(CharacterStat->GetAIBossShootDamage(), DamageEvent, GetController(), this);//데미지 세기,대미지 종류,공격명령 가해자,대미지전달 도구
		}
	}
	
}

void AShooterBossCharacter::CharacterDead()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));//콜리전 설정
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));//콜리전을 래그돌로설정
	GetMesh()->SetSimulatePhysics(true);//래그돌 활성화
	KockBack(70000.0f);
	ShooterAIController->StopAI();//AI정지
	HPBarWidget->SetHiddenInGame(true);//HP 위젯 숨김
}

void AShooterBossCharacter::Grenade()
{
	UGameplayStatics::PlaySoundAtLocation(this, MissileFireSound, GetActorLocation());//수류탄 발사소리 실행
	for (auto Grenade : BossGrenadeScene)//수류탄 신마다 수류탄 스폰
	{
		FVector SpawnLoc = Grenade->GetComponentLocation();
		FRotator SpawnRot = Grenade->GetComponentRotation();
		AShooterGrenade* pGrenade = GetWorld()->SpawnActor<AShooterGrenade>(AShooterGrenade::StaticClass(), SpawnLoc, SpawnRot);//수류탄 스폰
	}
}

void AShooterBossCharacter::Missile()//미사일 발사
{
	UGameplayStatics::PlaySoundAtLocation(this, MissileFireSound, GetActorLocation());//미사일 발사소리 실행
	MissileFireParticle->Activate();//미사일 발사 파티클 실행

	FVector MissileEjectLocation = BossMissileScene->GetComponentLocation();
	FRotator MissileEjectRotation = GetActorRotation();
	MissileEjectRotation.Pitch -= 90.0f;
	GetWorld()->SpawnActor<AShooterBossMissile>(AShooterBossMissile::StaticClass(), MissileEjectLocation, MissileEjectRotation);//미사일 스폰
}


// Called every frame
void AShooterBossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	auto PlayerCharacter = Cast<AShooter2Character>(GetWorld()->GetFirstPlayerController()->GetPawn());//타깃임시생성
	auto PlayerCharacterController = Cast<AShooterCharacterPlayerController>(PlayerCharacter->GetController());//타깃컨트롤러
	float Distance = PlayerCharacter->GetDistanceTo(this);

	PlayerCharacterController->GetHUDWidget()->GetDistanceToBoss(Distance);
	
}

// Called to bind functionality to input
void AShooterBossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}