// Copyright Epic Games, Inc. All Rights Reserved.

#include "Shooter2Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "ShooterAnimInstance.h"//�ִ� �ν��Ͻ�
#include "ShooterGun.h"//�ʵ� ������ ��
#include "ShooterGripGun.h"//���� ������ ��
#include "DrawDebugHelpers.h"//����� 
#include "Runtime/Engine/Public/TimerManager.h"//Ÿ�̸� �Ŵ���
#include "FireCameraShake.h"//ī�޶� ����ũ
#include "ShooterCharacterPlayerController.h"//�÷��̾� ��Ʈ�ѷ�
#include "ShooterActorComponent.h"//ĳ���� ����
#include "Components/WidgetComponent.h"//UI���� 
#include "ShooterCharacterWidget.h"//�� ĳ���� ����
#include "ShooterGrenade.h"//����ź �߻�ü
#include "ShooterGripGrenade.h"//����ź ������
#include "ShooterHUDWidget.h"//HUD
#include "SupplyBox.h"//���ö��̹ڽ�

#include "ShooterEnemyCharacter.h"//���ʹ� origin ĳ����
#include "ShooterAIEnemyController.h"//���ʹ� origin ��Ʈ�ѷ�

//////////////////////////////////////////////////////////////////////////
// AShooter2Character

