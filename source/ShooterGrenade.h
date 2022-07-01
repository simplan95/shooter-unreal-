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
	//구체 콜리전 컴포넌트
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* CollisionComp;

	//발사체 파티클
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* ExplosionParticle;

	//발사체 메시
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* ProjMesh;

	//밤사체 움직임 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)//핀뽑기 사운드
		class USoundBase* PinPullSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)//폭발 사운드
		class USoundBase* ExplosionSound;

	FTimerHandle GrenadeTimer;//수류탄 타이머

	void Explosion();//폭발실행
	void knockBack(FOverlapResult HitResult, FVector Origin, float Impulse);//폭발 넉백

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
