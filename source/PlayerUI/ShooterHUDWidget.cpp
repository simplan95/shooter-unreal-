// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterHUDWidget.h"
#include "Components/ProgressBar.h"//���α׷����� ������Ʈ ��Ŭ���
#include "Components/TextBlock.h"//�ؽ�Ʈ��� ��Ŭ���
#include "ShooterActorComponent.h"//ĳ���ͽ��� ������Ʈ
#include "ShooterPlayerState.h"//�÷��̾� ������Ʈ
#include "ShooterBossCharacter.h"//����ĳ����

//���� �÷��̾� HUD
void UShooterHUDWidget::BindCharacterStat(class UShooterActorComponent* CharacterStat)//���� ������Ʈ Ŭ������ ���ε� (ĳ���� Ŭ������ �����)
{
	CurrentCharacterStat = CharacterStat;
	CharacterStat->OnHPChanged.AddUObject(this, &UShooterHUDWidget::UpdateCharacterHPStat);//OnHPChanged ��ε� ĳ��Ʈ�� ȣ��(HP ������)
	CharacterStat->OnGrenadeCountChanged.AddUObject(this, &UShooterHUDWidget::UpdateCharacterGrenadeStat);//OnGrenadeCountChanged ��ε� ĳ��Ʈ�� ȣ��(����ź �� ������)
	CharacterStat->OnAmmoCountChaned.AddUObject(this, &UShooterHUDWidget::UpdateCharacterAmmoStat);//OnGrenadeCountChanged ��ε� ĳ��Ʈ�� ȣ��(�Ѿ� �� ������)
}

void UShooterHUDWidget::BindPlayerState(class AShooterPlayerState* PlayerState)//�÷��̾� ������Ʈ Ŭ������ ���ε� (�÷��̾� ��Ʈ�ѷ��� �����)
{
	CurrentPlayerState = PlayerState;
	PlayerState->OnScoreChanged.AddUObject(this, &UShooterHUDWidget::UpdatePlayerScore);//OnScoreChanged ��ε� ĳ��Ʈ�� ȣ��(���ھ� ������)
}

void UShooterHUDWidget::NativeConstruct() //������ UIŬ���� ���ε�
{
	Super::NativeConstruct();

	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHP")));//HPBar �ν��Ͻ��� HUD.Class�� pbHP�� ���ε�

	PlayerMagazine = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPlayerMagazine")));//���� ���

	PlayerAmmo = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPlayerAmmo")));//���� ���

	PlayerGrenade = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPlayerGrenade")));//���� ���

	CurrentScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtCurrentScore")));//���� ���

	DistanceToBoss = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtDistance")));//���ϱ��

}

void UShooterHUDWidget::UpdateCharacterHPStat()
{
	HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());//������ HPBar �ۼ�Ʈ�� ������Ʈ
}

void UShooterHUDWidget::UpdateCharacterGrenadeStat()
{
	PlayerGrenade->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetGrenadeCount())));//����ź �� ������Ʈ
}

void UShooterHUDWidget::UpdateCharacterAmmoStat()
{
	PlayerMagazine->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetMagazineBullet())));//źâ �Ѿ� �� ������Ʈ
	PlayerAmmo->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetMaxBullet())));//���� �Ѿ� �� ������Ʈ
}

void UShooterHUDWidget::UpdatePlayerScore()
{
	CurrentScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameScore())));//���� ���ھ� ������Ʈ
}

void UShooterHUDWidget::GetDistanceToBoss(float Distance)
{
	DistanceToBoss -> SetText(FText::FromString(FString::FromInt(Distance)));
}