AShooter2Character::AShooter2Character()
{
	
	PrimaryActorTick.bCanEverTick = true;

	// �浹 ĸ���� ũ�� ����
	GetCapsuleComponent()->InitCapsuleSize(37.f, 90.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("ShooterChara"));//�ݸ��� ����

	// �Է¿� ���� ȸ���� ����
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// ��Ʈ�ѷ��� ȸ�� �� �� ĳ���� ȸ�� ���� ī�޶󿡸� ������ ��Ĩ�ϴ�.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// ĳ���� ������ ����
	GetCharacterMovement()->bOrientRotationToMovement = false; // ĳ���Ͱ� �Է� �������� ȸ���մϴ�	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // �� ȸ�� �ӵ���
	GetCharacterMovement()->JumpZVelocity = 450.0f;//��������
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;// ĳ���� �ɱ� ��� Ȱ��ȭ
	GetCharacterMovement()->bCanWalkOffLedges = true;//�ɾ��� �� �������� �������� ����


	// ī�޶� ���� ����ϴ� (�浹���ִ� ��� �÷��̾ ���� ���)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);//��Ʈ ������Ʈ�� ����
	CameraBoom->TargetArmLength = 200.0f; //ī�޶�� ĳ���� �ڿ����� �Ÿ��� ���󰩴ϴ�.	
	CameraBoom->bUsePawnControlRotation = true; // ��Ʈ�ѷ��� �������� �� ȸ��
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));//ī�޶� �� ��ġ

	// �ȷο� ī�޶� �����
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // ī�޶� �� ���� �����ϰ� ��Ʈ�ѷ� ���⿡ �°� ���� �����մϴ�.
	FollowCamera->bUsePawnControlRotation = false; // ī�޶� ���� �������� ȸ������ �ʽ��ϴ�.
	FollowCamera->SetRelativeLocation(FVector(0.0f, CameraLeftRight, 10.0f));//ī�޶� ��ġ

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -87.0f), FRotator(0.0f, 270.0f, 0.0f));//�� �⺻��ġ ����
	GetCharacterMovement()->CrouchedHalfHeight = 55.0f;//�ɾ��� �� ĸ����������
	GetCharacterMovement()->MaxWalkSpeed = StandSpeed;//ĳ���� ���ǵ� ����

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_Body(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));//ĳ���� ��Ų
	if (SK_Body.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Body.Object);//ĳ���� ��Ų�߰�
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));//�޽ô� �ݸ��� ��Ȱ��
	}

	//�ִϸ��̼� �߰�
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> myAnim(TEXT("/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C"));
	//�ִ��ν��Ͻ� �߰� �������Ʈ class �ҷ����� �ڿ� _C�� �ٿ� Ŭ���������� �������� ��θ� ����
	if (myAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(myAnim.Class);
	}
	
	ImpactParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("ImpactParticle"));//��ź ����Ʈ ��ƼŬ
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

	MissSound = CreateDefaultSubobject<USoundBase>(TEXT("MIss"));//�������� �̽� ����
	static ConstructorHelpers::FObjectFinder<USoundBase>SB_MissSound(TEXT("/Game/MyAsset/SC_Punch_Miss.SC_Punch_Miss"));
	if (SB_MissSound.Succeeded())
	{
		MissSound = SB_MissSound.Object;
	}

	HitSound = CreateDefaultSubobject<USoundBase>(TEXT("Hit"));//���� ���� ��Ʈ ����
	static ConstructorHelpers::FObjectFinder<USoundBase>SB_HitSound(TEXT("/Game/MyAsset/SC_Punch_Hit.SC_Punch_Hit"));
	if (SB_HitSound.Succeeded())
	{
		HitSound = SB_HitSound.Object;
	}

	CharacterStat = CreateDefaultSubobject<UShooterActorComponent>(TEXT("CharacterStat"));//ĳ���� ���� Ŭ����
	CharacterStat->SetCurrentHP(1000.0f);//������ ����
	CharacterStat->SetMaxHP(1000.0f);//���� ����

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));//���� ����
	HPBarWidget->SetupAttachment(GetMesh());//������ �޽��� ����
	HPBarWidget->SetRelativeLocation(FVector(0.0f,0.0f,190.f));//���� ��ġ ����
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);//������ ��ũ���� ����
	static ConstructorHelpers::FClassFinder<UUserWidget>WG_HPBar(TEXT("/Game/MyAsset/WG_HPBar.WG_HPBar_C"));
	if(WG_HPBar.Succeeded())
	{
		HPBarWidget->SetWidgetClass(WG_HPBar.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f,50.0f));
	}

	GrenadeScene = CreateDefaultSubobject<USceneComponent>(TEXT("GrenadeScene"));//����ź�� ������ ������ ��
	GrenadeScene->SetupAttachment(RootComponent);//��Ʈ ������Ʈ�� ����
	GrenadeScene->SetRelativeLocation(FVector(50.0f,0.0f,80.0f));//�� ��ġ ����

	DetectedScene = CreateDefaultSubobject<USceneComponent>(TEXT("DetecteScene"));//AI�� �������� ���� ��
	DetectedScene->SetupAttachment(RootComponent);//��Ʈ ������Ʈ�� ����
	DetectedScene->SetRelativeLocation(FVector(0.0f, 0.0f, 75.0f));//�� ��ġ ����

	VisibleGrenade = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GREANDE"));//����ź ���̷�Ż �޽� �ʱ�ȭ(����ź�� ������ ���־������θ� ��Ÿ��)
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_GRENADE(TEXT("/Game/GrenadeKit/Meshes/MK2_Grenade_SK.MK2_Grenade_SK"));//����ź �޽�
	if (SK_GRENADE.Succeeded())
	{
		VisibleGrenade->SetSkeletalMesh(SK_GRENADE.Object);
		VisibleGrenade->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5));//����ź ũ������
		VisibleGrenade->SetCollisionProfileName(TEXT("NoCollision"));//�ݸ��� ����
		VisibleGrenade->SetHiddenInGame(true, true);//����ź ����
	}
	FName HandSocket(TEXT("my_hand_rSocket"));
	VisibleGrenade->SetupAttachment(GetMesh(), HandSocket);//����ź�� ������ ���Ͽ� ����


	SetActorHiddenInGame(true);//���� ����
	HPBarWidget->SetHiddenInGame(true);//HP ���� ����
	DeadTimer = 3.0f;//����Ÿ�̸� 3��
}

void AShooter2Character::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsPlayerControlled())//�÷��̾ ��Ʈ���� ��
	{
		ShooterPlayerController = Cast < AShooterCharacterPlayerController >(GetController());//�÷��̾� ��Ʈ�ѷ� ����
	}
	SetCharacterState(ECharacterState::READY);//�÷��̾� ĳ���� ���� �غ�Ϸ�
}

void AShooter2Character::SetCharacterState(ECharacterState NewState)//ĳ���� ���� ����
{
	CurrentState = NewState;

	switch (CurrentState)
	{
		case ECharacterState::READY:
		{
			SetActorHiddenInGame(false);//���� ����
			HPBarWidget->SetHiddenInGame(false);//HP ���� ����

			//ĳ���� HUD�� ������ ���ε�(������ ĳ����Ŭ�������� �÷��̾����Ʈ�� ��Ʈ�ѷ�����)
			ShooterPlayerController->GetHUDWidget()->BindCharacterStat(CharacterStat);

			CharacterStat->OnHPIsZero.AddLambda([this]()->void {//HP 0�� ��������Ʈ�� ����Ͽ� DEAD ������Ʈ�� ����
				SetCharacterState(ECharacterState::DEAD);
				});
			break;
		}
		case ECharacterState::DEAD:
		{
			CharacterDead();
			GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
				ShooterPlayerController->ShowResultUI();//��� UIȰ��ȭ
				}), DeadTimer, false);
			break;
		}
	}
}

