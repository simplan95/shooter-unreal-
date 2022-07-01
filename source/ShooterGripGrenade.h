// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "GameFramework/Actor.h"
#include "ShooterGripGrenade.generated.h"

UCLASS()
class SHOOTER2_API AShooterGripGrenade : public AActor
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

	//���� �ʱ� ����
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = MyMesh)//�Ž� ����
		USkeletalMeshComponent* Grenade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)//����ź �ݴ� �Ҹ�
		class USoundBase* GetSound;

	UPROPERTY(EditInstanceOnly, Category = MyMesh)//Ư�� Ŭ������ ��ӹ��� Ŭ������� ����� �����ϴ� TSubclassof Ű���� ���
		TSubclassOf<class AShooterGripGrenade> GrenadeItemClass;

private:
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
