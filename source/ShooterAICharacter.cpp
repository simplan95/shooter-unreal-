// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAICharacter.h"
#include "Components/CapsuleComponent.h"

#include "ShooterAIAnimInstance.h"//AI �ִ� �ν��Ͻ�
#include "ShooterGun.h"//�ʵ� ������ ��
#include "ShooterGripGun.h"//���� ������ ��
#include "DrawDebugHelpers.h"//����� �׸���
#include "Runtime/Engine/Public/TimerManager.h"//Ÿ�̸� �Ŵ���
#include "ShooterCharacterPlayerController.h"//�÷��̾���Ʈ�ѷ�
#include "ShooterActorComponent.h"//ĳ���� ����
#include "Components/WidgetComponent.h"//UI���� ������� ��Ŭ���
#include "ShooterCharacterWidget.h"//�� ĳ���� ����
#include "ShooterAIController.h"//Ai ��Ʈ�ѷ� ��Ŭ���
#include "Shooter2Character.h"//�÷��̾� ĳ����
#include "ShooterBossCharacter.h"//A ����
#include "Shooter2GameMode.h"//���Ӹ��

//AI �� ĳ���� ���� �� ��� ����
AShooterAICharacter::AShooterAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DeadTimer = 30.0f;//���� Ÿ�̸�(AI �����)
	WidgetDeadTimer = 3.0f;//���� ���� Ÿ�̸�(���� ���ӽð�)
	StandSpeed = 260.0f;//�ȱ�ӵ�
	StandRunSpeed = 400.0f;//�޸���ӵ�

	AIControllerClass = AShooterAIController::StaticClass();//AI Control Ŭ������ ����
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;//�����ǰų� ���忡 ���� �� �ڵ����� AI��Ʈ�ѷ��� ����

	// �浹 ĸ���� ũ�� ����
	GetCapsuleComponent()->InitCapsuleSize(42.f, 90.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("AIChara"));//�ݸ��� ����

	// ĳ���� ������ ����(ȸ���� �ε巴��)
	bUseControllerRotationYaw = false;//��Ʈ�ѷ��� ȸ���� ���� Z�� ȸ���� ��
	GetCharacterMovement()->bUseControllerDesiredRotation = true;//�ε巯�� ȸ��
	GetCharacterMovement()->bOrientRotationToMovement = true; // ĳ���Ͱ� �Է� �������� ȸ���մϴ�	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // �� ȸ�� �ӵ���
	GetCharacterMovement()->JumpZVelocity = 450.0f;//��������
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->bCanWalkOffLedges = true;//�ɾ��� �� �������� �������� ����

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -87.0f), FRotator(0.0f, 270.0f, 0.0f));//�� �⺻��ġ ����
	GetCharacterMovement()->MaxWalkSpeed = StandSpeed;//ĳ���� ���ǵ� ����

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_Body(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));//ĳ���� ��Ų
	if (SK_Body.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Body.Object);//ĳ���� ��Ų�߰�
	}


	//�ִϸ��̼� �߰�
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> myAnim(TEXT("/Game/Mannequin/Animations/AI/ThirdPerson_AnimBP_AI.ThirdPerson_AnimBP_AI_C"));
	//�ִ��ν��Ͻ� �߰� �������Ʈ class �ҷ����� �ڿ� _C�� �ٿ� Ŭ���������� �������� ��θ� ����
	if (myAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(myAnim.Class);
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));//�޽ô� �ݸ��� ��Ȱ��
	}

	ImpactParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("ImpactParticle"));//�Ѿ� ����Ʈ ��ƼŬ
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_Impact(TEXT("/Game/MyAsset/PS_Impact.PS_Impact"));
	if (PT_Impact.Succeeded())
	{
		ImpactParticle =PT_Impact.Object;
	}

	PitanSound = CreateDefaultSubobject<USoundBase>(TEXT("Pitan"));//��ź ����
	static ConstructorHelpers::FObjectFinder<USoundBase>SB_PitanSound(TEXT("/Game/MyAsset/SC_Pitan.SC_Pitan"));
	if (SB_PitanSound.Succeeded())
	{
		PitanSound = SB_PitanSound.Object;
	}

	DetectScene = CreateDefaultSubobject<USceneComponent>(TEXT("DetecteScene"));//AI�� �������� ���� ��
	DetectScene->SetupAttachment(RootComponent);//��Ʈ ������Ʈ�� ����
	DetectScene->SetRelativeLocation(FVector(0.0f, 0.0f, 75.0f));//�� ��ġ ����

	HP = 100.0f;
	CharacterStat->SetCurrentHP(HP);//������ ����
	CharacterStat->SetMaxHP(HP);//���� ����

	HPBarWidget->SetupAttachment(GetMesh());//������ �޽��� ����

	SetActorHiddenInGame(true);//���� ����
	HPBarWidget->SetHiddenInGame(true);//HP ���� ����
}