ECharacterState AShooter2Character::GetCharacterState() const//���� ĳ���� ���� ��ȯ
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
	CameraLeftRight = FMath::FInterpTo(CameraLeftRight, NewCameraLeftRight, DeltaTime, 5.0f);//ī�޶� �ε巴�� ��ȯ
	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, CameraBoomLenth, DeltaTime, 5.0f);//ī�޶���� �ε巴�� ��ȯ
	FollowCamera->SetRelativeLocation(FVector(0.0f, CameraLeftRight, 0.0f));
	
	//�� �̻�� && ���� �̻��� || �޸��� �� && ���� �̻���
	if (!ShooterAnim->IsGunUsed() && !bIsAttacking || ShooterAnim->IsSprinting() && !ShooterAnim->IsAiming())
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;//�̵����������� ĳ����ȸ�� on
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;//�̵����������� ĳ����ȸ�� off
		//���� ĳ���� �����̼��� Ÿ�� ��Ʈ�ѷ� �����̼�����(Yaw�ุ)
		FRotator Rotation = FMath::RInterpTo(GetActorRotation(), GetControlRotation(), DeltaTime, 25.0f);
		SetActorRotation(FRotator(0.0f, Rotation.Yaw, 0.0f));
	}
}

void AShooter2Character::MyJump()
{
	Jump();
	if (ShooterAnim->MyIsCrouching() == true)//�ɾ��� �� ������ �ɱ� ����
	{
		ShooterAnim->SetCrouching(false);
		UnCrouch();
	}
}

void AShooter2Character::MyJumpStop()
{
	StopJumping();
}

void AShooter2Character::Run()//�޸���
{
	if (ShooterAnim->IsAiming() == false)//������ �ƴҰ�쿡 �޸��� �ӵ���
	{
		GetCharacterMovement()->MaxWalkSpeed = StandRunSpeed;
		GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
	}
	ShooterAnim->SetSprinting(true);//�������� Ȱ��ȭ
}
void AShooter2Character::Walk()//�ȱ�
{
	GetCharacterMovement()->MaxWalkSpeed = StandSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
	ShooterAnim->SetSprinting(false);//�������� ��Ȱ��ȭ
}
void AShooter2Character::DoCrouch()
{

	if (CanCrouch() && !GetCharacterMovement()->IsFalling())//�������������� �ɱⰡ��
	{
		Crouch();
		CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));//ī�޶� �� ��ġ
		DetectedScene->SetRelativeLocation(FVector(0.0f, 0.0f, 45.0f));//Ai ������ �� ��ġ����
	}
	else
	{
		UnCrouch();
		CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));//ī�޶� �� ��ġ
		DetectedScene->SetRelativeLocation(FVector(0.0f, 0.0f, 75.0f)); //Ai ������ �� ��ġ����
	}

}

void AShooter2Character::Aim()//���� ��
{
	CameraBoomLenth = 50.0f;//ī�޶� �� ���̼���
	if (ShooterAnim->IsAiming() == false && ShooterAnim->IsGunUsed() == true)//������ �ӵ� ����
	{
		ShooterAnim->SetAiming(true);
		GetCharacterMovement()->MaxWalkSpeed = StandSpeed;
	}
}
void AShooter2Character::Aimoff()//���� ����
{
	CameraBoomLenth = 200.0f;//ī�޶� �ձ��� ����
	if (ShooterAnim->IsAiming() == true )//������ �ӵ� ����
	{
		ShooterAnim->SetAiming(false);
		if (ShooterAnim->IsSprinting() == true)//�޸��⸦ ������ �߿� ������ ������ �޸��� �ӵ���
		{
			GetCharacterMovement()->MaxWalkSpeed = StandRunSpeed;
		}
	}
}

void AShooter2Character::AimChange()//���� �¿� ���� ����
{
	NewCameraLeftRight *= -1.0f;
}

