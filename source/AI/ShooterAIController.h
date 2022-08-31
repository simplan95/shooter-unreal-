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

	//����
	virtual void OnPossess(APawn* InPawn) override;

	//ƽ
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY()
		class AShooterAICharacter* AICharacter;//AI�� �����ϴ� ĳ����

	UPROPERTY()
		class UShooterAIAnimInstance* AIAnim;//AI�� �����ϴ� ĳ���� �ִ��ν��Ͻ�

};
