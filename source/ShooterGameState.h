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
	int32 GetTotalGameScore() const;//최종 게임스코어 반환
	void AddGameScore();//게임스코어 추가

	void SetGameCleared();//게임 완료시 호출
	bool IsGameCleared() const;//게임클리어 여부 반환

private:
	UPROPERTY(Transient)
		int32 TotalGameScore;

	UPROPERTY(Transient)
		bool bGameCleared;//게임 완료여부 확인 변수
};