void AShooter2Character::Reload()//������ ��Ÿ�� ����
{
	if (ShooterAnim->IsGunUsed() == true && !ShooterAnim->UAnimInstance::IsAnyMontagePlaying() && CharacterStat->GetMagazineBullet() <30)
	{
		ShooterAnim->PlayReloadMontage();//������ ��Ÿ�� ����
		CurrentGun->PlayReloadSound();//������ ���� ����

		if (CharacterStat->GetMagazineBullet() > 0)//źâ�� �Ѿ��� ������� ��
		{
			int32 NeedToAmmo = 31 - CharacterStat->GetMagazineBullet();

			if (CharacterStat->GetMaxBullet() >= NeedToAmmo && CharacterStat->GetMaxBullet() > 0)//�����Ѿ��� ������ �ʿ��� �Ѿ˼����� ���� ���
			{
				CharacterStat->SetMagazineCount(NeedToAmmo);//�ʿ��� �Ѿ� ����ŭ źâ�� �߰�
				CharacterStat->SetMaxBullet(-NeedToAmmo);//�ʿ��� �Ѿ� ����ŭ �����Ѿ� ����
			}
			else if (CharacterStat->GetMaxBullet() < NeedToAmmo && CharacterStat->GetMaxBullet()>0)//�����Ѿ��� ������ �ʿ��� �Ѿ˼����� ���� ���
			{
				CharacterStat->SetMagazineCount(CharacterStat->GetMaxBullet());//�����ִ� �����Ѿ˼� ��ŭ �߰�
				CharacterStat->SetMaxBulletQuantity(0);//�����Ѿ� ���� 0����
			}
		}
		else if (CharacterStat->GetMagazineBullet() <= 0)//ź�忡 �Ѿ��� ��� ���� ��
		{
			int32 NeedToAmmo = 30;
			if (CharacterStat->GetMaxBullet() >= NeedToAmmo && CharacterStat->GetMaxBullet() > 0)//�����Ѿ��� ������ �ʿ��� �Ѿ˼����� ���� ���
			{
				CharacterStat->SetMagazineCount(NeedToAmmo);//�ʿ��� �Ѿ� ����ŭ źâ�� �߰�
				CharacterStat->SetMaxBullet(-NeedToAmmo);//�ʿ��� �Ѿ� ����ŭ �����Ѿ� ����
			}
			else if (CharacterStat->GetMaxBullet() < NeedToAmmo && CharacterStat->GetMaxBullet() >0)//�����Ѿ��� ������ �ʿ��� �Ѿ˼����� ���� ���
			{
				CharacterStat->SetMagazineCount(CharacterStat->GetMaxBullet());//�����ִ� �����Ѿ˼� ��ŭ �߰�
				CharacterStat->SetMaxBulletQuantity(0);//�����Ѿ� ���� 0����
			}
		}
		CharacterStat->OnAmmoCountChaned.Broadcast();//�Ѿ� �� ������ ȣ��
	}
}

void AShooter2Character::InterAct()//���ö��� �ڽ��� ��ȣ�ۿ�
{
	if (bCanSupply && SupplyBox)
	{
		if (SupplyBox->GetEnableSupply())
		{
			int32 NeedAmmo = 150 - CharacterStat->GetMaxBullet();
			int32 NeedGrenade = 5 - CharacterStat->GetGrenadeCount();

			CharacterStat->SetMaxBullet(NeedAmmo);//�Ѿ� �ִ��
			CharacterStat->SetGrenadeCount(NeedGrenade);//����ź�� �ִ��
			CharacterStat->SetHP(CharacterStat->GetMaxHP());//max ü�� ä���
			SupplyBox->InitCooltimeCount();//���޹ڽ� �ʱ�ȭ
			SupplyBox->SupplySoundPlay();//���޻��� ���
		}
	}
}

void AShooter2Character::Attack()//���� ��Ÿ�� ����
{
	bIsAttacking = true;//������ Ȱ��ȭ
	
	//�� ����� && ��Ÿ�ְ� ���� ������ ���� ��
	if (ShooterAnim->IsGunUsed() == true && !ShooterAnim->UAnimInstance::IsAnyMontagePlaying())
	{
		Fire();//�� �߻�
	}
	else
	{
		ShooterAnim->PlayPunchMontage();//��ġ ��Ÿ�� ����
	}

}

void AShooter2Character::AttackEnd()//������ ������ ��
{
	GetWorld()->GetTimerManager().ClearTimer(FireTimer);//FireTimer ����
	bIsAttacking = false;//������ ��Ȱ��ȭ
}

