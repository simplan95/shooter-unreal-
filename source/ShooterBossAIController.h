// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
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
class SHOOTER2_API AShooterBossAIController : public AAIController
{
	GENERATED_BODY()

	//����
	virtual void OnPossess(APawn* InPawn) override;

	//ƽ
	virtual void Tick(float DeltaTime) override;

public:
	AShooterBossAIController();
	
private:
	UPROPERTY()
		class UBehaviorTree* BTAsset;//�ൿƮ�� 

	UPROPERTY()
		class UBlackboardData* BBAsset;//������ 

	UPROPERTY()
		class UBlackboardComponent* BlackboardComp;//������ ������Ʈ

	UPROPERTY()
		class AShooterBossCharacter* AICharacter;//AI�� �����ϴ� ĳ����

	UPROPERTY()
		class AShooter2Character* TargetCharacter;//�÷��̾ �����ϴ� ĳ����
	
public:
	//Ÿ�� ����
	UFUNCTION(BlueprintCallable, Category = Behavior)//BlueprintCallable �������Ʈ ��� �½�ũ���� ȣ�� ����
		void TrackToTarget();

	//Ÿ�� ���� ����
	UFUNCTION(BlueprintCallable, Category = Behavior)
		void StopTrack();

	//Ÿ�� ���� ����
	UFUNCTION(BlueprintCallable, Category = Behavior)
		void TrackToTargetOutRange(FVector TargetLocation);

	UPROPERTY()
	EBossState BossPattern;//���� ����

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName TargetPosKey;
	static const FName AttackPattern;

	void RunAI();//AI ����
	void StopAI();//AI ����
	bool IsAIRunning() const;//AI�������� Ȯ��

	bool bIsAIRunning = false;//AI ���� ����
	bool bIsTracking;//�÷��̾� ��������

	FBlackboard::FKey BossStateBlackBoardKey;//������ Ű�� �����Ϳ��� �������� ����ü
};
