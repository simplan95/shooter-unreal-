// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Shooter2.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"
/**
 *
 */
UCLASS()
class SHOOTER2_API AShooterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AShooterAIController();

	//����
	virtual void OnPossess(APawn* InPawn) override;

public:
	//ƽ
	virtual void Tick(float DeltaTime) override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName TargetPosKey;

	void RunAI();//AI ����
	void StopAI();//AI ����

	bool bIsTracking;//����������

private:
	UPROPERTY()
		class UBehaviorTree* BTAsset;//�����̺��Ʈ�� ����

	UPROPERTY()
		class UBlackboardData* BBAsset;//������ ����
	
	UPROPERTY()
		class UBlackboardComponent* BlackboardComp;//������ ������Ʈ

	UPROPERTY()
		class AShooterAICharacter* AICharacter;//AI�� �����ϴ� ĳ����

	UPROPERTY()
		class UShooterAIAnimInstance* AIAnim;//AI�� �����ϴ� ĳ����

	UPROPERTY()
		class AShooter2Character* TargetCharacter;//�÷��̾ �����ϴ� ĳ����



public:
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
