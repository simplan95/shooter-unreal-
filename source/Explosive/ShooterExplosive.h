// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "GameFramework/Actor.h"
#include "ShooterExplosive.generated.h"

UCLASS()
class SHOOTER2_API AShooterExplosive : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShooterExplosive();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	//��ü �ݸ��� ������Ʈ
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* ExplosionCollisionComp;

	//���� ��ƼŬ
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* ExplosionParticle;

	//���߹� �޽�
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* ExplosionMesh;

	//���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)
		class USoundBase* ExplosionSound;

	float fRange;//���� ����
	float fExplosionDamage;//���� ������

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Explosion();//���߽���

	virtual void knockBack(FOverlapResult HitResult, FVector Origin, float Impulse);//���� �˹�

};
