// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterBossCharacter.h"
#include "ShooterBossAIController.h"//Boss AI
#include "ShooterGripGun.h"//���� ������ ��
#include "DrawDebugHelpers.h"//����� �׸���
#include "Runtime/Engine/Public/TimerManager.h"//Ÿ�̸� �Ŵ���
#include "ShooterActorComponent.h"//ĳ���� ����
#include "Components/WidgetComponent.h"//UI���� ������� 
#include "ShooterCharacterWidget.h"//�� ĳ���� ����
#include "ShooterGrenade.h"//����ź
#include "ShooterBossMissile.h"//�̻���
#include "ShooterAICharacter.h"//AIĳ����
#include "ShooterGameState.h"//���� ������Ʈ (������ �׾��� �� Ŭ���� ����)
#include "Shooter2GameMode.h"//���Ӹ��

#include "ShooterCharacterPlayerController.h"//�÷��̾���Ʈ�ѷ�
#include "Shooter2Character.h"//�÷��̾� ĳ����
#include "ShooterHUDWidget.h"//HUD����

//AI ���� ĳ������ ���� �� ��ɱ���
AShooterBossCharacter::AShooterBossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DeadTimer = 3.0f;//���� Ÿ�̸� �ð�(���� �����)
	WidgetDeadTimer = 3.0f;//���� ���� Ÿ�̸� �ð�(������ ���ӽð�)
	StandSpeed = 400.0f;//�ӵ�

	AIControllerClass = AShooterBossAIController::StaticClass();//AI Control Ŭ������ ����
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;//�����ǰų� ���忡 ���� �� �ڵ����� AI��Ʈ�ѷ��� ����

	// �浹 ĸ���� ũ�� ����
	GetCapsuleComponent()->InitCapsuleSize(160.0f, 160.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("AIChara"));//�ݸ��� ����

	// ĳ���� ������ ����(ȸ���� �ε巴��)
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;//�ε巯�� ȸ��
	GetCharacterMovement()->bOrientRotationToMovement = true; // ĳ���Ͱ� �Է� �������� ȸ���մϴ�	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // �� ȸ�� �ӵ���
	GetCharacterMovement()->JumpZVelocity = 450.0f;//���� ����
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->bCanWalkOffLedges = true;//�ɾ��� �� �������� �������� ����
	GetCharacterMovement()->MaxWalkSpeed = StandSpeed;//ĳ���� ���ǵ� ����

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_Body(TEXT("/Game/MyAsset/BOSS/BossMesh.BossMesh"));//ĳ���� ��Ų �޽�
	if (SK_Body.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Body.Object);
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));//�޽ô� �ݸ��� ��Ȱ��
	}

	ImpactParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("ImpactParticle"));//�Ѿ� ����Ʈ ��ƼŬ
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_Impact(TEXT("/Game/MyAsset/PS_Impact.PS_Impact"));
	if (PT_Impact.Succeeded())
	{
		ImpactParticle = PT_Impact.Object;
	}

	PitanSound = CreateDefaultSubobject<USoundBase>(TEXT("Pitan"));//��ź ����
	static ConstructorHelpers::FObjectFinder<USoundBase>SB_PitanSound(TEXT("/Game/MyAsset/SC_Pitan.SC_Pitan"));
	if (SB_PitanSound.Succeeded())
	{
		PitanSound = SB_PitanSound.Object;
	}

	HP=1500.0f
	CharacterStat->SetCurrentHP(HP);//HP��������
	CharacterStat->SetMaxHP(HP);//max HP��������

	HPBarWidget->SetupAttachment(GetMesh());//������ �޽��� ����

	BossMissileScene = CreateDefaultSubobject<USceneComponent>(TEXT("BossMissileScene"));//�̻��� ��ȯ ��
	BossMissileScene->SetupAttachment(RootComponent);//��Ʈ ������Ʈ�� ����
	BossMissileScene->SetRelativeLocation(FVector(50.0f,40.0f,130.0f));//�̻��� ��ȯ�� ��ġ����

	MissileFireParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MissileFireParticle"));//�߻� ��ƼŬ
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_FireMissile(TEXT("/Game/MyAsset/PS_MissileFlash.PS_MissileFlash"));
	if (PT_FireMissile.Succeeded())
	{
		MissileFireParticle->SetTemplate(PT_FireMissile.Object);
		MissileFireParticle->bAutoActivate = false;//�ڵ����� Off
		MissileFireParticle->SetupAttachment(BossMissileScene);//��ƼŬ�� ���� �ſ� ����
	}
	

	MissileFireSound = CreateDefaultSubobject<USoundBase>(TEXT("MissileFireSound"));//�̻��� �߻� ����
	static ConstructorHelpers::FObjectFinder<USoundBase>SB_FireMissile(TEXT("/Game/MyAsset/SC_MissileLaunch.SC_MissileLaunch"));
	if (SB_FireMissile.Succeeded())
	{
		MissileFireSound = SB_FireMissile.Object;
	}

	MissileHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MISSILEHEAD"));//�̻��� �߻�� ��ü
	MissileLauncher = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MISSILELAUNCCHER"));//�̻��� �߻��

	FString AssetPath = TEXT("/Game/MyAsset/MissleLauncher/Mesh/StaticM_MissileLauncherTurret_Body.StaticM_MissileLauncherTurret_Body");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_TURRETHEAD(*AssetPath);//�̻��� �߻�� ��ü �޽� �ҷ�����
	if (SM_TURRETHEAD.Succeeded())
	{
		MissileHead->SetStaticMesh(SM_TURRETHEAD.Object);
		MissileHead->SetCollisionProfileName(TEXT("NoCollision"));//�޽ô� �ݸ��� ��Ȱ��
		MissileHead->SetRelativeScale3D(FVector(0.8f,0.8f,0.6f));//�޽�ũ�� ����
	}

	FString AssetPath2 = TEXT("/Game/MyAsset/MissleLauncher/Mesh/StaticM_MissileLauncherTurret_Launcher.StaticM_MissileLauncherTurret_Launcher");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_TURRETLAUNCHER(*AssetPath2);//�̻��� �߻�� �޽� �ҷ�����
	if (SM_TURRETLAUNCHER.Succeeded())
	{
		MissileLauncher->SetStaticMesh(SM_TURRETLAUNCHER.Object);
		MissileLauncher->SetCollisionProfileName(TEXT("NoCollision"));//�޽ô� �ݸ��� ��Ȱ��
	}

	FName MissileHeadSocket(TEXT("MissileHead"));
	if (MissileHead->DoesSocketExist(MissileHeadSocket))//�̻��� �߻�� ��ü �޽ÿ� �ش� ������ �����Ұ��
	{
		MissileLauncher->SetupAttachment(MissileHead, MissileHeadSocket);//�߻�븦 �߻�� ��ü�� ����
	}

	FName MissileSocket(TEXT("MissileSoket"));
	MissileHead->SetupAttachment(GetMesh(), MissileSocket);//�̻��Ϸ�ó�� ���� ���Ͽ� ����

	for (int i = 0; i < 2; ++i)//����ź ���� �� �ʱ�ȭ
	{
		//�� �ʱ�ȭ
		FString SceneName = "Scene" + FString::FromInt(i);
		FName SceneID = FName(*SceneName);
		USceneComponent* thisScene = CreateDefaultSubobject<USceneComponent>(SceneID);
		check(thisScene);

		BossGrenadeScene.Add(thisScene);//BossGrenadeScene �迭 ����ü�� ���� �߰�
	}

	float SpawnPoint = -50.0f;
	for (auto Scene : BossGrenadeScene)//���� ����ź ���� ���� ��ġ�� ����
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
	

	//�ѻ��� �� �������
	FName WeaponSocket(TEXT("GunSoket"));
	NewWeapon = GetWorld()->SpawnActor<AShooterGripGun>(FVector::ZeroVector, FRotator::ZeroRotator);//�ѻ���
	NewWeapon->SetActorScale3D(FVector(2.5f));
	NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);//�ش� ������Ʈ�� �ش� ���Ͽ� ����
	NewWeapon->SetOwner(this);//�����ڴ� ����

	SetCharacterState(ECharacterState::READY);//������ ���� ����

}

