// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGamePlayWidget.h"
#include "Components/Button.h"//��ư ������Ʈ 
#include "ShooterCharacterPlayerController.h"

//���� �÷��� �� �Ͻ����� �� ǥ�� �� UI
void UShooterGamePlayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));//�������� ��ư �ν��Ͻ� �ʱ�ȭ
	if (nullptr != ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UShooterGamePlayWidget::OnResumeClicked);//��ư Ŭ���� ȣ��Ǵ� ��������Ʈ �Լ�
	}

	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));//Ÿ��Ʋ�� ��ư �ν��Ͻ� �ʱ�ȭ
	if (nullptr != ReturnToTitleButton)
	{
		ReturnToTitleButton->OnClicked.AddDynamic(this, &UShooterGamePlayWidget::OnReturnToTitleClicked);//��ư Ŭ���� ȣ��Ǵ� ��������Ʈ �Լ�
	}

	RetryGameButton = Cast<UButton>(GetWidgetFromName(TEXT("btnRetryGame")));//����� ��ư �ν��Ͻ� �ʱ�ȭ
	if (nullptr != RetryGameButton)
	{
		RetryGameButton->OnClicked.AddDynamic(this, &UShooterGamePlayWidget::OnRetryGameClicked);//��ư Ŭ���� ȣ��Ǵ� ��������Ʈ �Լ�
	}
}

void UShooterGamePlayWidget::OnResumeClicked()
{
	auto PlayerController = Cast<AShooterCharacterPlayerController>(GetOwningPlayer());

	RemoveFromParent();//����Ʈ�� �� UI�ڽ��� ����
	PlayerController->ChangeInputMode(true);//���� ��ǲ ���
	PlayerController->SetPause(false);//���� �Ͻ����� ��ü
}


void UShooterGamePlayWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));//Ȩ Ÿ��Ʋ ������ ��
}


void UShooterGamePlayWidget::OnRetryGameClicked()
{
	auto PlayerController = Cast<AShooterCharacterPlayerController>(GetOwningPlayer());//���� ��Ʈ�ѷ��� ��ȯ
	PlayerController->RestartLevel();//���� �����
}