void AShooter2Character::Fire()//�� �߻�
{
	if (CharacterStat->GetMagazineBullet() > 0)
	{
		if (ShooterAnim->IsSprinting() == true)//�޸��� ���� �� ���� �߻�� �ȱ�ӵ��� 
		{
			Walk();
		}

		CharacterStat->SetMagazineCount(-1);//źâ���� �ѹ� �����鼭 ��ε�ĳ��Ʈ
		CurrentGun->PlayFireSound();//�� �߻� �Ҹ� ���
		CurrentGun->FirePaticle();//�ѱ� �߻� ��ƼŬ ���
		CurrentGun->SingleEject();//�߻�� ����Ʈ ��ƼŬ

		//ī�޶� �ݵ�
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(UFireCameraShake::StaticClass(), 1.f, ECameraAnimPlaySpace::CameraLocal);
	
		//����Ʈ�� �������� ����Ʈ���̽��� ���۰� ���� ������
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			PlayerViewPointLocation,
			PlayerViewPointRotation
		);
		FVector End = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * 10000.0f;//����Ʈ���̽� ���� 
		FVector Start= PlayerViewPointLocation + PlayerViewPointRotation.Vector() * CameraBoomLenth;//����Ʈ���̽� ������

		//////////////////////////////////////////////////////////////// �� �߻� �� ������ ����
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

		if (Result)//��� �ν��Ͻ� ��ȿ�� �� 
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, HitRes.ImpactPoint);//��Ʈ������ �Ѿ� ��ź ��ƼŬ ����
			UGameplayStatics::PlaySoundAtLocation(this, PitanSound, HitRes.ImpactPoint);//��ź �Ҹ����

			FName CollisionName(TEXT("Ragdoll"));//���׵� 
			FName CollisionName2(TEXT("AIChara"));//AIĳ���� 
			
			if (HitRes.Actor.IsValid())//�������� ������� �޸𸮸� ������ �޸� ���� ����
			{
				if (CollisionName == HitRes.GetComponent()->GetCollisionProfileName())//���׵� ���Ӱ� �浹 ����� ������
				{
					// �÷��̾�� Ÿ���� ������ ����
					FVector ImpulseDirection = HitRes.GetActor()->GetActorLocation() - GetActorLocation();//�о��� ���� ����(���� -����)
					ImpulseDirection.Normalize();

					float ImpulseStrength = 5000.0;//�浹 ����
					FVector FinalImpulse = ImpulseDirection * ImpulseStrength;//���޽� ����

					HitRes.GetComponent()->SetPhysicsLinearVelocity(FVector::ZeroVector);
					HitRes.GetComponent()->AddImpulseAtLocation(FinalImpulse, HitRes.GetActor()->GetActorLocation());//�ش���ġ���� ���޽� ��ŭ �о
				}

				else if (CollisionName2 == HitRes.GetComponent()->GetCollisionProfileName())//�ݸ����� AI�϶�
				{
					auto HitAI = Cast<AShooterEnemyCharacter>(HitRes.GetActor());
					HitAI->SetDamagePoint(GetActorLocation());//����ġ�� ��� ���� ������ ����Ʈ�� ����
				}
				FDamageEvent  DamageEvent;

				//������ ����,����� ����,���ݸ�� ������,��������� ����
				HitRes.Actor->TakeDamage(CharacterStat->GetShootDamage(), DamageEvent, GetController(), this);
			}
		}
		//////////////////////////////////////////////////////////////// �� �߻� �� ������ ����

		//////////////////////////////////////////////////////////////// �ݵ�����
		float RandPitch = FMath::RandRange(-1.5f, 0.2f);//�����ݵ�
		float RandYaw = FMath::RandRange(-0.3f, 0.3f);//����ݵ�
		AddControllerPitchInput(RandPitch);//�����ݵ�
		AddControllerYawInput(RandYaw);//����ݵ�	
		////////////////////////////////////////////////////////////////�ݵ�����

		////////////////////////////////////////////////////////////////�� ��׷�
		//�� �߻�� �������� �ȿ��ִ� ������ ����
		UWorld* World = this->GetWorld();
		FVector Center = this->GetActorLocation();
		float DetectRadius = 2000.0f;//Ž������
		if (nullptr == World) return;

		TArray<FOverlapResult> OverLapResults;//TArray �ڷᱸ���� ������ ��� �ν��Ͻ��� ����.
		FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);//�ݸ��� ����

		//������Ʈ Ÿ���� ��Ƽ ������� Ž��(������Ʈ ������� Ž�� : Ž���� ����� �Ǵ� �ݸ������� �˻��� ��ȯ)
		bool AgroResult = World->OverlapMultiByObjectType(
			OverLapResults,
			Center,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel2,//Ž�� ������ ����� Ʈ���̽� ä�� ����
			FCollisionShape::MakeSphere(DetectRadius),//Ž���� ����� �⺻ ���� ����, ��ü, ĸ��, �ڽ��� ���
			CollisionQueryParam//�ݸ��� ����
		);

		if (AgroResult)//AI�Ǵ� �� ��׷� Ȱ��ȭ
		{
			for (auto OverLapResult : OverLapResults)
			{
				if (OverLapResult.GetActor()->GetClass()->IsChildOf(AShooterEnemyCharacter::StaticClass()))//AIĳ������ ��� TrackToTargetOutRangeȣ��
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
		////////////////////////////////////////////////////////////////�� ��׷�

		if (!CurrentGun->GetMode() && bIsAttacking)//�������� �� Ÿ�̸Ӹ� �̿��� ���ȣ���� �Ͽ� ���� ����
		{
			GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &AShooter2Character::Fire, 0.1f, false);
		}
		
	}
	else if (CharacterStat->GetMagazineBullet() <= 0 && CharacterStat->GetMaxBullet() > 0)
	{
		Reload();//��źâ ���ε�
	}
	else if (CharacterStat->GetMagazineBullet() <= 0 && CharacterStat->GetMaxBullet() <= 0)
	{
		CurrentGun->ModeChangeSound->Play();//�Ѿ� ������ ���� �Ҹ�
	}
}

