// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacterWidget.h"
#include "Components/ProgressBar.h"//프로그래스바 헤더 
#include "ShooterActorComponent.h"//캐릭터 스탯 

//캐릭터 체력바
void UShooterCharacterWidget::BindCharacterStat(class UShooterActorComponent* NewCharacterStat)
{
	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &UShooterCharacterWidget::UpdateHPWidget);//OnHPChanged 브로드캐스트시 호출(HP 변동시)
}


void UShooterCharacterWidget::NativeConstruct()//UI 시스템 준비완료시 호출됨
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));//HP바 인스턴스를 위젯컴포넌트의 PB_HPBar와 바인딩
	UpdateHPWidget();
}

void UShooterCharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());//HP바 조정(CurrentCharacterStat클래스의 HP비율을 가져옴)
		}
	}
}
