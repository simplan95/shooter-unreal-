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
		TSubclassOf<class UShooterHUDWidget> HUDWidgetClass;//Ŭ���� ������ ������ �ҷ����� ���� HUD ���� (TSubclassOf)Ư�� Ŭ������ ��ӹ��� Ŭ������� ����� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UShooterGamePlayWidget> MenuWidgetClass;// Ŭ���� ������ ������ �ҷ��������� ���� UI Widget ����(Menu)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UShooterGamePlayResultWidget> ResultWidgetClass;// Ŭ���� ������ ������ �ҷ��������� ���� UI Widget ����(Result)

	UPROPERTY()
		class UShooterHUDWidget* HUDWidget;//HUD ���� Ŭ����
	UPROPERTY()
		class UShooterGamePlayWidget* MenuWidget;//�޴� Ŭ���� ����
	UPROPERTY()
		class UShooterGamePlayResultWidget* ResultWidget;//��� Ŭ���� ����


	UPROPERTY()
		class AShooterPlayerState* ShooterPlayerState;//

protected:
	virtual void SetupInputComponent() override;//������ ���� ������� ���� �ý��� ���� ��� pause ����� ��Ʈ�ѷ� Ŭ�������� ����

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* apawn) override;//�÷��̾� ��Ʈ�ѷ��� ���� ����

public:
	void OnGamePause();//���� ǻ���

	void AddGameScore() const;//�÷��̾� ������Ʈ Ŭ�������� ����� �޼����̹Ƿ� constó��

	void ChangeInputMode(bool bGameMode = true);//��ǲ ��� ��ȯ

	void ShowResultUI();//��� UIǥ��

	class UShooterHUDWidget* GetHUDWidget() const;

	FInputModeGameOnly  GameInputMode;//���� �Է� �ν��Ͻ�
	FInputModeUIOnly UIInputMode;//UI �Է� �ν��Ͻ�
	
};
