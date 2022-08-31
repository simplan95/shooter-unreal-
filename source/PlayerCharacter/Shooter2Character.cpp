// Copyright Epic Games, Inc. All Rights Reserved.

#include "Shooter2Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "ShooterAnimInstance.h"//애님 인스턴스
#include "ShooterGun.h"//필드 아이템 총
#include "ShooterGripGun.h"//장착 아이템 총
#include "DrawDebugHelpers.h"//디버그 
#include "Runtime/Engine/Public/TimerManager.h"//타이머 매니저
#include "FireCameraShake.h"//카메라 쉐이크
#include "ShooterCharacterPlayerController.h"//플레이어 컨트롤러
#include "ShooterActorComponent.h"//캐릭터 스탯
#include "Components/WidgetComponent.h"//UI위젯 
#include "ShooterCharacterWidget.h"//내 캐릭터 위젯
#include "ShooterGrenade.h"//수류탄 발사체
#include "ShooterGripGrenade.h"//수류탄 아이템
#include "ShooterHUDWidget.h"//HUD
#include "SupplyBox.h"//서플라이박스

#include "ShooterEnemyCharacter.h"//에너미 origin 캐릭터
#include "ShooterAIEnemyController.h"//에너미 origin 컨트롤러

//////////////////////////////////////////////////////////////////////////
// AShooter2Character

