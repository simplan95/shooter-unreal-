// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "GameFramework/Actor.h"
#include "ShooterGripGun.generated.h"

UCLASS()
class SHOOTER2_API AShooterGripGun : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShooterGripGun();

public:
	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))//무기 컴포넌트
		class USkeletalMeshComponent* Gun;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Paticle, meta = (AllowPrivateAccess = "true"))//총 발사 파티클
		class UParticleSystemComponent* MyFireParticle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Paticle, meta = (AllowPrivateAccess = "true"))//총 임팩트 파티클
		class UParticleSystemComponent* MyImpactParticle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Paticle, meta = (AllowPrivateAccess = "true"))//총 발사 파티클
		class UParticleSystem* SingleEjectParticle;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))//머즐 신
		class USceneComponent* MuzzleScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)//재장전 사운드
		class UAudioComponent* ReloadSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)//총 발사 사운드
		class USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)//총 모드 체인지 사운드
		class UAudioComponent* ModeChangeSound;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))//연사, 단발 모드
		bool bSingleMode;

public:
	void FirePaticle();//총 발사 파티클
	void SingleEject();//탄피 분출
	void PlayReloadSound();//재장전소리
	void PlayFireSound();//발사소리
	bool GetMode();//무기 모드 반환
	void ModeChange();//무기 모드 체인지

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
