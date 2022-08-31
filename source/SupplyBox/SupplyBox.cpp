// Fill out your copyright notice in the Description page of Project Settings.


#include "SupplyBox.h"
#include "Shooter2Character.h"
#include "SupplyBoxProgressWidget.h"//프로그레스 위젯
#include "Components/TextRenderComponent.h"//텍스트처리
#include "Components/WidgetComponent.h"//UI위젯 헤더파일 
#include "Math/Color.h"
#include "Sound/SoundCue.h"//사운드 큐

//플레이어가 보급 받을 수 있는 상자
ASupplyBox::ASupplyBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));//트리거 초기화
	box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));//박스 초기화

	RootComponent = trigger;//트리거를 루트컴포넌트로 지정
	box->SetupAttachment(RootComponent);//박스를 트리거에 부착

	trigger->SetBoxExtent(FVector(80.0f, 70.0f, 70.0f));//트리거 크기설정 
	trigger->SetCollisionProfileName(TEXT("Supply"));//트리거 콜리전네임
	

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("/Game/Military_VOL7_Containers/Meshes/SM_Large_Crate_01a.SM_Large_Crate_01a"));
	if (SM_BOX.Succeeded())
	{
		box->SetStaticMesh(SM_BOX.Object);//박스메시 설정
		box->SetCollisionProfileName(TEXT("Block"));//트리거 콜리전네임
		box->SetRelativeLocation(FVector(-65.0f, -50.0f, -70.0f));//박스 위치조정
		box->SetGenerateOverlapEvents(true);//박스 메시 오버랩 이벤트를 활성화 
	}

	ProgressBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ProgressBar"));//위젯 선언
	ProgressBarWidget->SetupAttachment(RootComponent);//위젯을 메쉬에 부착
	ProgressBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 100.f));//위젯 위치 설정
	ProgressBarWidget->SetWidgetSpace(EWidgetSpace::Screen);//위젯을 스크린에 고정
	static ConstructorHelpers::FClassFinder<UUserWidget>WG_PGBar(TEXT("/Game/MyAsset/WG_ProgressBar.WG_ProgressBar_C"));
	if (WG_PGBar.Succeeded())
	{
		ProgressBarWidget->SetWidgetClass(WG_PGBar.Class);//위젯 설정
		ProgressBarWidget->SetDrawSize(FVector2D(300.0f, 100.0f));//위젝 크기 설정
		ProgressBarWidget->SetHiddenInGame(true);//HP 위젯 숨김
	}

	SupplySound = CreateDefaultSubobject<UAudioComponent>(TEXT("SupplySound"));//보급 사운드
	SupplySound->SetupAttachment(RootComponent);//트리거에 부착
	SupplySound->bAutoActivate = false;//자동실행 false
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
	trigger->OnComponentBeginOverlap.AddDynamic(this, &ASupplyBox::OnCharacterOverLap);//오버랩 시작시 발생되는 델리게이트 함수
	trigger->OnComponentEndOverlap.AddDynamic(this, &ASupplyBox::OnCharacterEndOverLap);//오버랩 종료시 발생되는 델리게이트 함수

	ProgressBarWidget->InitWidget();//위젯 초기화 필요(블루프린트 위젯 부모도 변경 필요) 
	auto SupplyBoxWidget = Cast<USupplyBoxProgressWidget>(ProgressBarWidget->GetUserWidgetObject());//위젯 호출
	if (nullptr != ProgressBarWidget)
	{
		SupplyBoxWidget->BindSupplyBox(this);//보급상자 프로그레스바를 보급상자 액터와 바인딩
	}

}
	
void ASupplyBox::InitCooltimeCount()//쿨타임 초기화
{
	fCoolTimeCount = 0.0f;//현재 쿨타임 카운트를 0으로
	bCoolTimeComplete = false;//보급 불가능
}

void ASupplyBox::SupplySoundPlay()//보급시 소리활성화
{
	SupplySound -> Play();
}

bool ASupplyBox::GetEnableSupply() const//보급 가능여부 반환
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
	ProgressBarWidget->SetHiddenInGame(false);//보급 쿨타임 bar(위젯) 표시

}

void ASupplyBox::OnCharacterEndOverLap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	ProgressBarWidget->SetHiddenInGame(true);//보급 쿨타임 bar(위젯) 숨김
}

// Called every frame
void ASupplyBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (fCoolTime >= fCoolTimeCount)//틱마다 쿨타임 업데이트
	{
		fCoolTimeCount += DeltaTime;
		OnCoolTimeProgress.Broadcast();//OnCoolTimeProgress가 틱마다 브로드캐스트
		bCoolTimeComplete = false;
		
	}
	else if(fCoolTime < fCoolTimeCount)//쿨타임 완료시 보급가능
	{
		bCoolTimeComplete = true;
	}
}

float ASupplyBox::GetCoolDownRatio()//쿨타임 프로그레스 반환
{
	return (fCoolTimeCount < KINDA_SMALL_NUMBER) ? 0.0f : (fCoolTimeCount / fCoolTime);
}
