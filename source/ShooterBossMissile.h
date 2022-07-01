// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "GameFramework/Actor.h"
#include "ShooterBossMissile.generated.h"

UCLASS()
class SHOOTER2_API AShooterBossMissile : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AShooterBossMissile();
	
protected:
	UPROPERTY()
		class UCapsuleComponent* MissileComp;//미사일 캠슐컴포넌트

	UPROPERTY(VisibleAnywhere, Category = Missile, Meta = (AllowPrivateAccess = true))//미사일 메시
		UStaticMeshComponent* MissileBody;

	UPROPERTY(VisibleAnywhere, Category = Missile, Meta = (AllowPrivateAccess = true))//분사 메시
		UStaticMeshComponent* MissileEject;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "true"))//폭발 파티클
		class UParticleSystem* ExplosionParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)//폭발 사운드
		class USoundBase* ExplosionSound;

	UPROPERTY(VisibleAnywhere, Category = Stat)//스탯 클래스
		class UShooterActorComponent* CharacterStat;

	UPROPERTY()
		class APawn* TargetPlyaerCharacter;

	UPROPERTY(EditAnywhere)//최단거리
		float fMinimalLen = 5000.0f;

	UFUNCTION()//미사일이 액터와 겹칠경우
		void MyOnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Explosion();//폭파

	void MissileTracking();//미사일 플레이어 트랙킹

	FVector MoveToward(FVector Start, FVector End, float Alpah);//미사일 트랙킹을 위한 Move메서드

	void knockBack(FOverlapResult HitResult, FVector Origin, float Impulse);//넉백

};
