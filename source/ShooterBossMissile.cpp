// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterBossMissile.h"
#include "ShooterAICharacter.h"//AI 캐릭터
#include "ShooterBossCharacter.h"//Ai 보스캐릭터
#include "Shooter2Character.h"//캐릭터
#include "DrawDebugHelpers.h"//디버그 그리기
#include "EngineUtils.h"//반복자 사용
#include "ShooterActorComponent.h"//캐릭터 스탯
#include "GameFramework/Pawn.h"
#include "ShooterEnemyCharacter.h"//에너미 캐릭터


//보스가 발사할 미사일
AShooterBossMissile::AShooterBossMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MissileComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MissileComp"));//미사일 캡슐 컴포넌트 초기화
	MissileComp->InitCapsuleSize(8.0f,40.0f);//캡슐크기 설정
	MissileComp->SetCollisionProfileName(TEXT("AIMissile"));//콜리전 설정
	RootComponent = MissileComp;//캡슐을 루트 컴초넌트로 지정
	
	MissileBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MISSILEBODY"));//미사일 바디
	MissileEject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MISSILEEJECT"));//미사일 추진

	FString AssetPath = TEXT("/Game/MyAsset/MissleLauncher/Mesh/StaticM_Missile.StaticM_Missile");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MISSILEBODY(*AssetPath);//미사일바디 불러오기
	if (SM_MISSILEBODY.Succeeded())
	{
		MissileBody->SetStaticMesh(SM_MISSILEBODY.Object);
		MissileBody->SetCollisionProfileName(TEXT("NoCollision"));//메시는 콜리전 비활성
		MissileBody->SetupAttachment(RootComponent);//루트컴포넌트에 미사일 Body부착
		MissileBody->SetRelativeRotation(FRotator(90.0f,0.0f,0.0f));//메시회전 설정
	}

	FString AssetPath2 = TEXT("/Game/MyAsset/MissleLauncher/Mesh/StaticM_MissileExhaust.StaticM_MissileExhaust");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MISSILEEJECT(*AssetPath2);//미사일 이젝트 불러오기
	if (SM_MISSILEEJECT.Succeeded())
	{
		MissileEject->SetStaticMesh(SM_MISSILEEJECT.Object);
		MissileEject->SetCollisionProfileName(TEXT("NoCollision"));//메시는 콜리전 비활성
		MissileEject->SetupAttachment(MissileBody);//미사일 Body에 Eject메시를 부착
	}

	FName MissileHeadSocket(TEXT("ExhaustSoket"));
	if (MissileBody->DoesSocketExist(MissileHeadSocket))//MissileBody 메시에 ExhaustSoket이름의 소켓이 있을 때
	{
		MissileEject->SetupAttachment(MissileBody, MissileHeadSocket);//미사일 Body에 Eject를 부착
	}

	ExplosionParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("ExplosionPaticle"));//폭파 파티클
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PT_Explosion(TEXT("/Game/MyAsset/PS_MissileExplosion.PS_MissileExplosion"));
	if (PT_Explosion.Succeeded())
	{
		ExplosionParticle = PT_Explosion.Object;
	}

	ExplosionSound = CreateDefaultSubobject<USoundBase>(TEXT("Explosion"));//폭파 사운드
	static ConstructorHelpers::FObjectFinder<USoundBase>SB_Explosion(TEXT("/Game/MyAsset/SC_MissileExplosion.SC_MissileExplosion"));
	if (SB_Explosion.Succeeded())
	{
		ExplosionSound = SB_Explosion.Object;
	}

	//델리게이트 바인딩
	OnActorBeginOverlap.AddDynamic(this, &AShooterBossMissile::MyOnBeginOverlap);//액터가 Overlap시

	CharacterStat = CreateDefaultSubobject<UShooterActorComponent>(TEXT("CharacterStat"));//캐릭터 스탯 클래스

}

void AShooterBossMissile::MyOnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	Explosion();//액터 겹침 발생시 폭발
}

float AShooterBossMissile::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	Destroy();//데미지를 입을 경우 Destroy
	
	return FinalDamage;
}

