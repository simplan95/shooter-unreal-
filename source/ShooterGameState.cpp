// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameState.h"
#include "EngineUtils.h"//���ͷ�����
#include "ShooterAICharacter.h"//AIĳ����
#include "Runtime/Engine/Public/TimerManager.h"//Ÿ�̸� �Ŵ���

//���� ���� ����(Ŭ���� ����, ���ھ�)
AShooterGameState::AShooterGameState()//�����ڷ� ���� ������Ʈ �ʱ�ȭ
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
	for (TActorIterator<AShooterAICharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)//���峻 AIĳ���͵��� Ȯ���� Ÿ�̸� Ŭ����
	{
		GetWorld()->GetTimerManager().ClearTimer(ActorItr->DeadTimerHandle);
	}

	bGameCleared = true;
}

bool AShooterGameState::IsGameCleared() const
{
	return bGameCleared;
}