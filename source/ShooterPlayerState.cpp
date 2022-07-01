// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerState.h"
#include "ShooterSaveGame.h"

//플레이어 점수 
AShooterPlayerState::AShooterPlayerState()//플레이어 데이터 초기화
{
	GameScore = 0;
}

int32 AShooterPlayerState::GetGameScore() const//현재 게임 스코어 반환
{
	return GameScore;
}

void AShooterPlayerState::InitPlayerData()//플레이어 데이터  초기화
{
	GameScore = 0;//스코어 0으로

}

void AShooterPlayerState::AddGameScore()
{
	GameScore++;
	
	OnScoreChanged.Broadcast();//점수를 얻으면 브로드캐스트
}