AShooter2Character::AShooter2Character()
{
	
	PrimaryActorTick.bCanEverTick = true;

	// 충돌 캡슐의 크기 설정
	GetCapsuleComponent()->InitCapsuleSize(37.f, 90.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ShooterChara"));//콜리전 설정

	// 입력에 대한 회전율 설정
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// 컨트롤러가 회전 할 때 캐릭터 회전 안함 카메라에만 영향을 미칩니다.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 캐릭터 움직임 구성
	GetCharacterMovement()->bOrientRotationToMovement = false; // 캐릭터가 입력 방향으로 회전합니다	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // 이 회전 속도로
	GetCharacterMovement()->JumpZVelocity = 450.0f;//점프높이
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;// 캐릭터 앉기 기능 활성화
	GetCharacterMovement()->bCanWalkOffLedges = true;//앉았을 때 절벽에서 떨어지기 가능


	// 카메라 붐을 만듭니다 (충돌이있는 경우 플레이어를 향해 당김)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);//루트 컴포넌트에 부착
	CameraBoom->TargetArmLength = 200.0f; //카메라는 캐릭터 뒤에서이 거리를 따라갑니다.	
	CameraBoom->bUsePawnControlRotation = true; // 컨트롤러를 기준으로 팔 회전
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));//카메라 붐 위치

	// 팔로우 카메라 만들기
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // 카메라를 붐 끝에 부착하고 컨트롤러 방향에 맞게 붐을 조정합니다.
	FollowCamera->bUsePawnControlRotation = false; // 카메라가 팔을 기준으로 회전하지 않습니다.
	FollowCamera->SetRelativeLocation(FVector(0.0f, CameraLeftRight, 10.0f));//카메라 위치

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -87.0f), FRotator(0.0f, 270.0f, 0.0f));//폰 기본위치 설정
	GetCharacterMovement()->CrouchedHalfHeight = 55.0f;//앉았을 때 캡슐컴포높이
	GetCharacterMovement()->MaxWalkSpeed = StandSpeed;//캐릭터 스피드 지정

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_Body(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));//캐릭터 스킨
	if (SK_Body.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Body.Object);//캐릭터 스킨추가
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));//메시는 콜리전 비활성
	}

	//애니메이션 추가
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> myAnim(TEXT("/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C"));
	//애님인스턴스 추가 블루프린트 class 불러오기 뒤에 _C를 붙여 클래스정보를 가져오는 경로를 생성
	if (myAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(myAnim.Class);
	}
	
	ImpactParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("ImpactParticle"));//피탄 임팩트 파티클
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

	MissSound = CreateDefaultSubobject<USoundBase>(TEXT("MIss"));//근접공격 미스 사운드
	static ConstructorHelpers::FObjectFinder<USoundBase>SB_MissSound(TEXT("/Game/MyAsset/SC_Punch_Miss.SC_Punch_Miss"));
	if (SB_MissSound.Succeeded())
	{
		MissSound = SB_MissSound.Object;
	}

	HitSound = CreateDefaultSubobject<USoundBase>(TEXT("Hit"));//근접 공격 히트 사운드
	static ConstructorHelpers::FObjectFinder<USoundBase>SB_HitSound(TEXT("/Game/MyAsset/SC_Punch_Hit.SC_Punch_Hit"));
	if (SB_HitSound.Succeeded())
	{
		HitSound = SB_HitSound.Object;
	}

	CharacterStat = CreateDefaultSubobject<UShooterActorComponent>(TEXT("CharacterStat"));//캐릭터 스탯 클래스
	CharacterStat->SetCurrentHP(1000.0f);//스탯을 지정
	CharacterStat->SetMaxHP(1000.0f);//스탯 지정

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));//위젯 선언
	HPBarWidget->SetupAttachment(GetMesh());//위젯을 메쉬에 부착
	HPBarWidget->SetRelativeLocation(FVector(0.0f,0.0f,190.f));//위젯 위치 설정
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);//위젯을 스크린에 고정
	static ConstructorHelpers::FClassFinder<UUserWidget>WG_HPBar(TEXT("/Game/MyAsset/WG_HPBar.WG_HPBar_C"));
	if(WG_HPBar.Succeeded())
	{
		HPBarWidget->SetWidgetClass(WG_HPBar.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f,50.0f));
	}

	GrenadeScene = CreateDefaultSubobject<USceneComponent>(TEXT("GrenadeScene"));//수류탄이 던져질 시작점 신
	GrenadeScene->SetupAttachment(RootComponent);//루트 컴포넌트에 부착
	GrenadeScene->SetRelativeLocation(FVector(50.0f,0.0f,80.0f));//신 위치 조정

	DetectedScene = CreateDefaultSubobject<USceneComponent>(TEXT("DetecteScene"));//AI가 디텍팅할 지점 신
	DetectedScene->SetupAttachment(RootComponent);//루트 컴포넌트에 부착
	DetectedScene->SetRelativeLocation(FVector(0.0f, 0.0f, 75.0f));//신 위치 조정

	VisibleGrenade = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GREANDE"));//수류탄 스켈레탈 메시 초기화(수류탄을 던질떄 비주얼적으로만 나타남)
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_GRENADE(TEXT("/Game/GrenadeKit/Meshes/MK2_Grenade_SK.MK2_Grenade_SK"));//수류탄 메시
	if (SK_GRENADE.Succeeded())
	{
		VisibleGrenade->SetSkeletalMesh(SK_GRENADE.Object);
		VisibleGrenade->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5));//수류탄 크기조정
		VisibleGrenade->SetCollisionProfileName(TEXT("NoCollision"));//콜리전 설정
		VisibleGrenade->SetHiddenInGame(true, true);//수류탄 숨김
	}
	FName HandSocket(TEXT("my_hand_rSocket"));
	VisibleGrenade->SetupAttachment(GetMesh(), HandSocket);//수류탄을 오른손 소켓에 부착


	SetActorHiddenInGame(true);//액터 숨김
	HPBarWidget->SetHiddenInGame(true);//HP 위젯 숨김
	DeadTimer = 3.0f;//데드타이머 3초
}

void AShooter2Character::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsPlayerControlled())//플레이어가 컨트롤할 때
	{
		ShooterPlayerController = Cast < AShooterCharacterPlayerController >(GetController());//플레이어 컨트롤러 설정
	}
	SetCharacterState(ECharacterState::READY);//플레이어 캐릭터 상태 준비완료
}

void AShooter2Character::SetCharacterState(ECharacterState NewState)//캐릭터 상태 설정
{
	CurrentState = NewState;

	switch (CurrentState)
	{
		case ECharacterState::READY:
		{
			SetActorHiddenInGame(false);//액터 보임
			HPBarWidget->SetHiddenInGame(false);//HP 위젯 보임

			//캐릭터 HUD와 스탯을 바인딩(스탯은 캐릭터클래스에서 플레이어스테이트는 컨트롤러에서)
			ShooterPlayerController->GetHUDWidget()->BindCharacterStat(CharacterStat);

			CharacterStat->OnHPIsZero.AddLambda([this]()->void {//HP 0시 델리게이트를 사용하여 DEAD 스테이트로 설정
				SetCharacterState(ECharacterState::DEAD);
				});
			break;
		}
		case ECharacterState::DEAD:
		{
			CharacterDead();
			GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
				ShooterPlayerController->ShowResultUI();//결과 UI활성화
				}), DeadTimer, false);
			break;
		}
	}
}

