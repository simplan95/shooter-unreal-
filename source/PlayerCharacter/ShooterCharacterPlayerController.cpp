// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacterPlayerController.h"
#include "Shooter2Character.h"//캐릭터 클래스
#include "ShooterHUDWidget.h"//HUD위젯
#include "ShooterGamePlayWidget.h"//개임 일시정지화면 위젯
#include "Components/WidgetComponent.h"//UI위젯 헤더파일 
#include "ShooterGamePlayResultWidget.h"//게임 결과화면 위젯
#include "ShooterPlayerState.h"//플레이어스테이트
#include "ShooterGameState.h"//게임 스테이트

//플레이어 컨트롤러
AShooterCharacterPlayerController::AShooterCharacterPlayerController()
{
	
	static ConstructorHelpers::FClassFinder<UShooterHUDWidget>UI_HUD(TEXT("/Game/MyAsset/UI/UI_HUD.UI_HUD_C"));//클래스 파일은 경로뒤 _C 붙임
	if (UI_HUD.Succeeded())
	{
		HUDWidgetClass = UI_HUD.Class;
	}
	
	static ConstructorHelpers::FClassFinder<UShooterGamePlayWidget> UI_Menu_C(TEXT("/Game/MyAsset/UI/UI_Menu.UI_Menu_C"));//Menu Widget 클래스 불러오기
	if (UI_Menu_C.Succeeded())
	{
		MenuWidgetClass = UI_Menu_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UShooterGamePlayResultWidget> UI_Result_C(TEXT("/Game/MyAsset/UI/UI_Result.UI_Result_C"));//Result Widget 클래스 불러오기
	if (UI_Result_C.Succeeded())
	{
		ResultWidgetClass = UI_Result_C.Class;
	}
}

void AShooterCharacterPlayerController::OnPossess(APawn* apawn)
{
	Super::OnPossess(apawn);

	HUDWidget = CreateWidget<UShooterHUDWidget>(this, HUDWidgetClass);//HUD 생성
	HUDWidget->AddToViewport(1);

	ResultWidget = CreateWidget<UShooterGamePlayResultWidget>(this, ResultWidgetClass);//결과 UI 생성
}

void AShooterCharacterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ChangeInputMode(true);//인풋모드 활성화

	ShooterPlayerState = Cast<AShooterPlayerState>(PlayerState);//플레이어 스테이트 설정
	HUDWidget->BindPlayerState(ShooterPlayerState);//플레이어스테이트를 HUD에 바인딩(캐릭터 스탯은 캐릭터에 플레이어 스테이트는 컨트롤러에)
	ShooterPlayerState->OnScoreChanged.Broadcast();//(컨트롤러가 시작되면 OnScoreChanged 브로드캐스트 1회발생)
}

UShooterHUDWidget* AShooterCharacterPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

void AShooterCharacterPlayerController::AddGameScore() const
{
	ShooterPlayerState->AddGameScore();//플레이어 스테이트에서 점수추가 실행
}

void AShooterCharacterPlayerController::SetupInputComponent()//OnGamePause 와 퓨즈 기능 바인딩
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this, &AShooterCharacterPlayerController::OnGamePause);
}

void AShooterCharacterPlayerController::OnGamePause()//게임 퓨즈시
{
	auto GameState = Cast<AShooterGameState>(UGameplayStatics::GetGameState(this));//현재 게임 스테이트 생성

	if (!GameState->IsGameCleared())//게임 클리어가 아닐시
	{
		MenuWidget = CreateWidget<UShooterGamePlayWidget>(this, MenuWidgetClass);//게임 퓨즈 UI 생성
		MenuWidget->AddToViewport(3);

		SetPause(true);//일시정지 On
		ChangeInputMode(false);//UI인풋모드 활성화
	}

}

//인풋모드 변경
void AShooterCharacterPlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		SetInputMode(GameInputMode);//인풋을 게임모드로
		bShowMouseCursor = false;//커서 숨김
	}
	else
	{
		SetInputMode(UIInputMode);//인풋을 UI모드로
		bShowMouseCursor = true;//커서 표시
	}
}

//결과 UI 표시
void AShooterCharacterPlayerController::ShowResultUI()
{
	auto GameState = Cast<AShooterGameState>(UGameplayStatics::GetGameState(this));//현재 게임 스테이트 생성
	ResultWidget->BindGameState(GameState);//게임스테이트와 결과 UI 바인딩

	ResultWidget->AddToViewport(2);
	ChangeInputMode(false);//UI인풋모드 활성화
}