// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/HandItems/RifleDemoGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ARifleDemoGun::ARifleDemoGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	gunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	RootComponent = gunMesh;
	gunMesh->bOnlyOwnerSee = true;
	gunMesh->CastShadow = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> tempRifle(TEXT("/Game/Assets/CustomAssets/FP_Gun/AssultRifle/Meshes/SK_AssaultRifle.SK_AssaultRifle"));
	if (tempRifle.Succeeded())
	{
		gunMesh->SetSkeletalMesh(tempRifle.Object);
		
	}

}

// Called when the game starts or when spawned
void ARifleDemoGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARifleDemoGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

