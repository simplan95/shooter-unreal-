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
	//±¸Ã¼ ÄÝ¸®Àü ÄÄÆ÷³ÍÆ®
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* ExplosionCollisionComp;

	//Æø¹ß ÆÄÆ¼Å¬
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* ExplosionParticle;

	//Æø¹ß¹° ¸Þ½Ã
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* ExplosionMesh;

	//Æø¹ß »ç¿îµå
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)
		class USoundBase* ExplosionSound;

	float fRange;//Æø¹ß ¹üÀ§
	float fExplosionDamage;//Æø¹ß µ¥¹ÌÁö

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Explosion();//Æø¹ß½ÇÇà

	virtual void knockBack(FOverlapResult HitResult, FVector Origin, float Impulse);//Æø¹ß ³Ë¹é

};
