// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGamePlayResultWidget.h"
#include "Components/TextBlock.h"//�ؽ�Ʈ ��� 
#include "ShooterGameState.h"//���� ������Ʈ
#include "Components/Button.h"//��ư ������Ʈ 
#include "ShooterCharacterPlayerController.h"//�÷��̾� ��Ʈ�ѷ�


//���ӽ�����Ʈ Ŭ������ ���ε�
void UShooterGamePlayResultWidget::BindGameState(class AShooterGameState* GameState)
{
	CurrentGameState = GameState;
}

//���� ��� ǥ�� UI
void UShooterGamePlayResultWidget::NativeConstruct()
{
	Super::NativeConstruct();


	auto Result = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtResult")));//��� �ؽ�Ʈ

	//CurrentGameState�� bIsCleared �ν��Ͻ��� Ȱ���Ͽ� ��� �ؽ�Ʈ����
	Result->SetText(FText::FromString(CurrentGameState->IsGameCleared() ? TEXT("Misson Complete") : TEXT("Misson Failed")));

	//���ھ� �ؽ�Ʈ
	auto TotalScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTotalScore")));

	//CurrentGameState�� ���� ���ھ� �ν��Ͻ��� ��ȯ
	TotalScore->SetText(FText::FromString(FString::FromInt(CurrentGameState->GetTotalGameScore())));

	//Ÿ��Ʋ ���� ��ư
	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (nullptr != ReturnToTitleButton)
	{
		//��ư Ŭ���� ȣ��Ǵ� ��������Ʈ �Լ�
		ReturnToTitleButton->OnClicked.AddDynamic(this, &UShooterGamePlayResultWidget::OnReturnToTitleClicked);
	}

	//����� ��ư
	RetryGameButton = Cast<UButton>(GetWidgetFromName(TEXT("btnRetryGame")));
	if (nullptr != RetryGameButton)
	{
		//��ư Ŭ���� ȣ��Ǵ� ��������Ʈ �Լ�
		RetryGameButton->OnClicked.AddDynamic(this, &UShooterGamePlayResultWidget::OnRetryGameClicked);
	}
}

void UShooterGamePlayResultWidget::OnReturnToTitleClicked()//Ȩ���� Ŭ����
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));//Ȩ Ÿ��Ʋ ������ ��
}


void UShooterGamePlayResultWidget::OnRetryGameClicked()//����� Ŭ����
{
	auto PlayerController = Cast<AShooterCharacterPlayerController>(GetOwningPlayer());//���� ��Ʈ�ѷ��� ��ȯ
	PlayerController->RestartLevel();//���� �����
}