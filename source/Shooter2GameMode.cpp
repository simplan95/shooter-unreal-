// Copyright Epic Games, Inc. All Rights Reserved.

#include "Shooter2GameMode.h"
#include "Shooter2Character.h"
#include "UObject/ConstructorHelpers.h"
#include "ShooterCharacterPlayerController.h"//내플레이어 컨트롤러 
#include "Blueprint/UserWidget.h"
#include "ShooterPlayerState.h"//플레이어 스테이트
#include "ShooterGameState.h"//게임스테이트
#include "ShooterCharacterPlayerController.h"// 플레이어 컨트롤러 추가로 플레이어 스테이트도 포함

//게임모드 설정
AShooter2GameMode::AShooter2GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PlayerControllerClass = AShooterCharacterPlayerController::StaticClass();//게임모드 설정을 내 플레이어 컨트롤러로 설정
	PlayerStateClass = AShooterPlayerState::StaticClass();//플레이어 스테이트 설정
	GameStateClass = AShooterGameState::StaticClass();//게임 스테이트 설정
}

void AShooter2GameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ShooterGameState = Cast<AShooterGameState>(GameState);//게임 스테이트 할당
}

void AShooter2GameMode::AddScore(class AShooterCharacterPlayerController* ScoredPlayer)//(적 캐릭터에서 적이 죽으면 호출 )
{
	//점수를 얻으면 이터레이터를 이용하여 플레이어 컨트롤러를 찾아 점수를 추가
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		const auto ShooterPlayer = Cast<AShooterCharacterPlayerController>(It->Get());
		if ((nullptr != ShooterPlayer) && (ScoredPlayer == ShooterPlayer))
		{
			ShooterPlayer->AddGameScore();//적 처치시 해당플레이어의 점수를 추가
			break;
		}
	}
	ShooterGameState->AddGameScore();//적 처치시 게임스테이트의 점수를 추가

}

void AShooter2GameMode::PostLogin(APlayerController* newplayer)
{
	Super::PostLogin(newplayer);

	auto ABPlayerState = Cast<AShooterPlayerState>(newplayer->PlayerState);//플레이어 스테이트 생성 및 초기선언
	ABPlayerState->InitPlayerData();//플레이어 점수 초기화
}

void AShooter2GameMode::BeginPlay() 
{
	Super::BeginPlay(); 
}