void AShooter2Character::MeleeAttack()//��������
{
	float AttackRange = 50.0f;//�������� ����
	float AttackRadius = 50.0f;//�������� ����

	FHitResult HitRes;//������ �浹�� Ž���� ��� ���õ� ������ ��� ����ü
	FCollisionQueryParams Params(NAME_None, false, this);//�ݸ��� ����

	bool bResult = GetWorld()->SweepSingleByChannel( //���� ����� ���� (ä�� ������α��� : �ش� Ʈ���̽� ä�η� �˻縦 ������ ����� ��ȯ)
		HitRes,//������ �浹�� Ž���� ��� ���õ� ������ ��� ����ü
		GetActorLocation(),//Ž���� ������ ��ġ
		GetActorLocation() + GetActorForwardVector() * AttackRange,//Ž���� ���� ��ġ
		FQuat::Identity,//Ž���� ����� ������ ȸ��
		ECollisionChannel::ECC_GameTraceChannel5,//�����浹 ������ ����� Ʈ���̽� ä�� ����
		FCollisionShape::MakeSphere(AttackRadius),//Ž���� ����� �⺻ ���� ����, ��ü, ĸ��, �ڽ��� ���
		Params);//Ž������� ���� �������� ��Ƶ� ����ü

	if (bResult)
	{
		FName CollisionName(TEXT("Ragdoll"));//���׵� 
		FName CollisionName2(TEXT("AIChara"));//AIĳ���� 

		if (HitRes.Actor.IsValid())//�������� ������� �޸𸮸� �����ϱ� ������ �޸� ���� ����
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());//��Ʈ ���� ���

			if (CollisionName == HitRes.GetComponent()->GetCollisionProfileName())//���׵� ���Ӱ� �浹 ����� ������
			{
				// �÷��̾�� Ÿ���� ������ ����
				FVector ImpulseDirection = HitRes.GetActor()->GetActorLocation() - GetActorLocation();//�о��� ���� ����(���� -����)
				ImpulseDirection.Normalize();

				float ImpulseStrength = 5000.0;//�浹 ����
				FVector FinalImpulse = ImpulseDirection * ImpulseStrength;//���޽� ����

				HitRes.GetComponent()->SetPhysicsLinearVelocity(FVector::ZeroVector);
				HitRes.GetComponent()->AddImpulseAtLocation(FinalImpulse, HitRes.GetActor()->GetActorLocation());//�ش���ġ���� ���޽� ��ŭ �о
			}

			if (CollisionName2 == HitRes.GetComponent()->GetCollisionProfileName())//�ݸ����� AI�϶�
			{
				auto HitAI = Cast<AShooterEnemyCharacter>(HitRes.GetActor());
				HitAI->SetDamagePoint(GetActorLocation());//����ġ�� ��� ���� ������ ����Ʈ�� ����

				FDamageEvent  DamageEvent;

				//������ ����,����� ����,���ݸ�� ������,��������� ����
				HitRes.Actor->TakeDamage(CharacterStat->GetMeleeDamage(), DamageEvent, GetController(), this);
			}
		}
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(this, MissSound, GetActorLocation());//��Ʈ �̽� ���� ���
	}
	
}

