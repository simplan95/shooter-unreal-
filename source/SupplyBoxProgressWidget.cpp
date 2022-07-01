// Fill out your copyright notice in the Description page of Project Settings.


#include "SupplyBoxProgressWidget.h"
#include "Components/ProgressBar.h"//���α׷����� ��� 
#include "SupplyBox.h"//���ö��̹ڽ� 

//�ڽ� ���� ��Ÿ�� ���� ��
void USupplyBoxProgressWidget::BindSupplyBox(class ASupplyBox* NewSupplyBox)
{
	SupplyBox = NewSupplyBox;
	NewSupplyBox->OnCoolTimeProgress.AddUObject(this, &USupplyBoxProgressWidget::UpdatePGWidget);//OnCoolTimeProgress�ν��Ͻ��� ƽ���� ��ε�ĳ��Ʈ
}


void USupplyBoxProgressWidget::NativeConstruct()//UI �ý��� �غ�Ϸ�� ȣ���
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_CoolTime")));//ŧŸ�ӹ��ν��Ͻ��� ���� ������Ʈ ���ε�
	UpdatePGWidget();
	
}

void USupplyBoxProgressWidget::UpdatePGWidget()//�� ƽ���� ��Ÿ�ӹ� ������Ʈ
{
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(SupplyBox->GetCoolDownRatio());//��Ÿ�ӹ� ����
		}
	}
}