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
		class UCapsuleComponent* MissileComp;//�̻��� ķ��������Ʈ

	UPROPERTY(VisibleAnywhere, Category = Missile, Meta = (AllowPrivateAccess = true))//�̻��� �޽�
		UStaticMeshComponent* MissileBody;

	UPROPERTY(VisibleAnywhere, Category = Missile, Meta = (AllowPrivateAccess = true))//�л� �޽�
		UStaticMeshComponent* MissileEject;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "true"))//���� ��ƼŬ
		class UParticleSystem* ExplosionParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)//���� ����
		class USoundBase* ExplosionSound;

	UPROPERTY(VisibleAnywhere, Category = Stat)//���� Ŭ����
		class UShooterActorComponent* CharacterStat;

	UPROPERTY()
		class APawn* TargetPlyaerCharacter;

	UPROPERTY(EditAnywhere)//�ִܰŸ�
		float fMinimalLen = 5000.0f;

	UFUNCTION()//�̻����� ���Ϳ� ��ĥ���
		void MyOnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Explosion();//����

	void MissileTracking();//�̻��� �÷��̾� Ʈ��ŷ

	FVector MoveToward(FVector Start, FVector End, float Alpah);//�̻��� Ʈ��ŷ�� ���� Move�޼���

	void knockBack(FOverlapResult HitResult, FVector Origin, float Impulse);//�˹�

};
