// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "ShooterAIEnemyController.h"
#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"//������ Ű ��Ÿ�� 
#include "ShooterBossAIController.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EBossState : uint8//���� ��������
{
	BS_FIRE = 0 UMETA(DisplayName = "Fire"),
	BS_MISSILE = 1 UMETA(DisplayName = "Missile"),
	BS_GRENADE = 2 UMETA(DisplayName = "Grenade"),
};

UCLASS()
class SHOOTER2_API AShooterBossAIController : public AShooterAIEnemyController
{
	GENERATED_BODY()

public:
	AShooterBossAIController();

	//����
	virtual void OnPossess(APawn* InPawn) override;

	//ƽ
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY()
		class AShooterBossCharacter* AICharacter;//AI�� �����ϴ� ĳ����
	
public:
	UPROPERTY()
	EBossState BossPattern;//���� ����

	FBlackboard::FKey BossStateBlackBoardKey;//������ Ű�� �����Ϳ��� �������� ����ü

	static const FName AttackPattern;
	bool IsAIRunning() const;//AI�������� Ȯ��
	bool bIsAIRunning = false;//AI ���� ����
};
