// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/HandItems/PickAxe.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Items/EtcItems/GatherableRock.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ISocketProvider.h"


// Sets default values
APickAxe::APickAxe()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> tempPickAxe(TEXT("/Game/Pickaxe/SKM_lowpoly_pickaxe.SKM_lowpoly_pickaxe"));
	if (tempPickAxe.Succeeded())
	{
		skeletalMesh = tempPickAxe.Object;
	}
	
	// skeletalMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Assets/CustomAssets/FP_Gun/AssultRifle/Meshes/SK_AssaultRifle.SK_AssaultRifle"));
	
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempLeftMontage(TEXT("/Game/Assets/CustomAssets/FP_Axe/My_Standing_Melee_Attack_Montage.My_Standing_Melee_Attack_Montage"));
	if (tempLeftMontage.Succeeded())
	{
		leftMontage = tempLeftMontage.Object;
	}

	attachPointSocketName = TEXT("hand_rSocket");
	
}

// Called when the game starts or when spawned
void APickAxe::BeginPlay()
{
	Super::BeginPlay();

	// UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGatherableRock::StaticClass(), FoundRocks);
	// for (AActor* Actor : FoundRocks)
	// {
	// 	gatherableRock = Cast<AGatherableRock>(Actor);
	// 	if (gatherableRock)
	// 	{
	// 		// gatherableRock->m_Dele_hit.AddDynamic(this , &APickAxe::DamageRock);
	// 	}
	// }




}

// Called every frame
void APickAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void APickAxe::DamageRock(AGatherableRock* hitRock, AActor* pickAxe)
{
	hitRock->hp = hitRock->hp - 20;
}

void APickAxe::LeftAction() 
{
	
	    // 설정값 (원하면 멤버변수로 빼서 에디터 조정 가능)
    const float TraceDistance = 200.f;    // 카메라 앞쪽으로 얼마나 쏠지
    const float TraceRadius = 40.f;        // sphere 반지름
    const float DamageAmount = 20.f;       // 돌에 입힐 데미지

    // 1) 소유자(플레이어) 확인
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("PickAxe::LeftAction - No Owner"));
        return;
    }

    // 2) 카메라 위치/방향 얻기 (우선적으로 카메라 컴포넌트, 없으면 PlayerController camera fallback)
    FVector CamLocation;
    FVector CamForward;

    if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
    {
        // 시도 1: 카메라 컴포넌트가 있을 경우
        if (UCameraComponent* CamComp = OwnerChar->FindComponentByClass<UCameraComponent>())
        {
            CamLocation = CamComp->GetComponentLocation();
            CamForward = CamComp->GetForwardVector();
        }
    }

    // fallback: PlayerController의 camera (플레이어 컨트롤러가 있으면)
    if (CamForward.IsZero())
    {
        if (APawn* OwnerPawn = Cast<APawn>(OwnerActor))
        {
            if (APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController()))
            {
                FRotator CameraRot;
                PC->GetPlayerViewPoint(CamLocation, CameraRot); // 위치와 회전 얻기
                CamForward = CameraRot.Vector();
            }
        }
    }

    if (CamForward.IsZero())
    {
        UE_LOG(LogTemp, Warning, TEXT("PickAxe::LeftAction - Could not get camera forward vector"));
        return;
    }

    // 3) Sweep (Sphere Trace)
    const FVector Start = CamLocation;
    const FVector End = Start + CamForward * TraceDistance;

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);           // 자기 자신 무시
    if (OwnerActor) QueryParams.AddIgnoredActor(OwnerActor); // 소유자 무시 (자기 타격 방지)
    QueryParams.bReturnPhysicalMaterial = false;
    QueryParams.bTraceComplex = false;

    FHitResult Hit;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(TraceRadius);

    bool bHit = GetWorld()->SweepSingleByChannel(
        Hit,
        Start,
        End,
        FQuat::Identity,
        ECC_Visibility,   // 필요에 따라 채널 변경
        Sphere,
        QueryParams
    );

    // 디버그 드로우 (한 번만, 개발중에 확인용)
    #if WITH_EDITOR
    const FColor DrawColor = bHit ? FColor::Red : FColor::Green;
    DrawDebugSphere(GetWorld(), Start, TraceRadius, 12, FColor::Blue, false, 1.0f);
    DrawDebugLine(GetWorld(), Start, End, DrawColor, false, 1.0f, 0, 1.5f);
    if (bHit)
    {
        DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius * 0.75f, 12, FColor::Red, false, 2.0f);
    }
    #endif

    // 4) 히트 처리: AGatherableRock이면 데미지 입히기 / 파괴
    if (bHit && Hit.GetActor())
    {
        if (AGatherableRock* Rock = Cast<AGatherableRock>(Hit.GetActor()))
        {
            // 예: Rock이 hp 멤버를 공개적으로 가지고 있음
            Rock->hp -= DamageAmount;
            // UE_LOG(LogTemp, Log, TEXT("PickAxe hit rock. Remaining hp: %f"), Rock->hp);

            if (Rock->hp <= 0.f)
            {
                Rock->Destroy();
            }
            else
            {
                // 임팩트 이팩트, 사운드 등 추가 가능
            }
            return;
        }

        // 만약 AGatherableRock이 아니라면, 일반 데미지 이벤트를 보낼 수도 있음:
        UGameplayStatics::ApplyPointDamage(Hit.GetActor(), DamageAmount, CamForward, Hit, OwnerActor->GetInstigatorController(), this, nullptr);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("PickAxe::LeftAction - Nothing hit"));
    }
}
