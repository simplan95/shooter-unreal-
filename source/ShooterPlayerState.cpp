// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerState.h"
#include "ShooterSaveGame.h"

//�÷��̾� ���� 
AShooterPlayerState::AShooterPlayerState()//�÷��̾� ������ �ʱ�ȭ
{
	GameScore = 0;
}

int32 AShooterPlayerState::GetGameScore() const//���� ���� ���ھ� ��ȯ
{
	return GameScore;
}

void AShooterPlayerState::InitPlayerData()//�÷��̾� ������  �ʱ�ȭ
{
	GameScore = 0;//���ھ� 0����

}

void AShooterPlayerState::AddGameScore()
{
	GameScore++;
	
	OnScoreChanged.Broadcast();//������ ������ ��ε�ĳ��Ʈ
}

