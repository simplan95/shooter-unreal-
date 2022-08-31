// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "GameFramework/Character.h"
#include "ShooterEnemyCharacter.generated.h"

UCLASS()
class SHOOTER2_API AShooterEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AShooterEnemyCharacter();

	void SetDamagePoint(FVector DamageSource);//������ ���� ����

	void KockBack(float Impulse);//�˹�

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		FVector DamegePoint = FVector(0.0f, 0.0f, 0.0f);//������ ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float DeadTimer;//����Ÿ�̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float WidgetDeadTimer;//��������Ÿ�̸�

	
	FTimerHandle WidgetDeadTimerHandle = {};//��������Ÿ�̸� �ڵ�

	//�ӵ�����
	UPROPERTY()
		float StandSpeed;
	UPROPERTY()
		float StandRunSpeed;


	UPROPERTY(VisibleAnywhere, Category = UI)//���� Ŭ����
		class UWidgetComponent* HPBarWidget;

public:	

	FTimerHandle DeadTimerHandle = {};//����Ÿ�̸� �ڵ�

	UPROPERTY(VisibleAnywhere, Category = Stat)//���� Ŭ����
		class UShooterActorComponent* CharacterStat;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