// Called when the game starts or when spawned
void AShooterAICharacter::BeginPlay()
{
	Super::BeginPlay();

	ShooterAIController = Cast<AShooterAIController>(GetController());
	ShooterAIController->RunAI();//AI ����

	//�ѻ��� �� �������
	FName WeaponSocket(TEXT("my_hand_rSocket"));
	NewWeapon = GetWorld()->SpawnActor<AShooterGripGun>(FVector::ZeroVector, FRotator::ZeroRotator);//�ѻ���
	NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	NewWeapon->SetOwner(this);

	CharacterStat->SetMagazineCount(30);//�ʱ� źâ �Ѿ˼� ����

	SetCharacterState(ECharacterState::READY);
}

void AShooterAICharacter::SetCharacterState(ECharacterState NewState)
{
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ECharacterState::READY:
	{
		SetActorHiddenInGame(false);//���� ����
		HPBarWidget->SetHiddenInGame(true);//HP ���� �Ⱥ���

		//HP 0�� �� ��������Ʈ�� ����Ͽ� DEAD ������Ʈ�� ����
		CharacterStat->OnHPIsZero.AddLambda([this]()->void {
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
	ShooterAnim = Cast<UShooterAIAnimInstance>(GetMesh()->GetAnimInstance());//�ִ� �ν��Ͻ� ����

	ShooterAnim->OnRestrictStart.AddUObject(this, &AShooterAICharacter::FCannotAttack);//�������� Ȯ�ν� ���ݰ���
	ShooterAnim->OnRestrictEnd.AddUObject(this, &AShooterAICharacter::FCanAttack);//�������� Ȯ�ν� ���ݺҰ�


	HPBarWidget->InitWidget();//���� �ʱ�ȭ �ʿ�(�������Ʈ ���� �θ� UShooterCharacterWidget�� ���� �ʿ�) 
	auto CharacterWidget = Cast<UShooterCharacterWidget>(HPBarWidget->GetUserWidgetObject());//���� ȣ��
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}
}

void AShooterAICharacter::CharacterDead()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));//�ݸ��� ����
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));//�ݸ����� ���׵��μ���
	GetMesh()->SetSimulatePhysics(true);//���׵� Ȱ��ȭ
	HPBarWidget->SetHiddenInGame(true);//HP ���� ����
	KockBack(2000.0f);
	NewWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);//�Ѷ���
	NewWeapon->SetActorEnableCollision(false);//�ݸ��� ��Ȱ��ȭ
	NewWeapon->Destroy();//���ı�
	NewWeapon->Gun->SetHiddenInGame(true, true);//�� ����, USkeletalMeshComponent������ ��밡��
	GetWorld()->SpawnActor<AShooterGun>(GetActorLocation(), FRotator::ZeroRotator);//�÷��̾ �ݰԵ� �ѻ���
	ShooterAIController->StopAI();//AI����

	auto PlayerController = Cast<AShooterCharacterPlayerController>(this->LastHitBy);//������ ���������� ���� ��Ʈ�ѷ��� ��ȯ
	auto GameMode = Cast<AShooter2GameMode>(GetWorld()->GetAuthGameMode());//���Ӹ�带 ������
	GameMode->AddScore(PlayerController);//�ش� �÷��̾� ���� �߰�
}

void AShooterAICharacter::Run()//Ÿ�� Ȯ�ν� �޸���ӵ���
{
	GetCharacterMovement()->MaxWalkSpeed = StandRunSpeed;
	ShooterAnim->SetIsSprinting(true);
	
}

void AShooterAICharacter::Walk()//Ÿ�� ��Ȯ�ν� �ȱ�ӵ�
{
	GetCharacterMovement()->MaxWalkSpeed = StandSpeed;
	ShooterAnim->SetIsSprinting(false);
}

