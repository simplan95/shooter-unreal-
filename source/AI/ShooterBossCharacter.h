// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "ShooterEnemyCharacter.h"
#include "GameFramework/Character.h"
#include "ShooterBossCharacter.generated.h"

UCLASS()
class SHOOTER2_API AShooterBossCharacter : public AShooterEnemyCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterBossCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		ECharacterState CurrentState;//현재 스테이트 구분

	UPROPERTY()//AI 컨트롤러
		class AShooterBossAIController* ShooterAIController;

	UPROPERTY(EditAnywhere)//보스 수류탄 스폰 신
		TArray<USceneComponent*> BossGrenadeScene;

	UPROPERTY(EditAnywhere, Category = Mesh)//보스 미사일 스폰신
		USceneComponent* BossMissileScene;

	UPROPERTY(VisibleAnywhere, Category = Weapon)//사용중인 총
		class AShooterGripGun* NewWeapon;

	UPROPERTY(VisibleAnywhere, Category = Missile, Meta = (AllowPrivateAccess = true))//미사일 발사대 몸체 메시
		UStaticMeshComponent* MissileHead;

	UPROPERTY(VisibleAnywhere, Category = Missile, Meta = (AllowPrivateAccess = true))//미사일 발사대 메시
		UStaticMeshComponent* MissileLauncher;

	UPROPERTY(EditAnywhere, Category = Particle, meta = (AllowPrivateAccess = "true"))//피탄 임팩트
		class UParticleSystem* ImpactParticle;
	UPROPERTY(EditAnywhere, Category = Sound, meta = (AllowPrivateAccess = "true"))//피탄사운드
		class USoundBase* PitanSound;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Paticle, meta = (AllowPrivateAccess = "true"))//미사일 발사 파티클
		class UParticleSystemComponent* MissileFireParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)//미사일 발사 사운드
		class USoundBase* MissileFireSound;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//테이크 데미지 함수 오버 라이드
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void PostInitializeComponents() override;

	void SetCharacterState(ECharacterState NewState);//캐릭터 스테이트 설정
	ECharacterState GetCharacterState() const;//캐릭터 스테이트 반환

	void CharacterDead();//HP 0 델리게이트 호출시 사망처리
	void Fire();//총발사
	void Grenade();//수류탄
	void Missile();//미사일
};
