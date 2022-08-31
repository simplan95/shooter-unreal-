// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterFieldItem.h"

//필드 아이템 Origin 클래스
AShooterFieldItem::AShooterFieldItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	itemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));//스켈레탈 메시
	getSound = CreateDefaultSubobject<USoundBase>(TEXT("GetSound"));//획득 사운드

	RootComponent = itemMesh;//루트컴포넌트 지정

	itemMesh->SetSimulatePhysics(true);//아이템 물리동작 on
	itemMesh->SetCollisionProfileName(TEXT("GunMesh"));//메시 콜리전 지정
	itemMesh->SetGenerateOverlapEvents(true);//건 메시 오버랩 이벤트를 활성화 

}

// Called when the game starts or when spawned
void AShooterFieldItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShooterFieldItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

