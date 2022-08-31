// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "ShooterExplosive.h"//폭발 Origin 클래스
#include "GameFramework/Actor.h"
#include "ShooterGrenade.generated.h"

UCLASS()
class SHOOTER2_API AShooterGrenade : public AShooterExplosive
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AShooterGrenade();

protected:

	//밤사체 움직임 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

	FTimerHandle GrenadeTimer;//수류탄 타이머

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
