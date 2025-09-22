// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/HandItems/RifleDemoGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Items/EtcItems/Ammo.h"
#include "Items/EtcItems/RifleDefaultAmmo.h"
#include "UObject/ConstructorHelpers.h"



// Sets default values
ARifleDemoGun::ARifleDemoGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	property.maxMagazine = 30;
	gunSKM = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	SetRootComponent(gunSKM);
	
	//gunMesh->bOnlyOwnerSee = true;
	//gunMesh->CastShadow = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> tempRifle(TEXT("/Game/Assets/CustomAssets/FP_Gun/AssultRifle/Meshes/SK_AssaultRifle.SK_AssaultRifle"));
	if (tempRifle.Succeeded())
	{
		gunSKM->SetSkeletalMesh(tempRifle.Object);
		
	}
	ammoFactory = ARifleDefaultAmmo::StaticClass();
}


// Called when the game starts or when spawned
void ARifleDemoGun::BeginPlay()
{
	Super::BeginPlay();
	
}

