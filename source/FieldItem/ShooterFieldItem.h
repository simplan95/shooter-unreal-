// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "GameFramework/Actor.h"
#include "ShooterFieldItem.generated.h"

UCLASS()
class SHOOTER2_API AShooterFieldItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShooterFieldItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = MyMesh)//매시 영역
		USkeletalMeshComponent* itemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)//아이템 get sound
		class USoundBase* getSound;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
