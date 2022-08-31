// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "GameFramework/Actor.h"
#include "SupplyBox.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCoolTimeProgressDelegate);//���α׷�����(��ٿ�) ������Ʈ ��������Ʈ ���� (��ü����)
UCLASS()
class SHOOTER2_API ASupplyBox : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASupplyBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	float GetCoolDownRatio();//��Ÿ�� �� 
	void InitCooltimeCount();//��Ÿ�� �ʱ�ȭ
	void SupplySoundPlay();//���� ���� ���

	bool GetEnableSupply() const;//���� ���ɿ���

	FOnCoolTimeProgressDelegate OnCoolTimeProgress;

protected:
	UPROPERTY(VisibleAnywhere, Category = Box)//������ ���� 
		UBoxComponent* trigger;

	UPROPERTY(VisibleAnywhere, Category = Box)//�ڽ� �޽� 
		UStaticMeshComponent* box;

	UPROPERTY(VisibleAnywhere, Category = UI)//���� Ŭ����
		class UWidgetComponent* ProgressBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)//���� ����
		class UAudioComponent* SupplySound;

	UPROPERTY()
		bool bCoolTimeComplete = false;//��Ÿ�� �ϷῩ��
	UPROPERTY()
		float fCoolTimeCount=0.0;//���� ��Ÿ��
	UPROPERTY()
		float fCoolTime=30.0;//���� ��Ÿ��

protected:
	UFUNCTION()
		void OnCharacterOverLap(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);//������ �̺�Ʈ�� ó���ϴ� ��������Ʈ �Լ�

	UFUNCTION()
		void OnCharacterEndOverLap(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);//������ �̺�Ʈ�� ó���ϴ� ��������Ʈ �Լ�
};