void AShooterBossCharacter::SetCharacterState(ECharacterState NewState)
{
	CurrentState = NewState;//���� ������Ʈ ����

	switch (CurrentState)
	{
		case ECharacterState::READY://�غ����
		{
			SetActorHiddenInGame(false);//���� ����
			HPBarWidget->SetHiddenInGame(true);//HP ���� ����

			CharacterStat->OnHPIsZero.AddLambda([this]()->void {//HP 0�� ��������Ʈ�� ����Ͽ� DEAD ������Ʈ�� ����
				SetCharacterState(ECharacterState::DEAD);
				});


			break;
		}
		case ECharacterState::DEAD://���� ����
		{
			CharacterDead();//ĳ���� ����
			auto PlayerController = Cast<AShooterCharacterPlayerController>(this->LastHitBy);//������ ���������� ���� ��Ʈ�ѷ��� ��ȯ
			auto GameMode = Cast<AShooter2GameMode>(GetWorld()->GetAuthGameMode());//���Ӹ�带 ������
			GameMode->AddScore(PlayerController);//�ش� �÷��̾� ���� �߰�
			GameMode->ShooterGameState->SetGameCleared();//���� Ŭ���� ����

			//3�ʵ� ���� Ŭ���� ȭ�� ǥ��
			GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
					//���ӿϷ�� ���ͷ����͸� �̿��Ͽ� �÷��̾� �� �� NPC �� ��Ȱ��ȭ
					for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++)
					{
						(*It)->TurnOff();
					}

					//���ӿϷ�� ���ͷ����͸� �̿��Ͽ� �÷��̾� ��Ʈ�ѷ��� ã�� ���ȭ�� ���
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

ECharacterState AShooterBossCharacter::GetCharacterState() const//���� ĳ���� ���� ��ȯ
{
	return CurrentState;
}

void AShooterBossCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	HPBarWidget->InitWidget();//���� �ʱ�ȭ �ʿ�(�������Ʈ ���� �θ� UShooterCharacterWidget�� ���� �ʿ�) 
	auto CharacterWidget = Cast<UShooterCharacterWidget>(HPBarWidget->GetUserWidgetObject());//UWidgetComponent�� UShooterCharacterWidget�� ĳ����
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);//������ ���� Ŭ������ ���ε�
	}
}

float AShooterBossCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!ShooterAIController->IsAIRunning())//AI�� �񰡵��Ͻ� ���� 
	{
		ShooterAIController->RunAI();//AI ����
		ShooterAIController->bIsAIRunning = true;
	}
	
	HPBarWidget->SetHiddenInGame(false);//HP ���� ����

	GetWorld()->GetTimerManager().SetTimer(WidgetDeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
		HPBarWidget->SetHiddenInGame(true);//Ÿ�̸� �ð� �� HP ���� ����
		}), WidgetDeadTimer, false);

	ShooterAIController->TrackToTargetOutRange(DamageCauser->GetActorLocation());//������ ������ ������ �� �����ġ�� �̵�

	CharacterStat->SetDamage(FinalDamage);//�ǰݽ� �������� ����

	return FinalDamage;
}

void AShooterBossCharacter::Fire()//�Ѿ� �߻�
{
	NewWeapon->PlayFireSound();//�߻� ���� ����
	NewWeapon->FirePaticle();//�߻� ��ƼŬ ����
	NewWeapon->SingleEject();//�߻�� ź�� ����Ʈ ��ƼŬ����

	auto TargetCharacter = Cast<AShooter2Character>(GetWorld()->GetFirstPlayerController()->GetPawn());//Ÿ���ӽû���
	
	//TargetCharacter->Controll

	FVector Start = NewWeapon->MuzzleScene->GetComponentLocation();//����Ʈ���̽��� ������ �������� (���� �����)
	float RandX = FMath::FRandRange(-50.0f, 50.0f);
	float RandY = FMath::FRandRange(-50.0f, 50.0f);
	float RandZ = FMath::FRandRange(-50.0f, 50.0f);
	//���� Ʈ���̽��� ������ ������(�ش� �÷��̾��� ��ġ��)
	FVector End = FVector(TargetCharacter->GetActorLocation().X + RandX, TargetCharacter->GetActorLocation().Y + RandY, TargetCharacter->GetActorLocation().Z + 20.0f + RandZ);

	FCollisionObjectQueryParams ObjQuery;//���� �޴� �ν��Ͻ�
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel1);// ���ݹ޴� Ʈ���̽� ä�� ���� �߰� (ĳ����)
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel2);// ���ݹ޴� Ʈ���̽� ä�� ���� �߰� (AI)
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel4);// ���ݹ޴� Ʈ���̽� ä�� ���� �߰� (���)
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel6);// ���ݹ޴� Ʈ���̽� ä�� ���� �߰� (AIMissile)
	ObjQuery.AddObjectTypesToQuery(ECC_PhysicsBody);//�������ٵ� �߰�

	FCollisionQueryParams ColQuery;//���� ��ü �ν��Ͻ�
	ColQuery.AddIgnoredActor(this);//���� ��ü ���ʹ� ���� ����

	FHitResult HitRes;//��Ʈ Ȯ�� �ν��Ͻ�

	bool Result = GetWorld()->LineTraceSingleByObjectType(HitRes, Start, End, ObjQuery, ColQuery);//���� Ʈ���̽��� ��ƮȮ�ΰ˻�


	if (HitRes.bBlockingHit)//��� �ν��Ͻ� ��ȿ�� �� 
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, HitRes.ImpactPoint);//��Ʈ������ �Ѿ� ��ź ��ƼŬ ����
		UGameplayStatics::PlaySoundAtLocation(this, PitanSound, HitRes.ImpactPoint);//��ź �������

		FName CollisionName(TEXT("Ragdoll"));//���׵� ����
		FName CollisionName2(TEXT("AIChara"));//AIĳ���� ����
		FName CollisionName3(TEXT("ShooterChara"));//�÷��̾� ĳ���� ����

		if (CollisionName == HitRes.GetComponent()->GetCollisionProfileName() && HitRes.Actor.IsValid())//���׵� ���Ӱ� �浹 ����� ������
		{
			// �÷��̾�� Ÿ���� ������ ����
			FVector ImpulseDirection = HitRes.GetActor()->GetActorLocation() - GetActorLocation();//�о��� ���� ����(���� -����)
			ImpulseDirection.Normalize();

			//�浹 ����
			float ImpulseStrength = 5000.0;

			FVector FinalImpulse = ImpulseDirection * ImpulseStrength;//���޽� ����

			HitRes.GetComponent()->SetPhysicsLinearVelocity(FVector::ZeroVector);
			HitRes.GetComponent()->AddImpulseAtLocation(FinalImpulse, HitRes.GetActor()->GetActorLocation());//�ش���ġ���� ���޽� ��ŭ �о
		}

		if (HitRes.Actor.IsValid())//��ȿ�� ���ʹ� �������� �����ϰ� �� ��ġ�� ��� ������ ����Ʈ�� ����
		{
			if (CollisionName2 == HitRes.GetComponent()->GetCollisionProfileName())//�ݸ����� AI�϶�
			{
				auto HitAI = Cast<AShooterAICharacter>(HitRes.GetActor());
				HitAI->SetDamagePoint(GetActorLocation());//������ġ�� ��� ���� ������ ����Ʈ�� ����
				
			}
			else if (CollisionName3 == HitRes.GetComponent()->GetCollisionProfileName())//�ݸ����� ĳ�����϶� �϶�
			{
				auto PlayerChara = Cast<AShooter2Character>(HitRes.GetActor());
				PlayerChara->SetDamagePoint(GetActorLocation());//������ġ�� ��� ���� ������ ����Ʈ�� ����
			}
			FDamageEvent  DamageEvent;
			HitRes.Actor->TakeDamage(CharacterStat->GetAIBossShootDamage(), DamageEvent, GetController(), this);//������ ����,����� ����,���ݸ�� ������,��������� ����
		}
	}
	
}

