// Fill out your copyright notice in the Description page of Project Settings.


#include "SupplyBoxProgressWidget.h"
#include "Components/ProgressBar.h"//프로그래스바 헤더 
#include "SupplyBox.h"//서플라이박스 

//박스 보급 쿨타임 진행 바
void USupplyBoxProgressWidget::BindSupplyBox(class ASupplyBox* NewSupplyBox)
{
	SupplyBox = NewSupplyBox;
	NewSupplyBox->OnCoolTimeProgress.AddUObject(this, &USupplyBoxProgressWidget::UpdatePGWidget);//OnCoolTimeProgress인스턴스가 틱마다 브로드캐스트
}


void USupplyBoxProgressWidget::NativeConstruct()//UI 시스템 준비완료시 호출됨
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_CoolTime")));//큘타임바인스턴스와 위젯 컴포넌트 바인딩
	UpdatePGWidget();
	
}

void USupplyBoxProgressWidget::UpdatePGWidget()//매 틱마다 쿨타임바 업데이트
{
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(SupplyBox->GetCoolDownRatio());//쿨타임바 조정
		}
	}
}