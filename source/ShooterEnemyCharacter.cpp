// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterEnemyCharacter.h"
#include "ShooterActorComponent.h"//ĳ���� ����
#include "Components/WidgetComponent.h"//UI���� ������� ��Ŭ���
#include "ShooterCharacterWidget.h"//�� ĳ���� ����

//AI ĳ������ �θ� Ŭ����
AShooterEnemyCharacter::AShooterEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CharacterStat = CreateDefaultSubobject<UShooterActorComponent>(TEXT("CharacterStat"));//ĳ���� ���� Ŭ����

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));//���� ����
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 190.f));//���� ��ġ ����
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);//������ ��ũ���� ����
	static ConstructorHelpers::FClassFinder<UUserWidget>WG_HPBar(TEXT("/Game/MyAsset/WG_HPBar.WG_HPBar_C"));
	if (WG_HPBar.Succeeded())
	{
		HPBarWidget->SetWidgetClass(WG_HPBar.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));//���� �������
	}
}

void AShooterEnemyCharacter::SetDamagePoint(FVector DamageSource)
{
	DamegePoint = DamageSource;
}

void AShooterEnemyCharacter::KockBack(float Impulse)
{
	// �÷��̾�� Ÿ���� ������ ����
	FVector ImpulseDirection = GetActorLocation() - DamegePoint;//�о��� ���� ����(���� -����)
	ImpulseDirection.Normalize();

	//�浹 ����
	float ImpulseStrength = Impulse;

	FVector FinalImpulse = ImpulseDirection * ImpulseStrength;//���޽� ����

	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);//����� �ֱ��� �޽��� �ӵ��� 0���� ����
	GetMesh()->AddImpulseToAllBodiesBelow(FinalImpulse, NAME_None);//�ش���ġ���� ���޽� ��ŭ �о
}

// Called when the game starts or when spawned
void AShooterEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShooterEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

