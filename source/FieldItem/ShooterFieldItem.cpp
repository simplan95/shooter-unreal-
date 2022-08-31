// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterFieldItem.h"

//�ʵ� ������ Origin Ŭ����
AShooterFieldItem::AShooterFieldItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	itemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));//���̷�Ż �޽�
	getSound = CreateDefaultSubobject<USoundBase>(TEXT("GetSound"));//ȹ�� ����

	RootComponent = itemMesh;//��Ʈ������Ʈ ����

	itemMesh->SetSimulatePhysics(true);//������ �������� on
	itemMesh->SetCollisionProfileName(TEXT("GunMesh"));//�޽� �ݸ��� ����
	itemMesh->SetGenerateOverlapEvents(true);//�� �޽� ������ �̺�Ʈ�� Ȱ��ȭ 

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

