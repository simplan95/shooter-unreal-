// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAICharacter.h"
#include "Components/CapsuleComponent.h"

#include "ShooterAIAnimInstance.h"//AI 애님 인스턴스
#include "ShooterGripGun.h"//장착 아이템 총
#include "DrawDebugHelpers.h"//디버그 그리기
#include "Runtime/Engine/Public/TimerManager.h"//타이머 매니저
#include "ShooterActorComponent.h"//캐릭터 스탯
#include "Components/WidgetComponent.h"//UI위젯 
#include "ShooterCharacterWidget.h"//캐릭터 위젯
#include "ShooterAIController.h"//AI 컨트롤러 
#include "ShooterBossCharacter.h"//AI 보스
#include "Shooter2GameMode.h"//게임모드

// Sets default values
AShooterAICharacter::AShooterAICharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	DeadTimer = 30.0f;//데드 타이머(AI 사망시)
	WidgetDeadTimer = 2.0f;//위젯 데드 타이머(위젯 지속시간)
	StandSpeed = 260.0f;//걷기속도
	StandRunSpeed = 400.0f;//달리기속도

	AIControllerClass = AShooterAIController::StaticClass();//AI Control 클래스를 지정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;//스폰되거나 월드에 있을 때 자동으로 AI컨트롤러로 빙의

	// 충돌 캡슐의 크기 설정
	GetCapsuleComponent()->InitCapsuleSize(42.f, 90.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("AIChara"));//콜리전 설정

	// 캐릭터 움직임 구성(회전을 부드럽게)
	bUseControllerRotationYaw = false;//컨트롤러의 회전에 따라 Z축 회전을 끔
	GetCharacterMovement()->bUseControllerDesiredRotation = true;//부드러운 회전
	GetCharacterMovement()->bOrientRotationToMovement = true; // 캐릭터가 입력 방향으로 회전합니다	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // 이 회전 속도로
	GetCharacterMovement()->JumpZVelocity = 450.0f;//점프높이
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->bCanWalkOffLedges = true;//앉았을 때 절벽에서 떨어지기 가능

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -87.0f), FRotator(0.0f, 270.0f, 0.0f));//폰 기본위치 설정
	GetCharacterMovement()->MaxWalkSpeed = StandSpeed;//캐릭터 스피드 지정

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_Body(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));//캐릭터 스킨
	if (SK_Body.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Body.Object);//캐릭터 스킨추가
	}

	//애니메이션 추가
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> myAnim(TEXT("/Game/Mannequin/Animations/AI/ThirdPerson_AnimBP_AI.ThirdPerson_AnimBP_AI_C"));
	if (myAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(myAnim.Class);
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));//메시는 콜리전 비활성
	}

	ImpactParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("ImpactParticle"));//총알 임팩트 파티클
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_Impact(TEXT("/Game/MyAsset/PS_Impact.PS_Impact"));
	if (PT_Impact.Succeeded())
	{
		ImpactParticle =PT_Impact.Object;
	}

	PitanSound = CreateDefaultSubobject<USoundBase>(TEXT("Pitan"));//피탄 사운드
	static ConstructorHelpers::FObjectFinder<USoundBase>SB_PitanSound(TEXT("/Game/MyAsset/SC_Pitan.SC_Pitan"));
	if (SB_PitanSound.Succeeded())
	{
		PitanSound = SB_PitanSound.Object;
	}

	DetectScene = CreateDefaultSubobject<USceneComponent>(TEXT("DetecteScene"));//AI가 디텍팅할 지점 신
	DetectScene->SetupAttachment(RootComponent);//루트 컴포넌트에 부착
	DetectScene->SetRelativeLocation(FVector(0.0f, 0.0f, 75.0f));//신 위치 조정
	DetectScene->SetRelativeRotation(GetActorRotation());

	CharacterStat->SetCurrentHP(100.0f);//스탯을 지정
	CharacterStat->SetMaxHP(100.0f);//스탯 지정

	HPBarWidget->SetupAttachment(GetMesh());//위젯을 메쉬에 부착

	SetActorHiddenInGame(true);//액터 숨김
	HPBarWidget->SetHiddenInGame(true);//HP 위젯 숨김
}

// Called when the game starts or when spawned
void AShooterAICharacter::BeginPlay()
{
	Super::BeginPlay();

	ShooterAIController = Cast<AShooterAIController>(GetController());
	ShooterAIController->RunAI();//AI 가동

	//총생성 및 무기부착
	FName WeaponSocket(TEXT("my_hand_rSocket"));
	NewWeapon = GetWorld()->SpawnActor<AShooterGripGun>(FVector::ZeroVector, FRotator::ZeroRotator);//총생성
	NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	NewWeapon->SetOwner(this);

	CharacterStat->SetMagazineCount(30);//초기 탄창 총알수 설정

	SetCharacterState(ECharacterState::READY);
}