void AShooterBossMissile::Explosion()
{
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());//폭발사운드 재생
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionParticle, GetActorLocation());//폭발 파티클재생

	float Range = 300.0f;//공격범위
	FVector Center = GetActorLocation();//폭파지점
	TArray<FOverlapResult> OverLapResults;//TArray 자료구조로 오버랩 결과 인스턴스를 저장한다.
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);//콜리전 선언

	FCollisionObjectQueryParams ObjQuery;//공격 받는 인스턴스 집합(배열)
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel1);// 공격받는 트레이스 채널 설정 추가 (캐릭터)
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel2);// 공격받는 트레이스 채널 설정 추가 (AI)
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel3);// 공격받는 트레이스 채널 설정 추가 (GunMesh)
	ObjQuery.AddObjectTypesToQuery(ECC_GameTraceChannel6);// 공격받는 트레이스 채널 설정 추가 (AIMissile)
	ObjQuery.AddObjectTypesToQuery(ECC_PhysicsBody);//피직스바디 추가

	bool bResult = GetWorld()->OverlapMultiByObjectType(//오브젝트 타입을 멀티 방식으로 탐지(생성할 지역에 충돌할 개체들에 채널을 설정하여 탐색)
		OverLapResults,
		Center,
		FQuat::Identity,
		ObjQuery,//탐색 감지에 사용할 트레이스 채널 정보
		FCollisionShape::MakeSphere(Range),//탐색에 사용할 기본 도형 정보, 구체, 캡슐, 박스를 사용
		CollisionQueryParam//콜리전 선언
	);

	if (bResult)//유효값을 반환할 시
	{
		for (auto OverLapResult : OverLapResults)//for문 처리로 각각의 액터를 반환
		{

			FCollisionObjectQueryParams ObjQueryLine;//공격 받는 인스턴스 집합(배열)
			ObjQueryLine.AddObjectTypesToQuery(ECC_GameTraceChannel4);// 공격받는 트레이스 채널 설정 추가 (블록)

			FCollisionQueryParams ColQuery;//실행 주체 인스턴스
			ColQuery.AddIgnoredActor(this);//실행 주체 액터는 공격 무시

			FHitResult HitRes;//히트 확인 인스턴스

			//라인 트레이스로 히트확인검사
			bool HitResult = GetWorld()->LineTraceSingleByObjectType(HitRes, Center, OverLapResult.GetActor()->GetActorLocation(), ObjQueryLine, ColQuery);

			FName CollisionName(TEXT("Ragdoll"));//래그돌 네임
			FName CollisionName2(TEXT("GunMesh"));//건메시 네임
			FName CollisionName3(TEXT("AIChara"));//AI 캐릭터 네임
			FName CollisionName4(TEXT("ShooterChara"));//슈터 캐릭 네임
			FName CollisionName5(TEXT("AIMissile"));//AI 미사일 네임

			if (!HitResult)//HitRes.bBlockingHit)//라인트레이스를 실행했을 때 그 사이에 블록 요소가 없을 시 
			{
				if (CollisionName == OverLapResult.GetComponent()->GetCollisionProfileName())//래그돌 콜리전일 경우
				{
					knockBack(OverLapResult, Center, 50000.0f);
				}
				else if (CollisionName2 == OverLapResult.GetComponent()->GetCollisionProfileName())//건메시 네임콜리전일 경우
				{
					knockBack(OverLapResult, Center, 2000.0f);
				}

				if (OverLapResult.Actor.IsValid())
				{
					if (CollisionName3 == OverLapResult.GetComponent()->GetCollisionProfileName())//AI 콜리전일 경우
					{
						auto HitAI = Cast<AShooterEnemyCharacter>(OverLapResult.GetActor());
						HitAI->SetDamagePoint(GetActorLocation());//미사일 위치를 상대 액터 데미지 포인트에 지정
					}
					else if (CollisionName4 == OverLapResult.GetComponent()->GetCollisionProfileName())//플레이어 캐릭터일 경우
					{
						auto HitChara = Cast<AShooter2Character>(OverLapResult.GetActor());
						HitChara->SetDamagePoint(GetActorLocation());//미사일 위치를 상대 액터 데미지 포인트에 지정
					}

					FDamageEvent  DamageEvent;

					//데미지 실행. 데미지 세기,대미지 종류,공격명령 가해자,대미지전달 도구
					OverLapResult.Actor->TakeDamage(CharacterStat->GetAIBossMissileDamage(), DamageEvent, nullptr, this);

				}
			}

		}
	}
	else
	{
		//Donothing
	}

	Destroy();//폭발 이후 액터 Destroy
}

void AShooterBossMissile::MissileTracking()//미사일이 가장 가까이 있는 플레이어를 따라감
{
	FVector Between = TargetPlyaerCharacter->GetActorLocation() - GetActorLocation();//월드상 플레이어 캐릭터와 미사일 사이의 방향벡터를 구함

	FVector MissilePos = MoveToward(GetActorLocation(), TargetPlyaerCharacter->GetActorLocation(), 7.0f);//미사일이 이동할 벡터를 구함
	SetActorLocation(MissilePos);//미사일 이동

	FRotator RotToTarget = FRotationMatrix::MakeFromZ(Between).Rotator();
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), RotToTarget, GetWorld()->GetDeltaSeconds(), 15.0f));

	if (MissilePos == TargetPlyaerCharacter->GetActorLocation()) Destroy();


}

FVector AShooterBossMissile::MoveToward(FVector Start, FVector End, float Alpah)
{
	FVector a = End - Start;
	float magnitude = a.Size();//벡터의 magnitude구함
	if (magnitude <= Alpah || magnitude == 0.0f)
	{
		return End;
	}
	return Start + a / magnitude * Alpah;//시작지점에서 유닛벡터*Alpha값을 더한값을 반환
}

void AShooterBossMissile::knockBack(FOverlapResult HitResult, FVector Origin, float Impulse)
{
	// 플레이어와 타깃의 방향을 설정
	FVector ImpulseDirection = HitResult.GetActor()->GetActorLocation() - Origin;//밀어질 방향 설정(벡터 -연산)
	ImpulseDirection.Normalize();//벡터 정규화

	//충돌 세기
	float ImpulseStrength = Impulse;

	FVector FinalImpulse = ImpulseDirection * ImpulseStrength;//임펄스 설정

	HitResult.GetComponent()->SetPhysicsLinearVelocity(FVector::ZeroVector);
	HitResult.GetComponent()->AddImpulseAtLocation(FinalImpulse, HitResult.GetActor()->GetActorLocation());//해당위치에서 임펄스 만큼 밀어냄
}

// Called when the game starts or when spawned
void AShooterBossMissile::BeginPlay()
{
	Super::BeginPlay();
	
	for (FConstPawnIterator CharaIter = GetWorld()->GetPawnIterator(); CharaIter; ++CharaIter)//이터레이터를 이용해 플레이어 폰을 찾아내어 가장 가까이 있는 플레이어 설정
	{
		if (CharaIter->Get()->IsPlayerControlled())
		{
			APawn* TempTarget = CharaIter->Get();

			if (fMinimalLen > TempTarget->GetDistanceTo(this))
			{
				fMinimalLen = TempTarget->GetDistanceTo(this);
				TargetPlyaerCharacter = TempTarget;
			}
		}
	}
	
}

// Called every frame
void AShooterBossMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MissileTracking();//틱마다 미사일 트랙킹 활성화
}
