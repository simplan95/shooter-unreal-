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

	virtual void PostLogin(APlayerController* newplayer) override;//플레이어가 게임입장시 폰생성 밑 컨트롤러가 해당폰에 possess 하는 함수

	virtual void PostInitializeComponents() override;

	void AddScore(class AShooterCharacterPlayerController* ScoredPlayer);//플레이어 점수 추가

	UPROPERTY()
		class AShooterGameState* ShooterGameState;//게임 스테이트


};