ECharacterState AShooter2Character::GetCharacterState() const//현재 캐릭터 상태 반환
{
	return CurrentState;
}

void AShooter2Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AShooter2Character::MyJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AShooter2Character::MyJumpStop);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AShooter2Character::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AShooter2Character::Walk);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AShooter2Character::DoCrouch);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AShooter2Character::Aim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AShooter2Character::Aimoff);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AShooter2Character::Attack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AShooter2Character::AttackEnd);
	PlayerInputComponent->BindAction("Gun", IE_Pressed, this, &AShooter2Character::GunGrap);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AShooter2Character::Reload);
	PlayerInputComponent->BindAction("ModeChange", IE_Pressed, this, &AShooter2Character::WeaponModeChange);
	PlayerInputComponent->BindAction("Grenade", IE_Pressed, this, &AShooter2Character::ThrowGrena);
	PlayerInputComponent->BindAction("AimChange", IE_Pressed, this, &AShooter2Character::AimChange);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AShooter2Character::InterAct);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooter2Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooter2Character::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AShooter2Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CameraLeftRight = FMath::FInterpTo(CameraLeftRight, NewCameraLeftRight, DeltaTime, 5.0f);//카메라를 부드럽게 전환
	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, CameraBoomLenth, DeltaTime, 5.0f);//카메라붐을 부드럽게 전환
	FollowCamera->SetRelativeLocation(FVector(0.0f, CameraLeftRight, 0.0f));
	
	//총 미사용 && 공격 미상태 || 달리는 중 && 에임 미상태
	if (!ShooterAnim->IsGunUsed() && !bIsAttacking || ShooterAnim->IsSprinting() && !ShooterAnim->IsAiming())
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;//이동방향으로의 캐릭터회전 on
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;//이동방향으로의 캐릭터회전 off
		//현재 캐릭터 로테이션을 타깃 컨트롤러 로테이션으로(Yaw축만)
		FRotator Rotation = FMath::RInterpTo(GetActorRotation(), GetControlRotation(), DeltaTime, 25.0f);
		SetActorRotation(FRotator(0.0f, Rotation.Yaw, 0.0f));
	}
}

void AShooter2Character::MyJump()
{
	Jump();
	if (ShooterAnim->MyIsCrouching() == true)//앉았을 때 점프시 앉기 해제
	{
		ShooterAnim->SetCrouching(false);
		UnCrouch();
	}
}

void AShooter2Character::MyJumpStop()
{
	StopJumping();
}

void AShooter2Character::Run()//달리기
{
	if (ShooterAnim->IsAiming() == false)//에임이 아닐경우에 달리기 속도로
	{
		GetCharacterMovement()->MaxWalkSpeed = StandRunSpeed;
		GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
	}
	ShooterAnim->SetSprinting(true);//스프린팅 활성화
}
void AShooter2Character::Walk()//걷기
{
	GetCharacterMovement()->MaxWalkSpeed = StandSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
	ShooterAnim->SetSprinting(false);//스프린팅 비활성화
}
void AShooter2Character::DoCrouch()
{

	if (CanCrouch() && !GetCharacterMovement()->IsFalling())//떨어지지않을때 앉기가능
	{
		Crouch();
		CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));//카메라 붐 위치
		DetectedScene->SetRelativeLocation(FVector(0.0f, 0.0f, 45.0f));//Ai 디텍팅 신 위치조정
	}
	else
	{
		UnCrouch();
		CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));//카메라 붐 위치
		DetectedScene->SetRelativeLocation(FVector(0.0f, 0.0f, 75.0f)); //Ai 디텍팅 신 위치조정
	}

}

void AShooter2Character::Aim()//에임 온
{
	CameraBoomLenth = 50.0f;//카메라 붐 길이설정
	if (ShooterAnim->IsAiming() == false && ShooterAnim->IsGunUsed() == true)//에임중 속도 제어
	{
		ShooterAnim->SetAiming(true);
		GetCharacterMovement()->MaxWalkSpeed = StandSpeed;
	}
}
void AShooter2Character::Aimoff()//에임 오프
{
	CameraBoomLenth = 200.0f;//카메라 붐길이 설정
	if (ShooterAnim->IsAiming() == true )//에임중 속도 제어
	{
		ShooterAnim->SetAiming(false);
		if (ShooterAnim->IsSprinting() == true)//달리기를 누르는 중에 에임이 꺼지면 달리기 속도로
		{
			GetCharacterMovement()->MaxWalkSpeed = StandRunSpeed;
		}
	}
}

