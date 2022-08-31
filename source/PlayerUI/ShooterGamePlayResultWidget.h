// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "Blueprint/UserWidget.h"
#include "ShooterGamePlayResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER2_API UShooterGamePlayResultWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindGameState(class AShooterGameState* GameState);//게임 스테이트 클래스와 바인딩

protected:
	virtual void NativeConstruct() override;

private:
	TWeakObjectPtr<class AShooterGameState> CurrentGameState;//게임스테이르 클래스 선언 NativeConstruct함수는 AddViewport 함수가 외부에서 호출 되면 UI 위젯이 초기화되어 호출되기 때문에
														//미리 게임스테이트에서 정보를 받은후 호출하기 위함

	UPROPERTY()
		class UButton* ReturnToTitleButton;//타이틀로로

	UPROPERTY()
		class UButton* RetryGameButton;//재시작

	UFUNCTION()
		void OnReturnToTitleClicked();//타이틀로 클릭시

	UFUNCTION()
		void OnRetryGameClicked();//재시작 클릭시
};
