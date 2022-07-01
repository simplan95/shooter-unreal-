// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Shooter2.h"
#include "GameFramework/Character.h"
#include "Shooter2Character.generated.h"

UCLASS(config=Game)
class AShooter2Character : public ACharacter
{
	GENERATED_BODY()

public:
		AShooter2Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** 카메라 붐 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** 카메라 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(EditAnywhere, Category = Grenade)//수류탄이 소환되는 씬
		USceneComponent* GrenadeScene;

	UPROPERTY(EditAnywhere, Category = Grenade)//AI 디텍팅 지점
		USceneComponent* DetectedScene;

	UPROPERTY(VisibleAnywhere, Category = Weapon)//무기 클래스
		class AShooterGripGun* CurrentGun;

	UPROPERTY(VisibleAnywhere, Category = UI)//위젯 클래스
		class UWidgetComponent* HPBarWidget;

	UPROPERTY(VisibleAnywhere, Category = Supply)//보급상자 클래스
		class ASupplyBox* SupplyBox;

private:
	UPROPERTY()
		class UShooterAnimInstance* ShooterAnim;//애님인스턴스 클래스 선언

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		ECharacterState CurrentState;//현재 스테이트 구분

	UPROPERTY()
		class AShooterCharacterPlayerController* ShooterPlayerController;//컨트롤러 클래스 선언 스테이트 확인용

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float DeadTimer;//데드타이머

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float HP;//초기 및 최대체력 

	UPROPERTY()
		FTimerHandle DeadTimerHandle = {};//데드타이머 핸들

	UPROPERTY()
		FTimerHandle FireTimer;//총 연사를 위한 타이머핸들

	UPROPERTY(VisibleAnywhere, Category = MyMesh)
		USkeletalMeshComponent* VisibleGrenade;//수류탄 스켈레탈 매시 (수류탄을 던질떄 비주얼적으로만 나타남)


	UPROPERTY(EditAnywhere, Category = Particle, meta = (AllowPrivateAccess = "true"))//피탄 임팩트
		class UParticleSystem* ImpactParticle;
	UPROPERTY(EditAnywhere, Category = Sound, meta = (AllowPrivateAccess = "true"))//피탄사운드
		class USoundBase* PitanSound;
	UPROPERTY(EditAnywhere, Category = Sound, meta = (AllowPrivateAccess = "true"))//근접공격 미스 사운드
		class USoundBase* MissSound;
	UPROPERTY(EditAnywhere, Category = Sound, meta = (AllowPrivateAccess = "true"))//근접 공격 히트 사운드
		class USoundBase* HitSound;

	//속도변수
	float StandSpeed = 250.0f;
	float StandRunSpeed = 500.0f;
	float CrouchSpeed = 200.0f;

	//기타변수
	bool bIsAttacking = false;
	FVector AimVec = FVector(0.0f, 30.0f, 10.0f);//에임시 카메라 위치
	FVector AimOffVec = FVector(0.0f, 50.0f, 0.0f);//비에임시 카메라 위치
	FVector TargetVec;
	float CameraBoomLenth = 200.0f;
	float CameraLeftRight = 50.0;
	float NewCameraLeftRight = 50.0;
	bool bCanSupply = false;//보급가능여부

	FVector DamegePoint = FVector(0.0f, 0.0f, 0.0f);//데미지를 주는 액터 원점

public:
	UPROPERTY(VisibleAnywhere, Category = Stat)//스탯 클래스
		class UShooterActorComponent* CharacterStat;

	//달리기,걷기,앉기,에임,공격,점프 등 액션키 관련
	void Run();
	void Walk();
	void DoCrouch();
	void Aim();
	void Aimoff();
	void Attack();
	void AttackEnd();
	void MyJump();
	void MyJumpStop();
	void Reload();
	void ThrowGrena();
	void GrenadeSet();
	void GrenadeRelease();
	void WeaponSetAfterGrenade();
	void AimChange();
	void InterAct();


	void MeleeAttack();//노티파이 호출시 공격체크
	void SwapWeapon();//델리게이트 호출시 무기 변경
	void CharacterDead();//HP 0 델리게이트 호출시 사망처리

	void SetCharacterState(ECharacterState NewState);//캐릭터 스테이트 설정(, Ready, Dead)
	ECharacterState GetCharacterState() const;//현재 스테이트 반환

	//무기 관련
	bool CanGetWeapon = true;
	void GunGrap();//무기변경 호출
	bool CanSetWeapon();//무기소유 가능 여부
	void WeaponModeChange();//무기 발사 모드 변경
	void Fire();
	void SetWeapon(class AShooterGripGun* Weapon);//무기장착

	void SetDamagePoint(FVector DamageSource);//데미지 원점 설정
	void KockBack(float Impulse);//넉백
	FVector GetTargetPoint() const;
	bool GetIsAttacking() const;

	UFUNCTION()
		void OnSupplyBoxOverLap(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);//오버랩 이벤트를 처리하는 함수(서플라이박스)

	UFUNCTION()
		void OnSupplyBoxEndOverLap(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);//오버랩 이벤트를 처리하는 함수(서플라이박스)

protected:

	// 틱
	virtual void Tick(float DeltaTime) override;

	//테이크 대미지 함수 오버라이드
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;

	virtual void PostInitializeComponents() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

};