void AShooter2Character::AimChange()//에임 좌우 설정 스왑
{
	NewCameraLeftRight *= -1.0f;
}

void AShooter2Character::Reload()//재장전 몽타주 실행
{
	if (ShooterAnim->IsGunUsed() == true && !ShooterAnim->UAnimInstance::IsAnyMontagePlaying() && CharacterStat->GetMagazineBullet() <30)
	{
		ShooterAnim->PlayReloadMontage();//재장전 몽타주 실행
		CurrentGun->PlayReloadSound();//재장전 사운드 실행

		if (CharacterStat->GetMagazineBullet() > 0)//탄창이 총알이 들어있을 때
		{
			int32 NeedToAmmo = 31 - CharacterStat->GetMagazineBullet();

			if (CharacterStat->GetMaxBullet() >= NeedToAmmo && CharacterStat->GetMaxBullet() > 0)//소지총알이 장전에 필요한 총알수보다 많은 경우
			{
				CharacterStat->SetMagazineCount(NeedToAmmo);//필요한 총알 수만큼 탄창에 추가
				CharacterStat->SetMaxBullet(-NeedToAmmo);//필요한 총알 수만큼 소지총알 감소
			}
			else if (CharacterStat->GetMaxBullet() < NeedToAmmo && CharacterStat->GetMaxBullet()>0)//소지총알이 장전에 필요한 총알수보다 적은 경우
			{
				CharacterStat->SetMagazineCount(CharacterStat->GetMaxBullet());//남아있는 소지총알수 만큼 추가
				CharacterStat->SetMaxBulletQuantity(0);//소지총알 수는 0으로
			}
		}
		else if (CharacterStat->GetMagazineBullet() <= 0)//탄장에 총알이 비어 있을 때
		{
			int32 NeedToAmmo = 30;
			if (CharacterStat->GetMaxBullet() >= NeedToAmmo && CharacterStat->GetMaxBullet() > 0)//소지총알이 장전에 필요한 총알수보다 많은 경우
			{
				CharacterStat->SetMagazineCount(NeedToAmmo);//필요한 총알 수만큼 탄창에 추가
				CharacterStat->SetMaxBullet(-NeedToAmmo);//필요한 총알 수만큼 소지총알 감소
			}
			else if (CharacterStat->GetMaxBullet() < NeedToAmmo && CharacterStat->GetMaxBullet() >0)//소지총알이 장전에 필요한 총알수보다 적은 경우
			{
				CharacterStat->SetMagazineCount(CharacterStat->GetMaxBullet());//남아있는 소지총알수 만큼 추가
				CharacterStat->SetMaxBulletQuantity(0);//소지총알 수는 0으로
			}
		}
		CharacterStat->OnAmmoCountChaned.Broadcast();//총알 수 변동시 호출
	}
}

void AShooter2Character::InterAct()//서플라이 박스와 상호작용
{
	if (bCanSupply && SupplyBox)
	{
		if (SupplyBox->GetEnableSupply())
		{
			int32 NeedAmmo = 150 - CharacterStat->GetMaxBullet();
			int32 NeedGrenade = 5 - CharacterStat->GetGrenadeCount();

			CharacterStat->SetMaxBullet(NeedAmmo);//총알 최대로
			CharacterStat->SetGrenadeCount(NeedGrenade);//수류탄수 최대로
			CharacterStat->SetHP(CharacterStat->GetMaxHP());//max 체력 채우기
			SupplyBox->InitCooltimeCount();//보급박스 초기화
			SupplyBox->SupplySoundPlay();//보급사운드 재생
		}
	}
}

void AShooter2Character::Attack()//공격 몽타주 실행
{
	bIsAttacking = true;//공격중 활성화
	
	//총 사용중 && 몽타주가 실행 중이지 않을 때
	if (ShooterAnim->IsGunUsed() == true && !ShooterAnim->UAnimInstance::IsAnyMontagePlaying())
	{
		Fire();//총 발사
	}
	else
	{
		ShooterAnim->PlayPunchMontage();//펀치 몽타주 실행
	}

}

