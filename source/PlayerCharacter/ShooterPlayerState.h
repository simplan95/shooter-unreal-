// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "GameFramework/PlayerState.h"
#include "ShooterPlayerState.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnScoreDelegate);//���ھ� ��������Ʈ ���� (��ü����)
UCLASS()
class SHOOTER2_API AShooterPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AShooterPlayerState();

	int32 GetGameScore() const;

	void InitPlayerData();//�÷��̾� ������ �ʱ�ȭ

	void AddGameScore();//���ӽ��ھ� �߰�

	FOnScoreDelegate OnScoreChanged;//���ھ� ���� ��������Ʈ �ν��Ͻ�

private:
	UPROPERTY(Transient)//���ھ� ������ ������ �����Ƿ� Transient Ű���� �߰�
		int32 GameScore;
	
};