void AShooterBossCharacter::CharacterDead()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));//�ݸ��� ����
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));//�ݸ����� ���׵��μ���
	GetMesh()->SetSimulatePhysics(true);//���׵� Ȱ��ȭ
	KockBack(70000.0f);
	ShooterAIController->StopAI();//AI����
	HPBarWidget->SetHiddenInGame(true);//HP ���� ����
}

void AShooterBossCharacter::Grenade()
{
	UGameplayStatics::PlaySoundAtLocation(this, MissileFireSound, GetActorLocation());//����ź �߻�Ҹ� ����
	for (auto Grenade : BossGrenadeScene)//����ź �Ÿ��� ����ź ����
	{
		FVector SpawnLoc = Grenade->GetComponentLocation();
		FRotator SpawnRot = Grenade->GetComponentRotation();
		AShooterGrenade* pGrenade = GetWorld()->SpawnActor<AShooterGrenade>(AShooterGrenade::StaticClass(), SpawnLoc, SpawnRot);//����ź ����
	}
}

void AShooterBossCharacter::Missile()//�̻��� �߻�
{
	UGameplayStatics::PlaySoundAtLocation(this, MissileFireSound, GetActorLocation());//�̻��� �߻�Ҹ� ����
	MissileFireParticle->Activate();//�̻��� �߻� ��ƼŬ ����

	FVector MissileEjectLocation = BossMissileScene->GetComponentLocation();
	FRotator MissileEjectRotation = GetActorRotation();
	MissileEjectRotation.Pitch -= 90.0f;
	GetWorld()->SpawnActor<AShooterBossMissile>(AShooterBossMissile::StaticClass(), MissileEjectLocation, MissileEjectRotation);//�̻��� ����
}


// Called every frame
void AShooterBossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	auto PlayerCharacter = Cast<AShooter2Character>(GetWorld()->GetFirstPlayerController()->GetPawn());//Ÿ���ӽû���
	auto PlayerCharacterController = Cast<AShooterCharacterPlayerController>(PlayerCharacter->GetController());//Ÿ����Ʈ�ѷ�
	float Distance = PlayerCharacter->GetDistanceTo(this);

	PlayerCharacterController->GetHUDWidget()->GetDistanceToBoss(Distance);
	
}

// Called to bind functionality to input
void AShooterBossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}