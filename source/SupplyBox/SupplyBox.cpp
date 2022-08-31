// Fill out your copyright notice in the Description page of Project Settings.


#include "SupplyBox.h"
#include "Shooter2Character.h"
#include "SupplyBoxProgressWidget.h"//���α׷��� ����
#include "Components/TextRenderComponent.h"//�ؽ�Ʈó��
#include "Components/WidgetComponent.h"//UI���� ������� 
#include "Math/Color.h"
#include "Sound/SoundCue.h"//���� ť

//�÷��̾ ���� ���� �� �ִ� ����
ASupplyBox::ASupplyBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));//Ʈ���� �ʱ�ȭ
	box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));//�ڽ� �ʱ�ȭ

	RootComponent = trigger;//Ʈ���Ÿ� ��Ʈ������Ʈ�� ����
	box->SetupAttachment(RootComponent);//�ڽ��� Ʈ���ſ� ����

	trigger->SetBoxExtent(FVector(80.0f, 70.0f, 70.0f));//Ʈ���� ũ�⼳�� 
	trigger->SetCollisionProfileName(TEXT("Supply"));//Ʈ���� �ݸ�������
	

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("/Game/Military_VOL7_Containers/Meshes/SM_Large_Crate_01a.SM_Large_Crate_01a"));
	if (SM_BOX.Succeeded())
	{
		box->SetStaticMesh(SM_BOX.Object);//�ڽ��޽� ����
		box->SetCollisionProfileName(TEXT("Block"));//Ʈ���� �ݸ�������
		box->SetRelativeLocation(FVector(-65.0f, -50.0f, -70.0f));//�ڽ� ��ġ����
		box->SetGenerateOverlapEvents(true);//�ڽ� �޽� ������ �̺�Ʈ�� Ȱ��ȭ 
	}

	ProgressBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ProgressBar"));//���� ����
	ProgressBarWidget->SetupAttachment(RootComponent);//������ �޽��� ����
	ProgressBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 100.f));//���� ��ġ ����
	ProgressBarWidget->SetWidgetSpace(EWidgetSpace::Screen);//������ ��ũ���� ����
	static ConstructorHelpers::FClassFinder<UUserWidget>WG_PGBar(TEXT("/Game/MyAsset/WG_ProgressBar.WG_ProgressBar_C"));
	if (WG_PGBar.Succeeded())
	{
		ProgressBarWidget->SetWidgetClass(WG_PGBar.Class);//���� ����
		ProgressBarWidget->SetDrawSize(FVector2D(300.0f, 100.0f));//���� ũ�� ����
		ProgressBarWidget->SetHiddenInGame(true);//HP ���� ����
	}

	SupplySound = CreateDefaultSubobject<UAudioComponent>(TEXT("SupplySound"));//���� ����
	SupplySound->SetupAttachment(RootComponent);//Ʈ���ſ� ����
	SupplySound->bAutoActivate = false;//�ڵ����� false
	static ConstructorHelpers::FObjectFinder<USoundCue>SC_SupplySound(TEXT("/Game/MyAsset/SC_Supply.SC_Supply"));
	if (SC_SupplySound.Succeeded())
	{
		SupplySound->SetSound(SC_SupplySound.Object);
	}
}

// Called when the game starts or when spawned
void ASupplyBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASupplyBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	trigger->OnComponentBeginOverlap.AddDynamic(this, &ASupplyBox::OnCharacterOverLap);//������ ���۽� �߻��Ǵ� ��������Ʈ �Լ�
	trigger->OnComponentEndOverlap.AddDynamic(this, &ASupplyBox::OnCharacterEndOverLap);//������ ����� �߻��Ǵ� ��������Ʈ �Լ�

	ProgressBarWidget->InitWidget();//���� �ʱ�ȭ �ʿ�(�������Ʈ ���� �θ� ���� �ʿ�) 
	auto SupplyBoxWidget = Cast<USupplyBoxProgressWidget>(ProgressBarWidget->GetUserWidgetObject());//���� ȣ��
	if (nullptr != ProgressBarWidget)
	{
		SupplyBoxWidget->BindSupplyBox(this);//���޻��� ���α׷����ٸ� ���޻��� ���Ϳ� ���ε�
	}

}
	
void ASupplyBox::InitCooltimeCount()//��Ÿ�� �ʱ�ȭ
{
	fCoolTimeCount = 0.0f;//���� ��Ÿ�� ī��Ʈ�� 0����
	bCoolTimeComplete = false;//���� �Ұ���
}

void ASupplyBox::SupplySoundPlay()//���޽� �Ҹ�Ȱ��ȭ
{
	SupplySound -> Play();
}

bool ASupplyBox::GetEnableSupply() const//���� ���ɿ��� ��ȯ
{
	return bCoolTimeComplete;
}

void ASupplyBox::OnCharacterOverLap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	ProgressBarWidget->SetHiddenInGame(false);//���� ��Ÿ�� bar(����) ǥ��

}

void ASupplyBox::OnCharacterEndOverLap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	ProgressBarWidget->SetHiddenInGame(true);//���� ��Ÿ�� bar(����) ����
}

// Called every frame
void ASupplyBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (fCoolTime >= fCoolTimeCount)//ƽ���� ��Ÿ�� ������Ʈ
	{
		fCoolTimeCount += DeltaTime;
		OnCoolTimeProgress.Broadcast();//OnCoolTimeProgress�� ƽ���� ��ε�ĳ��Ʈ
		bCoolTimeComplete = false;
		
	}
	else if(fCoolTime < fCoolTimeCount)//��Ÿ�� �Ϸ�� ���ް���
	{
		bCoolTimeComplete = true;
	}
}

float ASupplyBox::GetCoolDownRatio()//��Ÿ�� ���α׷��� ��ȯ
{
	return (fCoolTimeCount < KINDA_SMALL_NUMBER) ? 0.0f : (fCoolTimeCount / fCoolTime);
}