void AShooterAICharacter::SetCharacterState(ECharacterState NewState)
{
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ECharacterState::READY:
	{
		SetActorHiddenInGame(false);//액터 보임
		HPBarWidget->SetHiddenInGame(true);//HP 위젯 안보임

		CharacterStat->OnHPIsZero.AddLambda([this]()->void {//HP 0시 델리게이트를 사용하여 DEAD 스테이트로 설정
			SetCharacterState(ECharacterState::DEAD);
			});
		break;
	}
	case ECharacterState::DEAD:
	{
		CharacterDead();
		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
			Destroy();
			}), DeadTimer, false);
		break;
	}
	}
}

ECharacterState AShooterAICharacter::GetCharacterState() const
{
	return CurrentState;
}

void AShooterAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ShooterAnim = Cast<UShooterAIAnimInstance>(GetMesh()->GetAnimInstance());//애님 인스턴스 생성

	ShooterAnim->OnRestrictStart.AddUObject(this, &AShooterAICharacter::FCannotAttack);//노피파이 확인시 공격가능
	ShooterAnim->OnRestrictEnd.AddUObject(this, &AShooterAICharacter::FCanAttack);//노피파이 확인시 공격불가


	HPBarWidget->InitWidget();//위젯 초기화 필요(블루프린트 위젯 부모도 UShooterCharacterWidget로 변경 필요) 
	auto CharacterWidget = Cast<UShooterCharacterWidget>(HPBarWidget->GetUserWidgetObject());//위젯 호출
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}
}

void AShooterAICharacter::CharacterDead()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));//콜리전 설정
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));//콜리전을 래그돌로설정
	GetMesh()->SetSimulatePhysics(true);//래그돌 활성화
	HPBarWidget->SetHiddenInGame(true);//HP 위젯 숨김
	KockBack(2000.0f);
	NewWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);//총떼기
	NewWeapon->SetActorEnableCollision(false);//콜리전 비활성화
	NewWeapon->Destroy();//총파괴
	NewWeapon->Gun->SetHiddenInGame(true, true);//총 제거, USkeletalMeshComponent에서만 사용가능
	GetWorld()->SpawnActor<AShooterGun>(GetActorLocation(), FRotator::ZeroRotator);//플레이어가 줍게될 총생성
	ShooterAIController->StopAI();//AI정지

	auto PlayerController = Cast<AShooterCharacterPlayerController>(this->LastHitBy);//본인을 마지막으로 때린 컨트롤러를 반환
	auto GameMode = Cast<AShooter2GameMode>(GetWorld()->GetAuthGameMode());//게임모드를 가져옴
	GameMode->AddScore(PlayerController);//해당 플레이어 점수 추가
}

void AShooterAICharacter::Run()//타깃 확인시 달리기속도로
{
	GetCharacterMovement()->MaxWalkSpeed = StandRunSpeed;
	ShooterAnim->SetIsSprinting(true);
	
}

void AShooterAICharacter::Walk()//타깃 미확인시 걷기속도
{
	GetCharacterMovement()->MaxWalkSpeed = StandSpeed;
	ShooterAnim->SetIsSprinting(false);
}

