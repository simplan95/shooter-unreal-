// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "GameFramework/Actor.h"
#include "ShooterGun.generated.h"

UCLASS()
class SHOOTER2_API AShooterGun : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AShooterGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//���� �ʱ� ����
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = MyMesh)//�Ž� ����
		USkeletalMeshComponent* Gun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)//�� �ݴ� �Ҹ�
		class USoundBase* GetSound;

	UPROPERTY(EditInstanceOnly, Category = MyMesh)//Ư�� Ŭ������ ��ӹ��� Ŭ������� ����� �����ϴ� TSubclassof Ű���� ���
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
		);//������ �̺�Ʈ�� ó���ϴ� ��������Ʈ �Լ�

};