float AShooter2Character::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CharacterStat->SetDamage(FinalDamage);//�ǰݽ� �������� ����
	return FinalDamage;
}

void AShooter2Character::CharacterDead()
{
	//SetActorEnableCollision(false);//�ݸ��� ����
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));//ĸ�� �ݸ����� NoCollision����
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));//����� �ݸ����� ���׵��μ���
	GetMesh()->SetSimulatePhysics(true);//���׵� Ȱ��ȭ
	HPBarWidget->SetHiddenInGame(true);//HP ���� ����
	DisableInput(ShooterPlayerController);//�÷��̾� �Է� ��Ȱ��ȭ
	KockBack(2000.0f);//�˹�
}

void AShooter2Character::SetDamagePoint(FVector DamageSource)//�ش���Ϳ� �������� ���� ���� ��ġ ���� (�˹� ���� ������)
{
	DamegePoint = DamageSource;
}

void AShooter2Character::KockBack(float Impulse)
{
	// �÷��̾�� Ÿ���� ������ ����
	FVector ImpulseDirection = GetActorLocation() - DamegePoint;//�о��� ���� ����(���� -����)
	ImpulseDirection.Normalize();

	float ImpulseStrength = Impulse;//�浹 ����
	FVector FinalImpulse = ImpulseDirection * ImpulseStrength;//���޽� ����

	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);//����� �ֱ��� �޽��� �ӵ��� 0���� ����
	GetMesh()->AddImpulseToAllBodiesBelow(FinalImpulse, NAME_None);//�ش���ġ���� ���޽� ��ŭ �о
}

void AShooter2Character::WeaponModeChange()//�� �߻��� ����
{
	if (ShooterAnim->IsGunUsed()==true)
	{
		UE_LOG(Shooter2, Error, TEXT("ModeChange!!"));
		CurrentGun->ModeChange();
	}
}

void AShooter2Character::GunGrap()//���⽺�� ��Ÿ�� ����
{
	ShooterAnim->PlayWeaponSwapMontage();
}

void AShooter2Character::SwapWeapon()//���� ����/����
{
	if (CurrentGun != nullptr)
	{
		if (ShooterAnim->IsGunUsed() == false)
		{
			//���� ��� Ȱ��ȭ
			ShooterAnim->SetGunUsed(true);

			//���⸦ ���� ������ �̵�
			FName WeaponSocket1(TEXT("my_hand_rSocket"));
			CurrentGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			CurrentGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket1);
		}
		else
		{
			//���� ��� ��Ȱ��ȭ
			ShooterAnim->SetGunUsed(false);

			//���⸦ ������ �̵�
			FName WeaponSocket2(TEXT("my_spine_Socket"));
			CurrentGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			CurrentGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket2);
		}
	}
}

void AShooter2Character::SetWeapon(class AShooterGripGun* Weapon)//���� ����
{
	FName WeaponSocket(TEXT("my_spine_Socket"));
	if (nullptr != Weapon)
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		Weapon->SetOwner(this);
		CurrentGun = Weapon;
	}
}

bool AShooter2Character::CanSetWeapon()//���� ���� ���� ����
{
	return (nullptr == CurrentGun);
}

void AShooter2Character::ThrowGrena()//����ź ��ô
{
	if (CharacterStat->GetGrenadeCount() > 0)
	{
		ShooterAnim->PlayThrowGrenade();
	}
}