void AShooterAICharacter::Fire()//AI 총 발사
{
	CharacterStat->SetMagazineCount(-1);//탄창에서 한발 빠짐

	if (CharacterStat->GetMagazineBullet()>0)
	{
		NewWeapon->PlayFireSound();
		NewWeapon->FirePaticle();
		NewWeapon->SingleEject();

		auto TargetCharacter = Cast<AShooter2Character>(GetWorld()->GetFirstPlayerController()->GetPawn());//타깃임시생성

		FVector Start = NewWeapon->MuzzleScene->GetComponentLocation();//라인트레이스 시작지점
		float RandX = FMath::FRandRange(-750.0f, 750.0f);
		float RandY = FMath::FRandRange(-750.0f, 750.0f);
		float RandZ = FMath::FRandRange(-750.0f, 750.0f);
		//라이트레이스 끝지점
		FVector End = Start + NewWeapon->MuzzleScene->GetComponentRotation().Vector()*10000.0f;//라인트레이스 끝점 
		FVector End2= FVector(End.X + RandX, End.Y + RandY, End.Z + RandZ);


		FCollisionObjectQueryParams ObjQuery;//공격 받는 인스턴스
		ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel1);// 공격받는 트레이스 채널 설정 추가 (캐릭터)
		ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel2);// 공격받는 트레이스 채널 설정 추가 (AI)
		ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel4);// 공격받는 트레이스 채널 설정 추가 (블록)
		ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel6);// 공격받는 트레이스 채널 설정 추가 (AIMissile)
		ObjQuery.AddObjectTypesToQuery(ECC_PhysicsBody);//피직스바디 추가

		FCollisionQueryParams ColQuery;//실행 주체 인스턴스
		ColQuery.AddIgnoredActor(this);//실행 주체 액터는 공격 무시

		FHitResult HitRes;//히트 확인 인스턴스

		bool Result = GetWorld()->LineTraceSingleByObjectType(HitRes, Start, End2, ObjQuery, ColQuery);//라인 트레이스로 히트확인검사

		if (HitRes.bBlockingHit)//결과 인스턴스 유효할 시 
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, HitRes.ImpactPoint);//히트지점에 총알 피탄 파티클 생성
			UGameplayStatics::PlaySoundAtLocation(this, PitanSound, HitRes.ImpactPoint);//피탄 사운드 재생

			FName CollisionName(TEXT("Ragdoll"));//래그돌 네임
			FName CollisionName2(TEXT("AIChara"));//AI캐릭터 네임
			FName CollisionName3(TEXT("ShooterChara"));//AI캐릭터 네임

			if (HitRes.Actor.IsValid())
			{
				if (CollisionName == HitRes.GetComponent()->GetCollisionProfileName() && HitRes.Actor.IsValid())//래그돌 네임과 충돌 결과가 같을시
				{
					// 플레이어와 타깃의 방향을 설정
					FVector ImpulseDirection = HitRes.GetActor()->GetActorLocation() - GetActorLocation();//밀어질 방향 설정
					ImpulseDirection.Normalize();

					//충돌 세기
					float ImpulseStrength = 5000.0;

					FVector FinalImpulse = ImpulseDirection * ImpulseStrength;//임펄스 설정

					HitRes.GetComponent()->SetPhysicsLinearVelocity(FVector::ZeroVector);
					HitRes.GetComponent()->AddImpulseAtLocation(FinalImpulse, HitRes.GetActor()->GetActorLocation());//해당위치에서 임펄스 만큼 밀어냄
				}

				else if (CollisionName2 == HitRes.GetComponent()->GetCollisionProfileName())//콜리전이 AI일때
				{
					auto HitAI = Cast<AShooterEnemyCharacter>(HitRes.GetActor());
					HitAI->SetDamagePoint(GetActorLocation());//현재 공격자의 위치를 상대 액터 데미지 포인트에 지정
				}
				else if (CollisionName3 == HitRes.GetComponent()->GetCollisionProfileName())//콜리전이 캐릭터일때 일때
				{
					auto PlayerChara = Cast<AShooter2Character>(HitRes.GetActor());
					PlayerChara->SetDamagePoint(GetActorLocation());//현재 공격자의 위치를 상대 액터 데미지 포인트에 지정
				}

				FDamageEvent  DamageEvent;

				//데미지 세기,대미지 종류,공격명령 가해자,대미지전달 도구
				HitRes.Actor->TakeDamage(CharacterStat->GetAIShootDamage(), DamageEvent, GetController(), this);
			}
		}
	}
	else if(CharacterStat->GetMagazineBullet() <= 0)//총알 없을 시 리로드
	{
		Reload();
	}
}

void AShooterAICharacter::Reload()
{
	ShooterAnim->PlayReloadMontage();//재장전 애니메이션 재생
	NewWeapon->PlayReloadSound();//재장전 소리 재생
	CharacterStat->SetMagazineCount(30);//탄창 총알 수 추가
}

void AShooterAICharacter::FCanAttack()
{
	CanAttack = true;
}

void AShooterAICharacter::FCannotAttack()
{
	CanAttack = false;
}

//데미지를 받았을 때
float AShooterAICharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	HPBarWidget->SetHiddenInGame(false);//HP 위젯 보임

	GetWorld()->GetTimerManager().SetTimer(WidgetDeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
		HPBarWidget->SetHiddenInGame(true);//타이머 시간 뒤 HP 위젯 숨김
		}), WidgetDeadTimer, false);

	ShooterAnim->PlayHitReactMontage();//피격 몽타주 실행
	
	ShooterAIController->TrackToTargetOutRange(DamageCauser->GetActorLocation());//피격시 AI 컨트롤러가 공격주체의 위치값을 가져와 그곳으로 이동

	CharacterStat->SetDamage(FinalDamage);//피격시 데미지를 셋함
	return FinalDamage;
}

bool AShooterAICharacter::GetCanAttack() const
{
	return CanAttack;
}

UShooterAIAnimInstance* AShooterAICharacter::GetAIAnim() const
{
	return ShooterAnim;
}

FVector AShooterAICharacter::GetDtectPoint() const//탐지를 시작할곳 반환
{
	return DetectScene->GetComponentLocation();
}

// Called every frame
void AShooterAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}