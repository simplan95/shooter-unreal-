// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "GameFramework/PlayerState.h"
#include "ShooterPlayerState.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnScoreDelegate);//스코어 델리게이트 선언 (전체파일)
UCLASS()
class SHOOTER2_API AShooterPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AShooterPlayerState();

	int32 GetGameScore() const;

	void InitPlayerData();//플레이어 데이터 초기화

	void AddGameScore();//게임스코어 추가

	FOnScoreDelegate OnScoreChanged;//스코어 변동 델리게이트 인스턴스

private:
	UPROPERTY(Transient)//스코어 변수의 변동이 잦으므로 Transient 키워드 추가
		int32 GameScore;
	
};