void AShooter2Character::AttackEnd()//공격이 끝났을 때
{
	GetWorld()->GetTimerManager().ClearTimer(FireTimer);//FireTimer 종료
	bIsAttacking = false;//공격중 비활성화
}

void AShooter2Character::Fire()//총 발사
{
	if (CharacterStat->GetMagazineBullet() > 0)
	{
		if (ShooterAnim->IsSprinting() == true)//달리기 중일 때 무기 발사시 걷기속도로 
		{
			Walk();
		}

		CharacterStat->SetMagazineCount(-1);//탄창에서 한발 빠지면서 브로드캐스트
		CurrentGun->PlayFireSound();//총 발사 소리 재생
		CurrentGun->FirePaticle();//총구 발사 파티클 재생
		CurrentGun->SingleEject();//발사시 이젝트 파티클

		//카메라 반동
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(UFireCameraShake::StaticClass(), 1.f, ECameraAnimPlaySpace::CameraLocal);
	
		//뷰포트를 기준으로 라인트레이스의 시작과 끝을 가져옴
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			PlayerViewPointLocation,
			PlayerViewPointRotation
		);
		FVector End = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * 10000.0f;//라인트레이스 끝점 
		FVector Start= PlayerViewPointLocation + PlayerViewPointRotation.Vector() * CameraBoomLenth;//라인트레이스 시작점

		//////////////////////////////////////////////////////////////// 총 발사 및 데미지 구현
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

		if (Result)//결과 인스턴스 유효할 시 
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, HitRes.ImpactPoint);//히트지점에 총알 피탄 파티클 생성
			UGameplayStatics::PlaySoundAtLocation(this, PitanSound, HitRes.ImpactPoint);//피탄 소리재생

			FName CollisionName(TEXT("Ragdoll"));//래그돌 
			FName CollisionName2(TEXT("AIChara"));//AI캐릭터 
			
			if (HitRes.Actor.IsValid())//약포인터 방식으로 메모리를 참조해 메모리 낭비 방지
			{
				if (CollisionName == HitRes.GetComponent()->GetCollisionProfileName())//래그돌 네임과 충돌 결과가 같을시
				{
					// 플레이어와 타깃의 방향을 설정
					FVector ImpulseDirection = HitRes.GetActor()->GetActorLocation() - GetActorLocation();//밀어질 방향 설정(벡터 -연산)
					ImpulseDirection.Normalize();

					float ImpulseStrength = 5000.0;//충돌 세기
					FVector FinalImpulse = ImpulseDirection * ImpulseStrength;//임펄스 설정

					HitRes.GetComponent()->SetPhysicsLinearVelocity(FVector::ZeroVector);
					HitRes.GetComponent()->AddImpulseAtLocation(FinalImpulse, HitRes.GetActor()->GetActorLocation());//해당위치에서 임펄스 만큼 밀어냄
				}

				else if (CollisionName2 == HitRes.GetComponent()->GetCollisionProfileName())//콜리전이 AI일때
				{
					auto HitAI = Cast<AShooterEnemyCharacter>(HitRes.GetActor());
					HitAI->SetDamagePoint(GetActorLocation());//내위치를 상대 액터 데미지 포인트에 지정
				}
				FDamageEvent  DamageEvent;

				//데미지 세기,대미지 종류,공격명령 가해자,대미지전달 도구
				HitRes.Actor->TakeDamage(CharacterStat->GetShootDamage(), DamageEvent, GetController(), this);
			}
		}
		//////////////////////////////////////////////////////////////// 총 발사 및 데미지 구현

		//////////////////////////////////////////////////////////////// 반동구현
		float RandPitch = FMath::RandRange(-1.5f, 0.2f);//수직반동
		float RandYaw = FMath::RandRange(-0.3f, 0.3f);//수평반동
		AddControllerPitchInput(RandPitch);//수직반동
		AddControllerYawInput(RandYaw);//수평반동	
		////////////////////////////////////////////////////////////////반동구현

		////////////////////////////////////////////////////////////////적 어그로
		//총 발사시 일정범위 안에있는 적들을 모음
		UWorld* World = this->GetWorld();
		FVector Center = this->GetActorLocation();
		float DetectRadius = 2000.0f;//탐지범위
		if (nullptr == World) return;

		TArray<FOverlapResult> OverLapResults;//TArray 자료구조로 오버랩 결과 인스턴스를 저장.
		FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);//콜리전 선언

		//오브젝트 타입을 멀티 방식으로 탐지(오브젝트 방식으로 탐지 : 탐지의 대상이 되는 콜리전들을 검사후 반환)
		bool AgroResult = World->OverlapMultiByObjectType(
			OverLapResults,
			Center,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel2,//탐색 감지에 사용할 트레이스 채널 정보
			FCollisionShape::MakeSphere(DetectRadius),//탐색에 사용할 기본 도형 정보, 구체, 캡슐, 박스를 사용
			CollisionQueryParam//콜리전 선언
		);

		if (AgroResult)//AI판단 및 어그로 활성화
		{
			for (auto OverLapResult : OverLapResults)
			{
				if (OverLapResult.GetActor()->GetClass()->IsChildOf(AShooterEnemyCharacter::StaticClass()))//AI캐릭터일 경우 TrackToTargetOutRange호출
				{
					auto ResultAI = Cast<AShooterEnemyCharacter>(OverLapResult.GetActor());
					auto AIController = Cast<AShooterAIEnemyController>(ResultAI->GetController());
					AIController->TrackToTargetOutRange(GetActorLocation());
				}
			}
		}
		else
		{
			//DoNothing
		}
		////////////////////////////////////////////////////////////////적 어그로

		if (!CurrentGun->GetMode() && bIsAttacking)//연사모드일 시 타이머를 이용한 재귀호출을 하여 연사 구현
		{
			GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &AShooter2Character::Fire, 0.1f, false);
		}
		
	}
	else if (CharacterStat->GetMagazineBullet() <= 0 && CharacterStat->GetMaxBullet() > 0)
	{
		Reload();//빈탄창 리로드
	}
	else if (CharacterStat->GetMagazineBullet() <= 0 && CharacterStat->GetMaxBullet() <= 0)
	{
		CurrentGun->ModeChangeSound->Play();//총알 부족시 빈총 소리
	}
}

