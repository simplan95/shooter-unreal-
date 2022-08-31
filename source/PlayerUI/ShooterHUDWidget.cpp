// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterHUDWidget.h"
#include "Components/ProgressBar.h"//프로그레스바 컴포넌트 인클루딩
#include "Components/TextBlock.h"//텍스트블록 인클루딩
#include "ShooterActorComponent.h"//캐릭터스탯 컴포넌트
#include "ShooterPlayerState.h"//플레이어 스테이트
#include "ShooterBossCharacter.h"//보스캐릭터

//게임 플레이어 HUD
void UShooterHUDWidget::BindCharacterStat(class UShooterActorComponent* CharacterStat)//액터 컴포넌트 클래스와 바인딩 (캐릭터 클래스에 선언됨)
{
	CurrentCharacterStat = CharacterStat;
	CharacterStat->OnHPChanged.AddUObject(this, &UShooterHUDWidget::UpdateCharacterHPStat);//OnHPChanged 브로드 캐스트시 호출(HP 변동시)
	CharacterStat->OnGrenadeCountChanged.AddUObject(this, &UShooterHUDWidget::UpdateCharacterGrenadeStat);//OnGrenadeCountChanged 브로드 캐스트시 호출(수류탄 수 변동시)
	CharacterStat->OnAmmoCountChaned.AddUObject(this, &UShooterHUDWidget::UpdateCharacterAmmoStat);//OnGrenadeCountChanged 브로드 캐스트시 호출(총알 수 변동시)
}

void UShooterHUDWidget::BindPlayerState(class AShooterPlayerState* PlayerState)//플레이어 스테이트 클래스와 바인딩 (플레이어 컨트롤러에 선언됨)
{
	CurrentPlayerState = PlayerState;
	PlayerState->OnScoreChanged.AddUObject(this, &UShooterHUDWidget::UpdatePlayerScore);//OnScoreChanged 브로드 캐스트시 호출(스코어 변동시)
}

void UShooterHUDWidget::NativeConstruct() //변수와 UI클래스 바인딩
{
	Super::NativeConstruct();

	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHP")));//HPBar 인스턴스를 HUD.Class의 pbHP와 바인딩

	PlayerMagazine = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPlayerMagazine")));//동일 기능

	PlayerAmmo = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPlayerAmmo")));//동일 기능

	PlayerGrenade = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPlayerGrenade")));//동일 기능

	CurrentScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtCurrentScore")));//동일 기능

	DistanceToBoss = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtDistance")));//동일기능

}

void UShooterHUDWidget::UpdateCharacterHPStat()
{
	HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());//위젯의 HPBar 퍼센트를 업데이트
}

void UShooterHUDWidget::UpdateCharacterGrenadeStat()
{
	PlayerGrenade->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetGrenadeCount())));//수류탄 수 업데이트
}

void UShooterHUDWidget::UpdateCharacterAmmoStat()
{
	PlayerMagazine->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetMagazineBullet())));//탄창 총알 수 업데이트
	PlayerAmmo->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetMaxBullet())));//소지 총알 수 업데이트
}

void UShooterHUDWidget::UpdatePlayerScore()
{
	CurrentScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameScore())));//게임 스코어 업데이트
}

void UShooterHUDWidget::GetDistanceToBoss(float Distance)
{
	DistanceToBoss -> SetText(FText::FromString(FString::FromInt(Distance)));
}