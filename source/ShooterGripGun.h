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
	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))//���� ������Ʈ
		class USkeletalMeshComponent* Gun;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Paticle, meta = (AllowPrivateAccess = "true"))//�� �߻� ��ƼŬ
		class UParticleSystemComponent* MyFireParticle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Paticle, meta = (AllowPrivateAccess = "true"))//�� ����Ʈ ��ƼŬ
		class UParticleSystemComponent* MyImpactParticle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Paticle, meta = (AllowPrivateAccess = "true"))//�� �߻� ��ƼŬ
		class UParticleSystem* SingleEjectParticle;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))//���� ��
		class USceneComponent* MuzzleScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)//������ ����
		class UAudioComponent* ReloadSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)//�� �߻� ����
		class USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)//�� ��� ü���� ����
		class UAudioComponent* ModeChangeSound;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))//����, �ܹ� ���
		bool bSingleMode;

public:
	void FirePaticle();//�� �߻� ��ƼŬ
	void SingleEject();//ź�� ����
	void PlayReloadSound();//�������Ҹ�
	void PlayFireSound();//�߻�Ҹ�
	bool GetMode();//���� ��� ��ȯ
	void ModeChange();//���� ��� ü����

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
