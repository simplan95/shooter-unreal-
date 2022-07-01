// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "GameFramework/Actor.h"
#include "ShooterGrenade.generated.h"

UCLASS()
class SHOOTER2_API AShooterGrenade : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AShooterGrenade();

protected:
	//��ü �ݸ��� ������Ʈ
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* CollisionComp;

	//�߻�ü ��ƼŬ
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* ExplosionParticle;

	//�߻�ü �޽�
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* ProjMesh;

	//���ü ������ ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)//�ɻ̱� ����
		class USoundBase* PinPullSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)//���� ����
		class USoundBase* ExplosionSound;

	FTimerHandle GrenadeTimer;//����ź Ÿ�̸�

	void Explosion();//���߽���
	void knockBack(FOverlapResult HitResult, FVector Origin, float Impulse);//���� �˹�

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