void AShooter2Character::MeleeAttack()//근접공격
{
	float AttackRange = 50.0f;//근접공격 길이
	float AttackRadius = 50.0f;//근접공격 범위

	FHitResult HitRes;//물리적 충돌이 탐지된 경우 관련된 정보를 담는 구조체
	FCollisionQueryParams Params(NAME_None, false, this);//콜리전 선언

	bool bResult = GetWorld()->SweepSingleByChannel( //공격 결과를 담음 (채널 방식으로구현 : 해당 트레이스 채널로 검사를 실행후 결과를 반환)
		HitRes,//물리적 충돌이 탐지된 경우 관련된 정보를 담는 구조체
		GetActorLocation(),//탐색을 시작할 위치
		GetActorLocation() + GetActorForwardVector() * AttackRange,//탐색을 끝낼 위치
		FQuat::Identity,//탐색에 사용할 도형의 회전
		ECollisionChannel::ECC_GameTraceChannel5,//물리충돌 감지에 사용할 트레이스 채널 정보
		FCollisionShape::MakeSphere(AttackRadius),//탐색에 사용할 기본 도형 정보, 구체, 캡슐, 박스를 사용
		Params);//탐색방법에 대한 설정값을 모아둔 구조체

	if (bResult)
	{
		FName CollisionName(TEXT("Ragdoll"));//래그돌 
		FName CollisionName2(TEXT("AIChara"));//AI캐릭터 

		if (HitRes.Actor.IsValid())//약포인터 방식으로 메모리를 참조하기 때문에 메모리 낭비를 방지
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());//히트 사운드 재생

			if (CollisionName == HitRes.GetComponent()->GetCollisionProfileName())//래그돌 네임과 충돌 결과가 같을시
			{
				// 플레이어와 타깃의 방향을 설정
				FVector ImpulseDirection = HitRes.GetActor()->GetActorLocation() - GetActorLocation();//밀어질 방향 설정(벡터 -연산)
				ImpulseDirection.Normalize();

				float ImpulseStrength = 5000.0;//충돌 세기
				FVector FinalImpulse = ImpulseDirection * ImpulseStrength;//임펄스 설정

				HitRes.GetComponent()->SetPhysicsLinearVelocity(FVector::ZeroVector);
				HitRes.GetComponent()->AddImpulseAtLocation(FinalImpulse, HitRes.GetActor()->GetActorLocation());//해당위치에서 임펄스 만큼 밀어냄
			}

			if (CollisionName2 == HitRes.GetComponent()->GetCollisionProfileName())//콜리전이 AI일때
			{
				auto HitAI = Cast<AShooterEnemyCharacter>(HitRes.GetActor());
				HitAI->SetDamagePoint(GetActorLocation());//내위치를 상대 액터 데미지 포인트에 지정

				FDamageEvent  DamageEvent;

				//데미지 세기,대미지 종류,공격명령 가해자,대미지전달 도구
				HitRes.Actor->TakeDamage(CharacterStat->GetMeleeDamage(), DamageEvent, GetController(), this);
			}
		}
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(this, MissSound, GetActorLocation());//히트 미스 사운드 재생
	}
	
}

