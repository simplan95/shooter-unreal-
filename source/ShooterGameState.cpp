// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameState.h"
#include "EngineUtils.h"//이터레이터
#include "ShooterAICharacter.h"//AI캐릭터
#include "Runtime/Engine/Public/TimerManager.h"//타이머 매니저

//게임 상태 관리(클리어 여부, 스코어)
AShooterGameState::AShooterGameState()//생성자로 게임 스테이트 초기화
{
	TotalGameScore = 0;
	bGameCleared = false;
}

int32 AShooterGameState::GetTotalGameScore() const
{
	return TotalGameScore;
}

void AShooterGameState::AddGameScore()
{
	TotalGameScore++;
}

void AShooterGameState::SetGameCleared()
{
	for (TActorIterator<AShooterAICharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)//월드내 AI캐릭터들을 확인후 타이머 클리어
	{
		GetWorld()->GetTimerManager().ClearTimer(ActorItr->DeadTimerHandle);
	}

	bGameCleared = true;
}

bool AShooterGameState::IsGameCleared() const
{
	return bGameCleared;
}