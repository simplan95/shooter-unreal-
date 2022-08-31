// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterExplosive.h"
#include "Shooter2Character.h"//캐릭터
#include "DrawDebugHelpers.h"//디버그 그리기
#include "ShooterEnemyCharacter.h"//에너미 캐릭터

//폭발 오브젝트 Origin 클래스
AShooterExplosive::AShooterExplosive()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	fRange = 350.0f;//폭발 범위
	fExplosionDamage = 100.0f;//폭발 데미지

}

// Called when the game starts or when spawned
void AShooterExplosive::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShooterExplosive::Explosion()
{
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());//폭파사운드 활성화
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionParticle, GetActorLocation());//폭파이펙트 활성화

	FVector Center = GetActorLocation();//폭파지점
	TArray<FOverlapResult> OverLapResults;//TArray 자료구조로 오버랩 결과 인스턴스를 저장한다.
	FCollisionObjectQueryParams ObjQuery;//공격 받는 인스턴스 집합(배열)
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);//실행 주체 액터는 공격 무시

	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel1);// 공격받는 트레이스 채널 설정 추가 (캐릭터)
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel2);// 공격받는 트레이스 채널 설정 추가 (AI)
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel3);// 공격받는 트레이스 채널 설정 추가 (GunMesh)
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel6);// 공격받는 트레이스 채널 설정 추가 (AIMissile)
	ObjQuery.AddObjectTypesToQuery(ECC_PhysicsBody);//피직스바디 추가
	

	bool bResult = GetWorld()->OverlapMultiByObjectType(//오브젝트 타입을 멀티 방식으로 탐지(생성할 지역에 충돌할 개체들의 채널을 설정하여 탐색)
		OverLapResults,
		Center,
		FQuat::Identity,
		ObjQuery,//탐색 감지에 사용할 트레이스 채널 정보
		FCollisionShape::MakeSphere(fRange),//탐색에 사용할 기본 도형 정보, 구체, 캡슐, 박스를 사용
		CollisionQueryParam//
	);

	if (bResult)//유효값을 반환할 시
	{
		FName CollisionName(TEXT("Ragdoll"));//래그돌 네임
		FName CollisionName2(TEXT("GunMesh"));//건메시 네임
		FName CollisionName3(TEXT("AIChara"));//AI 캐릭터 네임
		FName CollisionName4(TEXT("ShooterChara"));//슈터캐릭 네임
		FName CollisionName5(TEXT("AIMissile"));//AI 미사일 네임

		for (auto OverLapResult : OverLapResults)//for문 처리로 각각의 액터를 반환
		{
			FCollisionObjectQueryParams ObjQueryLine;//공격 받는 인스턴스 집합(배열)
			ObjQueryLine.AddObjectTypesToQuery(ECC_GameTraceChannel4);// 공격받는 트레이스 채널 설정 추가 (블록)

			FCollisionQueryParams ColQuery;//실행 주체 인스턴스
			ColQuery.AddIgnoredActor(this);//실행 주체 액터는 공격 무시

			FHitResult HitRes;//히트 확인 인스턴스

			bool IsBlocked = false;
			//라인 트레이스로 히트확인검사
			IsBlocked = GetWorld()->LineTraceSingleByObjectType(HitRes, Center, OverLapResult.GetActor()->GetActorLocation(), ObjQueryLine, ColQuery);

			if (!IsBlocked)//(!HitResult)//(HitRes.bBlockingHit)//라인트레이스를 했을 경우 그사이에 블록 요소가 없을 시 
			{
				if (OverLapResult.Actor.IsValid())
				{
					if (CollisionName == OverLapResult.GetComponent()->GetCollisionProfileName())//래그돌 네임과 충돌 결과가 같을시
					{
						knockBack(OverLapResult, Center, 50000.0f);//액터 넉백처리
					}
					else if (CollisionName2 == OverLapResult.GetComponent()->GetCollisionProfileName())//건메시 네임과 충돌 결과가 같을시
					{
						knockBack(OverLapResult, Center, 1500.0f);//액터 넉백처리
					}
					else if (CollisionName3 == OverLapResult.GetComponent()->GetCollisionProfileName())//콜리전이 AI일때
					{
						auto HitAI = Cast<AShooterEnemyCharacter>(OverLapResult.GetActor());
						HitAI->SetDamagePoint(GetActorLocation());//수류탄 위치를 상대 액터 데미지 포인트에 지정

					}
					else if (CollisionName4 == OverLapResult.GetComponent()->GetCollisionProfileName())//콜리전이 플레이어 캐릭터일때
					{
						auto HitChara = Cast<AShooter2Character>(OverLapResult.GetActor());
						HitChara->SetDamagePoint(GetActorLocation());//수류탄 위치를 상대 액터 데미지 포인트에 지정
					}

					FDamageEvent  DamageEvent;

					//결과 액터 데미지처리 (데미지 세기,대미지 종류,공격명령 가해자,대미지전달 도구 )
					OverLapResult.Actor->TakeDamage(fExplosionDamage, DamageEvent, GetWorld()->GetFirstPlayerController(), this);
				}
			}
		}
	}
	else
	{
		//DoNothing
	}

	Destroy();//액터 Destroy
}

void AShooterExplosive::knockBack(FOverlapResult HitResult, FVector Origin, float Impulse)
{
	// 플레이어와 타깃의 방향을 설정
	FVector ImpulseDirection = HitResult.GetActor()->GetActorLocation() - Origin;//밀어질 방향 설정(벡터 -연산)
	ImpulseDirection.Normalize();

	//충돌 세기
	float ImpulseStrength = Impulse;

	FVector FinalImpulse = ImpulseDirection * ImpulseStrength;//임펄스 벡터 설정

	HitResult.GetComponent()->SetPhysicsLinearVelocity(FVector::ZeroVector);
	HitResult.GetComponent()->AddImpulseAtLocation(FinalImpulse, HitResult.GetActor()->GetActorLocation());//해당위치에서 임펄스 만큼 밀어냄
}

// Called every frame
void AShooterExplosive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
