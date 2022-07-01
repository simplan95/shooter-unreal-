// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "GameFramework/GameStateBase.h"
#include "ShooterGameState.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER2_API AShooterGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AShooterGameState();

public:
	int32 GetTotalGameScore() const;//���� ���ӽ��ھ� ��ȯ
	void AddGameScore();//���ӽ��ھ� �߰�

	void SetGameCleared();//���� �Ϸ�� ȣ��
	bool IsGameCleared() const;//����Ŭ���� ���� ��ȯ

private:
	UPROPERTY(Transient)
		int32 TotalGameScore;

	UPROPERTY(Transient)
		bool bGameCleared;//���� �ϷῩ�� Ȯ�� ����
};
