// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterEnemyCharacter.h"
#include "ShooterActorComponent.h"//캐릭터 스탯
#include "Components/WidgetComponent.h"//UI위젯 헤더파일 인클루딩
#include "ShooterCharacterWidget.h"//내 캐릭터 위젯

//AI 캐릭터의 부모 클래스
AShooterEnemyCharacter::AShooterEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CharacterStat = CreateDefaultSubobject<UShooterActorComponent>(TEXT("CharacterStat"));//캐릭터 스탯 클래스

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));//위젯 선언
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 190.f));//위젯 위치 설정
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);//위젯을 스크린에 고정
	static ConstructorHelpers::FClassFinder<UUserWidget>WG_HPBar(TEXT("/Game/MyAsset/WG_HPBar.WG_HPBar_C"));
	if (WG_HPBar.Succeeded())
	{
		HPBarWidget->SetWidgetClass(WG_HPBar.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));//위젯 사이즈설정
	}
}

void AShooterEnemyCharacter::SetDamagePoint(FVector DamageSource)
{
	DamegePoint = DamageSource;
}

void AShooterEnemyCharacter::KockBack(float Impulse)
{
	// 플레이어와 타깃의 방향을 설정
	FVector ImpulseDirection = GetActorLocation() - DamegePoint;//밀어질 방향 설정(벡터 -연산)
	ImpulseDirection.Normalize();

	//충돌 세기
	float ImpulseStrength = Impulse;

	FVector FinalImpulse = ImpulseDirection * ImpulseStrength;//임펄스 설정

	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);//충격을 주기전 메시의 속도를 0으로 지정
	GetMesh()->AddImpulseToAllBodiesBelow(FinalImpulse, NAME_None);//해당위치에서 임펄스 만큼 밀어냄
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