float AShooter2Character::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CharacterStat->SetDamage(FinalDamage);//피격시 데미지를 셋함
	return FinalDamage;
}

void AShooter2Character::CharacterDead()
{
	//SetActorEnableCollision(false);//콜리전 없음
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));//캡슐 콜리전을 NoCollision으로
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));//사망시 콜리전을 래그돌로설정
	GetMesh()->SetSimulatePhysics(true);//래그돌 활성화
	HPBarWidget->SetHiddenInGame(true);//HP 위젯 숨김
	DisableInput(ShooterPlayerController);//플레이어 입력 비활성화
	KockBack(2000.0f);//넉백
}

void AShooter2Character::SetDamagePoint(FVector DamageSource)//해당액터에 데미지를 가한 액터 위치 설정 (넉백 방향 설정용)
{
	DamegePoint = DamageSource;
}

void AShooter2Character::KockBack(float Impulse)
{
	// 플레이어와 타깃의 방향을 설정
	FVector ImpulseDirection = GetActorLocation() - DamegePoint;//밀어질 방향 설정(벡터 -연산)
	ImpulseDirection.Normalize();

	float ImpulseStrength = Impulse;//충돌 세기
	FVector FinalImpulse = ImpulseDirection * ImpulseStrength;//임펄스 설정

	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);//충격을 주기전 메시의 속도를 0으로 지정
	GetMesh()->AddImpulseToAllBodiesBelow(FinalImpulse, NAME_None);//해당위치에서 임펄스 만큼 밀어냄
}

void AShooter2Character::WeaponModeChange()//총 발사모드 변경
{
	if (ShooterAnim->IsGunUsed()==true)
	{
		UE_LOG(Shooter2, Error, TEXT("ModeChange!!"));
		CurrentGun->ModeChange();
	}
}

void AShooter2Character::GunGrap()//무기스왑 몽타주 실행
{
	ShooterAnim->PlayWeaponSwapMontage();
}

void AShooter2Character::SwapWeapon()//무기 장착/해제
{
	if (CurrentGun != nullptr)
	{
		if (ShooterAnim->IsGunUsed() == false)
		{
			//무기 사용 활성화
			ShooterAnim->SetGunUsed(true);

			//무기를 잡을 손으로 이동
			FName WeaponSocket1(TEXT("my_hand_rSocket"));
			CurrentGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			CurrentGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket1);
		}
		else
		{
			//무기 사용 비활성화
			ShooterAnim->SetGunUsed(false);

			//무기를 등으로 이동
			FName WeaponSocket2(TEXT("my_spine_Socket"));
			CurrentGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			CurrentGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket2);
		}
	}
}

void AShooter2Character::SetWeapon(class AShooterGripGun* Weapon)//무기 장착
{
	FName WeaponSocket(TEXT("my_spine_Socket"));
	if (nullptr != Weapon)
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		Weapon->SetOwner(this);
		CurrentGun = Weapon;
	}
}

bool AShooter2Character::CanSetWeapon()//무기 집기 가능 여부
{
	return (nullptr == CurrentGun);
}

void AShooter2Character::ThrowGrena()//수류탄 투척
{
	if (CharacterStat->GetGrenadeCount() > 0)
	{
		ShooterAnim->PlayThrowGrenade();
	}
}