void AShooterAICharacter::Fire()
{
	CharacterStat->SetMagazineCount(-1);//źâ���� �ѹ� ����

	//źâ�� �Ѿ��� ���� ��
	if (CharacterStat->GetMagazineBullet()>0)
	{
		NewWeapon->PlayFireSound();
		NewWeapon->FirePaticle();
		NewWeapon->SingleEject();

		auto TargetCharacter = Cast<AShooter2Character>(GetWorld()->GetFirstPlayerController()->GetPawn());//Ÿ���ӽû���

		FVector Start = NewWeapon->MuzzleScene->GetComponentLocation();//����Ʈ���̽� ��������
		float RandX = FMath::FRandRange(-50.0f, 50.0f);
		float RandY = FMath::FRandRange(-50.0f, 50.0f);
		float RandZ = FMath::FRandRange(-50.0f, 50.0f);
		//����Ʈ���̽� ������
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

		//���� Ʈ���̽��� ��ƮȮ�ΰ˻�
		bool Result = GetWorld()->LineTraceSingleByObjectType(HitRes, Start, End, ObjQuery, ColQuery);

		//��� �ν��Ͻ� ��ȿ�� ��
		if (HitRes.bBlockingHit) 
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, HitRes.ImpactPoint);//��Ʈ������ �Ѿ� ��ź ��ƼŬ ����
			UGameplayStatics::PlaySoundAtLocation(this, PitanSound, HitRes.ImpactPoint);//��ź ���� ���

			FName CollisionName(TEXT("Ragdoll"));//���׵� ����
			FName CollisionName2(TEXT("AIChara"));//AIĳ���� ����
			FName CollisionName3(TEXT("ShooterChara"));//AIĳ���� ����

			//���׵� ���Ӱ� �浹 ����� ������
			if (CollisionName == HitRes.GetComponent()->GetCollisionProfileName() && HitRes.Actor.IsValid())
			{
				// �÷��̾�� Ÿ���� ������ ����
				FVector ImpulseDirection = HitRes.GetActor()->GetActorLocation() - GetActorLocation();//�о��� ���� ����(���� ����Ȱ��)
				ImpulseDirection.Normalize();

				//�浹 ����
				float ImpulseStrength = 5000.0;

				FVector FinalImpulse = ImpulseDirection * ImpulseStrength;//���޽� ����

				HitRes.GetComponent()->SetPhysicsLinearVelocity(FVector::ZeroVector);
				HitRes.GetComponent()->AddImpulseAtLocation(FinalImpulse, HitRes.GetActor()->GetActorLocation());//�ش���ġ���� ���޽� ��ŭ �о
			}

			//��ȿ�� ���ʹ� �������� �����ϰ� �� ��ġ�� ��� ������ ����Ʈ�� ����
			if (HitRes.Actor.IsValid())
			{
				if (CollisionName2 == HitRes.GetComponent()->GetCollisionProfileName())//�ݸ����� AI�϶���
				{
					auto HitAI = Cast<AShooterEnemyCharacter>(HitRes.GetActor());
					HitAI->SetDamagePoint(GetActorLocation());//���� �������� ��ġ�� ��� ���� ������ ����Ʈ�� ����
				}
				else if (CollisionName3 == HitRes.GetComponent()->GetCollisionProfileName())//�ݸ����� ĳ�����϶� �϶�
				{
					auto PlayerChara = Cast<AShooter2Character>(HitRes.GetActor());
					PlayerChara->SetDamagePoint(GetActorLocation());//���� �������� ��ġ�� ��� ���� ������ ����Ʈ�� ����
				}
				FDamageEvent  DamageEvent;

				//������ ����. ������ ����,����� ����,���ݸ�� ������,��������� ����
				HitRes.Actor->TakeDamage(CharacterStat->GetAIShootDamage(), DamageEvent, GetController(), this);

			}
		}
	}
	else if(CharacterStat->GetMagazineBullet() <= 0)//�Ѿ� ���� �� ���ε�
	{
		Reload();
	}
}

void AShooterAICharacter::Reload()
{
	ShooterAnim->PlayReloadMontage();//������ �ִϸ��̼� ���
	NewWeapon->PlayReloadSound();//������ �Ҹ� ���
	CharacterStat->SetMagazineCount(30);//źâ �Ѿ� �� �߰�
}

void AShooterAICharacter::FCanAttack()//���ݰ���
{
	CanAttack = true;
}

void AShooterAICharacter::FCannotAttack()//���ݺҰ���
{
	CanAttack = false;
}

float AShooterAICharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	HPBarWidget->SetHiddenInGame(false);//HP ���� ����

	GetWorld()->GetTimerManager().SetTimer(WidgetDeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
		HPBarWidget->SetHiddenInGame(true);//Ÿ�̸� �ð� �� HP ���� ����
		}), WidgetDeadTimer, false);

	ShooterAnim->PlayHitReactMontage();//�ǰ� ��Ÿ�� ����
	
	ShooterAIController->TrackToTargetOutRange(DamageCauser->GetActorLocation());//�ǰݽ� AI ��Ʈ�ѷ��� ������ü�� ��ġ���� ������ �װ����� �̵��ϰԲ� ��

	CharacterStat->SetDamage(FinalDamage);//�ǰݽ� �������� ����
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

FVector AShooterAICharacter::GetDtectPoint() const//Ž���� �����Ұ� ��ȯ
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