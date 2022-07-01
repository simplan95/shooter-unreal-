// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "ShooterEnemyCharacter.h"
#include "GameFramework/Character.h"
#include "ShooterAICharacter.generated.h"

UCLASS()
class SHOOTER2_API AShooterAICharacter : public AShooterEnemyCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		ECharacterState CurrentState;//현재 스테이트 구분

	UPROPERTY()//AI
		class AShooterAIController* ShooterAIController;//컨트롤러 클래스 선언 스테이트 확인용

	UPROPERTY()
		class UShooterAIAnimInstance* ShooterAnim;//애님인스턴스 클래스 선언

	UPROPERTY(VisibleAnywhere, Category = Weapon)//사용중인 총
		class AShooterGripGun* NewWeapon;

	UPROPERTY(EditAnywhere, Category = Particle, meta = (AllowPrivateAccess = "true"))//피탄 임팩트
		class UParticleSystem* ImpactParticle;
	UPROPERTY(EditAnywhere, Category = Sound, meta = (AllowPrivateAccess = "true"))//피탄사운드
		class USoundBase* PitanSound;

	UPROPERTY(EditAnywhere, Category = Grenade)//AI 디텍팅 지점
		USceneComponent* DetectScene;

	//공격가능 확인
	UPROPERTY()
		bool CanAttack = true;

public:
	//테이크 대미지 함수 오버라이드
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;


	void CharacterDead();//HP 0 델리게이트 호출시 사망처리

	void Run();//달리기속도로
	void Walk();//걷기속도로

	void Fire();//총 발사
	void Reload();//재장전

	void FCanAttack();//공격가능으로 세팅
	void FCannotAttack();//공격불가능으로 세팅
	void SetCharacterState(ECharacterState NewState);//스테이트세팅

	bool GetCanAttack() const;//공격가능 여부 반환
	class UShooterAIAnimInstance* GetAIAnim() const;//애님 인스턴스 반환
	ECharacterState GetCharacterState() const;
	FVector GetDtectPoint() const;//탐지시작신
};
