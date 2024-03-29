// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "ShooterFieldItem.h"
#include "GameFramework/Actor.h"
#include "ShooterGun.generated.h"

UCLASS()
class SHOOTER2_API AShooterGun : public AShooterFieldItem
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AShooterGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(EditInstanceOnly, Category = MyMesh)//특정 클래스와 상속받은 클래스들로 목록을 한정하는 TSubclassof 키워드 사용
		TSubclassOf<class AShooterGripGun> GunItemClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
		void OnCharacterOverLap(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);//오버랩 이벤트를 처리하는 델리게이트 함수

};
