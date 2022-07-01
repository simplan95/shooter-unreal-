// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGamePlayWidget.h"
#include "Components/Button.h"//버튼 컴포넌트 
#include "ShooterCharacterPlayerController.h"

//게임 플레이 중 일시정지 시 표시 될 UI
void UShooterGamePlayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));//게임으로 버튼 인스턴스 초기화
	if (nullptr != ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UShooterGamePlayWidget::OnResumeClicked);//버튼 클릭시 호출되는 델리게이트 함수
	}

	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));//타이틀로 버튼 인스턴스 초기화
	if (nullptr != ReturnToTitleButton)
	{
		ReturnToTitleButton->OnClicked.AddDynamic(this, &UShooterGamePlayWidget::OnReturnToTitleClicked);//버튼 클릭시 호출되는 델리게이트 함수
	}

	RetryGameButton = Cast<UButton>(GetWidgetFromName(TEXT("btnRetryGame")));//재시작 버튼 인스턴스 초기화
	if (nullptr != RetryGameButton)
	{
		RetryGameButton->OnClicked.AddDynamic(this, &UShooterGamePlayWidget::OnRetryGameClicked);//버튼 클릭시 호출되는 델리게이트 함수
	}
}

void UShooterGamePlayWidget::OnResumeClicked()
{
	auto PlayerController = Cast<AShooterCharacterPlayerController>(GetOwningPlayer());

	RemoveFromParent();//뷰포트에 뜬 UI자신을 제거
	PlayerController->ChangeInputMode(true);//게임 인풋 모드
	PlayerController->SetPause(false);//게임 일시정지 해체
}


void UShooterGamePlayWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));//홈 타이틀 맵으로 감
}


void UShooterGamePlayWidget::OnRetryGameClicked()
{
	auto PlayerController = Cast<AShooterCharacterPlayerController>(GetOwningPlayer());//현재 컨트롤러를 반환
	PlayerController->RestartLevel();//레벨 재시작
}