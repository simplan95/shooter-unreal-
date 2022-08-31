// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "AIController.h"
#include "ShooterAIEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER2_API AShooterAIEnemyController : public AAIController
{
	GENERATED_BODY()
public:
	AShooterAIEnemyController();

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName TargetPosKey;

protected:
	UPROPERTY()
		class UBehaviorTree* BTAsset;//�ൿƮ�� 

	UPROPERTY()
		class UBlackboardData* BBAsset;//������ 

	UPROPERTY()
		class UBlackboardComponent* BlackboardComp;//������ ������Ʈ

	UPROPERTY()
		class AShooter2Character* TargetCharacter;//�÷��̾ �����ϴ� ĳ����

	//����
	virtual void OnPossess(APawn* InPawn) override;

	//ƽ
	virtual void Tick(float DeltaTime) override;

	bool bIsTracking;//�÷��̾� ��������

public:
	void RunAI();//AI ����
	void StopAI();//AI ����

	//Ÿ�� �߰�
	UFUNCTION(BlueprintCallable, Category = Behavior)//BlueprintCallable �������Ʈ ��� �½�ũ���� ȣ�� ����
		void TrackToTarget();

	//Ÿ�� ���� ����
	UFUNCTION(BlueprintCallable, Category = Behavior)
		void StopTrack();

	//������ Ÿ�� ����
	UFUNCTION(BlueprintCallable, Category = Behavior)
		void TrackToTargetOutRange(FVector TargetLocation);
	
};
