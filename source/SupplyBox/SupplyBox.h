// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shooter2.h"
#include "GameFramework/Actor.h"
#include "SupplyBox.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCoolTimeProgressDelegate);//프로그레스바(쿨다운) 업데이트 델리게이트 선언 (전체파일)
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
	float GetCoolDownRatio();//쿨타임 바 
	void InitCooltimeCount();//쿨타임 초기화
	void SupplySoundPlay();//보급 사운드 재생

	bool GetEnableSupply() const;//보급 가능여부

	FOnCoolTimeProgressDelegate OnCoolTimeProgress;

protected:
	UPROPERTY(VisibleAnywhere, Category = Box)//오버랩 영역 
		UBoxComponent* trigger;

	UPROPERTY(VisibleAnywhere, Category = Box)//박스 메시 
		UStaticMeshComponent* box;

	UPROPERTY(VisibleAnywhere, Category = UI)//위젯 클래스
		class UWidgetComponent* ProgressBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = sound)//보급 사운드
		class UAudioComponent* SupplySound;

	UPROPERTY()
		bool bCoolTimeComplete = false;//쿨타임 완료여부
	UPROPERTY()
		float fCoolTimeCount=0.0;//현재 쿨타임
	UPROPERTY()
		float fCoolTime=30.0;//설정 쿨타임

protected:
	UFUNCTION()
		void OnCharacterOverLap(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);//오버랩 이벤트를 처리하는 델리게이트 함수

	UFUNCTION()
		void OnCharacterEndOverLap(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);//오버랩 이벤트를 처리하는 델리게이트 함수
};
