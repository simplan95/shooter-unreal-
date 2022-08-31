// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacterPlayerController.h"
#include "Shooter2Character.h"//ĳ���� Ŭ����
#include "ShooterHUDWidget.h"//HUD����
#include "ShooterGamePlayWidget.h"//���� �Ͻ�����ȭ�� ����
#include "Components/WidgetComponent.h"//UI���� ������� 
#include "ShooterGamePlayResultWidget.h"//���� ���ȭ�� ����
#include "ShooterPlayerState.h"//�÷��̾����Ʈ
#include "ShooterGameState.h"//���� ������Ʈ

//�÷��̾� ��Ʈ�ѷ�
AShooterCharacterPlayerController::AShooterCharacterPlayerController()
{
	
	static ConstructorHelpers::FClassFinder<UShooterHUDWidget>UI_HUD(TEXT("/Game/MyAsset/UI/UI_HUD.UI_HUD_C"));//Ŭ���� ������ ��ε� _C ����
	if (UI_HUD.Succeeded())
	{
		HUDWidgetClass = UI_HUD.Class;
	}
	
	static ConstructorHelpers::FClassFinder<UShooterGamePlayWidget> UI_Menu_C(TEXT("/Game/MyAsset/UI/UI_Menu.UI_Menu_C"));//Menu Widget Ŭ���� �ҷ�����
	if (UI_Menu_C.Succeeded())
	{
		MenuWidgetClass = UI_Menu_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UShooterGamePlayResultWidget> UI_Result_C(TEXT("/Game/MyAsset/UI/UI_Result.UI_Result_C"));//Result Widget Ŭ���� �ҷ�����
	if (UI_Result_C.Succeeded())
	{
		ResultWidgetClass = UI_Result_C.Class;
	}
}

void AShooterCharacterPlayerController::OnPossess(APawn* apawn)
{
	Super::OnPossess(apawn);

	HUDWidget = CreateWidget<UShooterHUDWidget>(this, HUDWidgetClass);//HUD ����
	HUDWidget->AddToViewport(1);

	ResultWidget = CreateWidget<UShooterGamePlayResultWidget>(this, ResultWidgetClass);//��� UI ����
}

void AShooterCharacterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ChangeInputMode(true);//��ǲ��� Ȱ��ȭ

	ShooterPlayerState = Cast<AShooterPlayerState>(PlayerState);//�÷��̾� ������Ʈ ����
	HUDWidget->BindPlayerState(ShooterPlayerState);//�÷��̾����Ʈ�� HUD�� ���ε�(ĳ���� ������ ĳ���Ϳ� �÷��̾� ������Ʈ�� ��Ʈ�ѷ���)
	ShooterPlayerState->OnScoreChanged.Broadcast();//(��Ʈ�ѷ��� ���۵Ǹ� OnScoreChanged ��ε�ĳ��Ʈ 1ȸ�߻�)
}

UShooterHUDWidget* AShooterCharacterPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

void AShooterCharacterPlayerController::AddGameScore() const
{
	ShooterPlayerState->AddGameScore();//�÷��̾� ������Ʈ���� �����߰� ����
}

void AShooterCharacterPlayerController::SetupInputComponent()//OnGamePause �� ǻ�� ��� ���ε�
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this, &AShooterCharacterPlayerController::OnGamePause);
}

void AShooterCharacterPlayerController::OnGamePause()//���� ǻ���
{
	auto GameState = Cast<AShooterGameState>(UGameplayStatics::GetGameState(this));//���� ���� ������Ʈ ����

	if (!GameState->IsGameCleared())//���� Ŭ��� �ƴҽ�
	{
		MenuWidget = CreateWidget<UShooterGamePlayWidget>(this, MenuWidgetClass);//���� ǻ�� UI ����
		MenuWidget->AddToViewport(3);

		SetPause(true);//�Ͻ����� On
		ChangeInputMode(false);//UI��ǲ��� Ȱ��ȭ
	}

}

//��ǲ��� ����
void AShooterCharacterPlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		SetInputMode(GameInputMode);//��ǲ�� ���Ӹ���
		bShowMouseCursor = false;//Ŀ�� ����
	}
	else
	{
		SetInputMode(UIInputMode);//��ǲ�� UI����
		bShowMouseCursor = true;//Ŀ�� ǥ��
	}
}

//��� UI ǥ��
void AShooterCharacterPlayerController::ShowResultUI()
{
	auto GameState = Cast<AShooterGameState>(UGameplayStatics::GetGameState(this));//���� ���� ������Ʈ ����
	ResultWidget->BindGameState(GameState);//���ӽ�����Ʈ�� ��� UI ���ε�

	ResultWidget->AddToViewport(2);
	ChangeInputMode(false);//UI��ǲ��� Ȱ��ȭ
}