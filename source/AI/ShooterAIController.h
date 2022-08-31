// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Shooter2.h"
#include "ShooterAIEnemyController.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"
/**
 *
 */
UCLASS()
class SHOOTER2_API AShooterAIController : public AShooterAIEnemyController
{
	GENERATED_BODY()

public:
	AShooterAIController();

	//빙의
	virtual void OnPossess(APawn* InPawn) override;

	//틱
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY()
		class AShooterAICharacter* AICharacter;//AI가 조종하는 캐릭터

	UPROPERTY()
		class UShooterAIAnimInstance* AIAnim;//AI가 조종하는 캐릭터 애님인스턴스

};
