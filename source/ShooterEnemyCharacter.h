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
	// Sets default values for this character's properties
	AShooterEnemyCharacter();

	void SetDamagePoint(FVector DamageSource);//데미지 원점 설정

	void KockBack(float Impulse);//넉백

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		FVector DamegePoint = FVector(0.0f, 0.0f, 0.0f);//데미지 원점

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float DeadTimer;//데드타이머
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float WidgetDeadTimer;//위젯데드타이머
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float HP;//초기 및 최대체력 

	FTimerHandle WidgetDeadTimerHandle = {};//위젯데드타이머 핸들

	//속도변수
	UPROPERTY()
		float StandSpeed;
	UPROPERTY()
		float StandRunSpeed;


	UPROPERTY(VisibleAnywhere, Category = UI)//위젯 클래스
		class UWidgetComponent* HPBarWidget;

public:	
	UPROPERTY(VisibleAnywhere, Category = Stat)//스탯 클래스
		class UShooterActorComponent* CharacterStat;

	FTimerHandle DeadTimerHandle = {};//데드타이머 핸들

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
