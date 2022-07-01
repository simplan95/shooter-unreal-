// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGamePlayResultWidget.h"
#include "Components/TextBlock.h"//텍스트 블록 
#include "ShooterGameState.h"//게임 스테이트
#include "Components/Button.h"//버튼 컴포넌트 
#include "ShooterCharacterPlayerController.h"//플레이어 컨트롤러

//게임 결과 UI
void UShooterGamePlayResultWidget::BindGameState(class AShooterGameState* GameState)//게이스테이트 클래스와 바인딩
{
	CurrentGameState = GameState;
}

void UShooterGamePlayResultWidget::NativeConstruct()
{
	Super::NativeConstruct();


	auto Result = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtResult")));//결과 텍스트
	//CurrentGameState의 bIsCleared 인스턴스를 활용하여 결과 텍스트설정
	Result->SetText(FText::FromString(CurrentGameState->IsGameCleared() ? TEXT("Misson Complete") : TEXT("Misson Failed")));

	auto TotalScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTotalScore")));//스코어 텍스트
	//CurrentGameState의 최종 스코어 인스턴스를 반환
	TotalScore->SetText(FText::FromString(FString::FromInt(CurrentGameState->GetTotalGameScore())));

	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (nullptr != ReturnToTitleButton)
	{
		ReturnToTitleButton->OnClicked.AddDynamic(this, &UShooterGamePlayResultWidget::OnReturnToTitleClicked);//버튼 클릭시 호출되는 델리게이트 함수
	}

	RetryGameButton = Cast<UButton>(GetWidgetFromName(TEXT("btnRetryGame")));
	if (nullptr != RetryGameButton)
	{
		RetryGameButton->OnClicked.AddDynamic(this, &UShooterGamePlayResultWidget::OnRetryGameClicked);//버튼 클릭시 호출되는 델리게이트 함수
	}
}

void UShooterGamePlayResultWidget::OnReturnToTitleClicked()//홈으로 클릭시
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));//홈 타이틀 맵으로 감
}


void UShooterGamePlayResultWidget::OnRetryGameClicked()//재시작 클릭시
{
	auto PlayerController = Cast<AShooterCharacterPlayerController>(GetOwningPlayer());//현재 컨트롤러를 반환
	PlayerController->RestartLevel();//레벨 재시작
}