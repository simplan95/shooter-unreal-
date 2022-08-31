// Copyright Epic Games, Inc. All Rights Reserved.

#include "Shooter2GameMode.h"
#include "Shooter2Character.h"
#include "UObject/ConstructorHelpers.h"
#include "ShooterCharacterPlayerController.h"//���÷��̾� ��Ʈ�ѷ� 
#include "Blueprint/UserWidget.h"
#include "ShooterPlayerState.h"//�÷��̾� ������Ʈ
#include "ShooterGameState.h"//���ӽ�����Ʈ
#include "ShooterCharacterPlayerController.h"// �÷��̾� ��Ʈ�ѷ� �߰��� �÷��̾� ������Ʈ�� ����

//���Ӹ�� ����
AShooter2GameMode::AShooter2GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PlayerControllerClass = AShooterCharacterPlayerController::StaticClass();//���Ӹ�� ������ �� �÷��̾� ��Ʈ�ѷ��� ����
	PlayerStateClass = AShooterPlayerState::StaticClass();//�÷��̾� ������Ʈ ����
	GameStateClass = AShooterGameState::StaticClass();//���� ������Ʈ ����
}

void AShooter2GameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ShooterGameState = Cast<AShooterGameState>(GameState);//���� ������Ʈ �Ҵ�
}

void AShooter2GameMode::AddScore(class AShooterCharacterPlayerController* ScoredPlayer)//(�� ĳ���Ϳ��� ���� ������ ȣ�� )
{
	//������ ������ ���ͷ����͸� �̿��Ͽ� �÷��̾� ��Ʈ�ѷ��� ã�� ������ �߰�
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		const auto ShooterPlayer = Cast<AShooterCharacterPlayerController>(It->Get());
		if ((nullptr != ShooterPlayer) && (ScoredPlayer == ShooterPlayer))
		{
			ShooterPlayer->AddGameScore();//�� óġ�� �ش��÷��̾��� ������ �߰�
			break;
		}
	}
	ShooterGameState->AddGameScore();//�� óġ�� ���ӽ�����Ʈ�� ������ �߰�

}

void AShooter2GameMode::PostLogin(APlayerController* newplayer)
{
	Super::PostLogin(newplayer);

	auto ABPlayerState = Cast<AShooterPlayerState>(newplayer->PlayerState);//�÷��̾� ������Ʈ ���� �� �ʱ⼱��
	ABPlayerState->InitPlayerData();//�÷��̾� ���� �ʱ�ȭ
}

void AShooter2GameMode::BeginPlay() 
{
	Super::BeginPlay(); 
}