void AShooter2Character::GrenadeSet()
{
	if (ShooterAnim->IsGunUsed())//���⸦ ������� �� ���⸦ ������ �̵�
	{
		FName WeaponSocket2(TEXT("my_spine_Socket"));
		CurrentGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket2);
	}
	VisibleGrenade->SetHiddenInGame(false, false);//����ź ����
	bUseControllerRotationYaw = true;//ĳ���Ͱ� ī�޶� ȸ���������� ȸ�� On
	if (ShooterAnim->IsSprinting())
	{
		Walk();
	}
}
void AShooter2Character::GrenadeRelease()
{
	const FRotator SpawnRotation = GetControlRotation();
	const FVector SpawnLocation = GrenadeScene->GetComponentLocation();
	VisibleGrenade->SetHiddenInGame(true, true);//����ź ����
	GetWorld()->SpawnActor<AShooterGrenade>(AShooterGrenade::StaticClass(), SpawnLocation, SpawnRotation);//����ź ��ȯ
	CharacterStat->SetGrenadeCount(-1);//����ź 1�� ���� �� ��ε�ĳ��Ʈ
	bUseControllerRotationYaw = false;//ĳ���Ͱ� ī�޶� ȸ���������� ȸ�� Off
	if (ShooterAnim->IsSprinting())
	{
		Run();
	}
}
void AShooter2Character::WeaponSetAfterGrenade()//����ź ��ô�� ���⸦ ��� ���� �� ���⸦ ����
{
	if (ShooterAnim->IsGunUsed())
	{
		FName WeaponSocket1(TEXT("my_hand_rSocket"));
		CurrentGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket1);//���⸦ ������� �� ���⸦ ������ �̵�
	}
	
}

void AShooter2Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ShooterAnim = Cast<UShooterAnimInstance>(GetMesh()->GetAnimInstance());//�ִ� �ν��Ͻ� ����

	ShooterAnim->OnWeaponSwap.AddUObject(this, &AShooter2Character::SwapWeapon);//��Ƽ���� Ȯ�ν� ���⽺�� 
	ShooterAnim->OnAttackCheck.AddUObject(this, &AShooter2Character::MeleeAttack);//�и����� Ȯ��
	ShooterAnim->OnGrenadeStart.AddUObject(this, &AShooter2Character::GrenadeSet);//����ź ����
	ShooterAnim->OnGrenadeEnd.AddUObject(this, &AShooter2Character::GrenadeRelease);//����ź ��
	ShooterAnim->OnWeaponSet.AddUObject(this, &AShooter2Character::WeaponSetAfterGrenade);//����ź ��ô�� ��������
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AShooter2Character::OnSupplyBoxOverLap);//�������� ó���ϴ� ��������Ʈ �Լ�(���ö��� �ڽ�)
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AShooter2Character::OnSupplyBoxEndOverLap);//�������� ó���ϴ� ��������Ʈ �Լ�(���ö��� �ڽ�)

	HPBarWidget->InitWidget();//���� �ʱ�ȭ �ʿ�(�������Ʈ ���� �θ� ���� �ʿ�) 
	auto CharacterWidget = Cast<UShooterCharacterWidget>(HPBarWidget->GetUserWidgetObject());//���� ȣ��
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);//ĳ���� ������ ���� ������Ʈ ���ε�
	}
}

FVector AShooter2Character::GetTargetPoint() const//������ ����Ʈ ��ȯ
{
	return DetectedScene->GetComponentLocation();
}

bool AShooter2Character::GetIsAttacking() const//���������� ���� ��ȯ
{
	return bIsAttacking;
}

void AShooter2Character::OnSupplyBoxOverLap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)//������ �̺�Ʈ�� ó���ϴ� �Լ�(���ö��̹ڽ�)
{
	FName CollisionName(TEXT("Supply"));//���ö��̹ڽ� ����
	if (OtherComp->GetCollisionProfileName() == CollisionName)
	{
		SupplyBox = Cast<ASupplyBox>(OtherActor);//���ö��̹ڽ� ����
		bCanSupply = true;//���ް��� Ȱ��ȭ
	}
}


void AShooter2Character::OnSupplyBoxEndOverLap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)//������ �̺�Ʈ�� ó���ϴ� �Լ�(���ö��̹ڽ�)
{
	FName CollisionName(TEXT("Supply"));//���ö��̹ڽ� ����
	if (OtherComp->GetCollisionProfileName() == CollisionName && SupplyBox)
	{
		SupplyBox = nullptr;//���ö��̹ڽ� ����
		bCanSupply = false;//���ް��� ��Ȱ��ȭ
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
	ShooterAnim->MoveForward = Value;//�ִ� �ν��Ͻ��� ������ �Է� ���� ����
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
	ShooterAnim->MoveRight = Value;//�ִ� �ν��Ͻ��� ������ �Է� ���� ����
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