void AShooter2Character::GrenadeSet()
{
	if (ShooterAnim->IsGunUsed())//무기를 사용중일 때 무기를 등으로 이동
	{
		FName WeaponSocket2(TEXT("my_spine_Socket"));
		CurrentGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket2);
	}
	VisibleGrenade->SetHiddenInGame(false, false);//수류탄 보임
	bUseControllerRotationYaw = true;//캐릭터가 카메라 회전방향으로 회전 On
	if (ShooterAnim->IsSprinting())
	{
		Walk();
	}
}
void AShooter2Character::GrenadeRelease()
{
	const FRotator SpawnRotation = GetControlRotation();
	const FVector SpawnLocation = GrenadeScene->GetComponentLocation();
	VisibleGrenade->SetHiddenInGame(true, true);//수류탄 숨김
	GetWorld()->SpawnActor<AShooterGrenade>(AShooterGrenade::StaticClass(), SpawnLocation, SpawnRotation);//수류탄 소환
	CharacterStat->SetGrenadeCount(-1);//수류탄 1개 빠짐 및 브로드캐스트
	bUseControllerRotationYaw = false;//캐릭터가 카메라 회전방향으로 회전 Off
	if (ShooterAnim->IsSprinting())
	{
		Run();
	}
}
void AShooter2Character::WeaponSetAfterGrenade()//수류탄 투척후 무기를 사용 중일 때 무기를 장착
{
	if (ShooterAnim->IsGunUsed())
	{
		FName WeaponSocket1(TEXT("my_hand_rSocket"));
		CurrentGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket1);//무기를 사용중일 때 무기를 손으로 이동
	}
	
}

void AShooter2Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ShooterAnim = Cast<UShooterAnimInstance>(GetMesh()->GetAnimInstance());//애님 인스턴스 생성

	ShooterAnim->OnWeaponSwap.AddUObject(this, &AShooter2Character::SwapWeapon);//노티파이 확인시 무기스왑 
	ShooterAnim->OnAttackCheck.AddUObject(this, &AShooter2Character::MeleeAttack);//밀리어택 확인
	ShooterAnim->OnGrenadeStart.AddUObject(this, &AShooter2Character::GrenadeSet);//수류탄 시작
	ShooterAnim->OnGrenadeEnd.AddUObject(this, &AShooter2Character::GrenadeRelease);//수류탄 끝
	ShooterAnim->OnWeaponSet.AddUObject(this, &AShooter2Character::WeaponSetAfterGrenade);//수류탄 투척후 무기장착
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AShooter2Character::OnSupplyBoxOverLap);//오버랩을 처리하는 델리게이트 함수(서플라이 박스)
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AShooter2Character::OnSupplyBoxEndOverLap);//오버랩을 처리하는 델리게이트 함수(서플라이 박스)

	HPBarWidget->InitWidget();//위젯 초기화 필요(블루프린트 위젯 부모도 변경 필요) 
	auto CharacterWidget = Cast<UShooterCharacterWidget>(HPBarWidget->GetUserWidgetObject());//위젯 호출
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);//캐릭터 위젯과 스탯 컴포넌트 바인딩
	}
}

FVector AShooter2Character::GetTargetPoint() const//데미지 포인트 반환
{
	return DetectedScene->GetComponentLocation();
}

bool AShooter2Character::GetIsAttacking() const//공격중인지 여부 반환
{
	return bIsAttacking;
}

void AShooter2Character::OnSupplyBoxOverLap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)//오버랩 이벤트를 처리하는 함수(서플라이박스)
{
	FName CollisionName(TEXT("Supply"));//서플라이박스 네임
	if (OtherComp->GetCollisionProfileName() == CollisionName)
	{
		SupplyBox = Cast<ASupplyBox>(OtherActor);//서플라이박스 설정
		bCanSupply = true;//보급가능 활성화
	}
}


void AShooter2Character::OnSupplyBoxEndOverLap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)//오버랩 이벤트를 처리하는 함수(서플라이박스)
{
	FName CollisionName(TEXT("Supply"));//서플라이박스 네임
	if (OtherComp->GetCollisionProfileName() == CollisionName && SupplyBox)
	{
		SupplyBox = nullptr;//서플라이박스 설정
		bCanSupply = false;//보급가능 비활성화
	}
	
}

void AShooter2Character::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		
	}
	ShooterAnim->MoveForward = Value;//애님 인스턴스의 변수에 입력 값을 삽입
}

void AShooter2Character::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);

		
	}
	ShooterAnim->MoveRight = Value;//애님 인스턴스의 변수에 입력 값을 삽입
}

void AShooter2Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AShooter2Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}