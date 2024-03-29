// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "ShooterFieldItem.h"
#include "GameFramework/Actor.h"
#include "ShooterGripGrenade.generated.h"

UCLASS()
class SHOOTER2_API AShooterGripGrenade : public AShooterFieldItem
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AShooterGripGrenade();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//무기 초기 설정
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditInstanceOnly, Category = MyMesh)//특정 클래스와 상속받은 클래스들로 목록을 한정하는 TSubclassof 키워드 사용
		TSubclassOf<class AShooterGripGrenade> GrenadeItemClass;

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
