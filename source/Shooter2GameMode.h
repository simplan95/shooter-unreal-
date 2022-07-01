// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Shooter2.h"
#include "GameFramework/GameModeBase.h"
#include "Shooter2GameMode.generated.h"

UCLASS(minimalapi)
class AShooter2GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AShooter2GameMode();
	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* newplayer) override;//�÷��̾ ��������� ������ �� ��Ʈ�ѷ��� �ش����� possess �ϴ� �Լ�

	virtual void PostInitializeComponents() override;

	void AddScore(class AShooterCharacterPlayerController* ScoredPlayer);//�÷��̾� ���� �߰�

	UPROPERTY()
		class AShooterGameState* ShooterGameState;//���� ������Ʈ


};



