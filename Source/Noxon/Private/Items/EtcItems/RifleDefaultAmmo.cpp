// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/EtcItems/RifleDefaultAmmo.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MainPlayer.h"


// Sets default values
ARifleDefaultAmmo::ARifleDefaultAmmo()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//root
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("collisionComp"));
	RootComponent = collisionComp;
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));

	bodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bodyMesh"));
	bodyMesh->SetupAttachment(collisionComp);
	bodyMesh->SetRelativeScale3D(FVector(0.1f));
	// Static Mesh 가져오기
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (TempMesh.Succeeded())
	{
		bodyMesh->SetStaticMesh(TempMesh.Object);
	}

	moveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("moveComp"));
	moveComp->InitialSpeed = speed;
	moveComp->MaxSpeed = speed;
	moveComp->bShouldBounce = true;
	moveComp->Bounciness = 0.2f;

	moveComp->SetUpdatedComponent(collisionComp);
	
}

// Called when the game starts or when spawned
void ARifleDefaultAmmo::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AMainPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainPlayer::StaticClass()));
	
}

// Called every frame
void ARifleDefaultAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ARifleDefaultAmmo::SetActive(bool bActive)
{

	if (bActive)
	{
		// Rendering
		bodyMesh->SetVisibility(true);
		// collision
		collisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		// 다시 발사 속도 세팅
		moveComp->Velocity = GetActorForwardVector() * moveComp->InitialSpeed;

		// 타이머 이용해서 제거 되도록 처리
		// 무명함수 (Lambda)
		auto DieLambda = [this]()->void
		{
			// Destroy();
			// 탄창에 넣어주자
			if (player)
			{
				SetActive(false);
				player->bulletPool.Add(this);
			}
			else
			{
				SetActive(false);
				
			}
		};

		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle, FTimerDelegate::CreateLambda(DieLambda), destroyTime, false);
	}
	else
	{
		// Rendering
		bodyMesh->SetVisibility(false);
		// collision
		collisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
	}
}