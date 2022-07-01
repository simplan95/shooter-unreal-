// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "GameFramework/PlayerController.h"
#include "ShooterCharacterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER2_API AShooterCharacterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AShooterCharacterPlayerController();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UShooterHUDWidget> HUDWidgetClass;//클래스 형태의 파일을 불러오기 위한 HUD 위젯 (TSubclassOf)특정 클래스와 상속받은 클래스들로 목록을 제한
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UShooterGamePlayWidget> MenuWidgetClass;// 클래스 형태의 파일을 불러오기위한 선언 UI Widget 선언(Menu)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UShooterGamePlayResultWidget> ResultWidgetClass;// 클래스 형태의 파일을 불러오기위한 선언 UI Widget 선언(Result)

	UPROPERTY()
		class UShooterHUDWidget* HUDWidget;//HUD 위젯 클래스
	UPROPERTY()
		class UShooterGamePlayWidget* MenuWidget;//메뉴 클래스 생성
	UPROPERTY()
		class UShooterGamePlayResultWidget* ResultWidget;//결과 클래스 생성


	UPROPERTY()
		class AShooterPlayerState* ShooterPlayerState;//

protected:
	virtual void SetupInputComponent() override;//빙의한 폰과 관계없는 게임 시스템 관련 기능 pause 기능은 컨트롤러 클래스에서 설정

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* apawn) override;//플레이어 컨트롤러가 폰에 빙의

public:
	void OnGamePause();//게임 퓨즈시

	void AddGameScore() const;//플레이어 스테이트 클래스에서 실행될 메서드이므로 const처리

	void ChangeInputMode(bool bGameMode = true);//인풋 모드 변환

	void ShowResultUI();//결과 UI표시

	class UShooterHUDWidget* GetHUDWidget() const;

	FInputModeGameOnly  GameInputMode;//게임 입력 인스턴스
	FInputModeUIOnly UIInputMode;//UI 입력 인스턴스
	
};
