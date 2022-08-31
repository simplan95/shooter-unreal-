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

int32 AShooterGameState::GetTotalGameScore() const//���� ���ھ� ��ȯ
{
	return TotalGameScore;
}

void AShooterGameState::AddGameScore()//���� ���ھ� �߰�
{
	TotalGameScore++;
}

void AShooterGameState::SetGameCleared()//���� Ŭ����
{
	//���峻 AIĳ���͵��� Ȯ���� Ÿ�̸� Ŭ����
	for (TActorIterator<AShooterAICharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		GetWorld()->GetTimerManager().ClearTimer(ActorItr->DeadTimerHandle);
	}

	bGameCleared = true;
}

bool AShooterGameState::IsGameCleared() const
{
	return bGameCleared;
}