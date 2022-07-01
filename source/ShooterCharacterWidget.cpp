// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacterWidget.h"
#include "Components/ProgressBar.h"//���α׷����� ��� 
#include "ShooterActorComponent.h"//ĳ���� ���� 

//ĳ���� ü�¹�
void UShooterCharacterWidget::BindCharacterStat(class UShooterActorComponent* NewCharacterStat)
{
	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &UShooterCharacterWidget::UpdateHPWidget);//OnHPChanged ��ε�ĳ��Ʈ�� ȣ��(HP ������)
}


void UShooterCharacterWidget::NativeConstruct()//UI �ý��� �غ�Ϸ�� ȣ���
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));//HP�� �ν��Ͻ��� ����������Ʈ�� PB_HPBar�� ���ε�
	UpdateHPWidget();
}

void UShooterCharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());//HP�� ����(CurrentCharacterStatŬ������ HP������ ������)
		}
	}
}
