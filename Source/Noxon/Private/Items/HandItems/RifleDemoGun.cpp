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
	// gunSKM = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	// SetRootComponent(gunSKM);
	
	//gunMesh->bOnlyOwnerSee = true;
	//gunMesh->CastShadow = false;

	reloadTime = 3.f;
	
	fireRate = 30.f;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> tempRifle(TEXT("/Game/Assets/CustomAssets/FP_Gun/AssultRifle/Meshes/SK_AssaultRifle.SK_AssaultRifle"));
	if (tempRifle.Succeeded())
	{
		skeletalMesh = tempRifle.Object;
	}
	
	// skeletalMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Assets/CustomAssets/FP_Gun/AssultRifle/Meshes/SK_AssaultRifle.SK_AssaultRifle"));
	
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempLeftMontage(TEXT("/Game/Assets/CustomAssets/FP_Gun/AssultRifle/Animations/A_FP_AssaultRifle_Fire_Aimed_Montage.A_FP_AssaultRifle_Fire_Aimed_Montage"));
	if (tempLeftMontage.Succeeded())
	{
		leftMontage = tempLeftMontage.Object;
	}

	attachPointSocketName = TEXT("ik_hand_gun");
	// if (tempRifle.Succeeded())
	// {
	// 	skeletalMesh->SetSkeletalMesh(tempRifle.Object);
	// 	
	// }
	ammoFactory = ARifleDefaultAmmo::StaticClass();
}


// Called when the game starts or when spawned
void ARifleDemoGun::BeginPlay()
{
	Super::BeginPlay();
	
}



void ARifleDemoGun::RightAction()
{
}

int ARifleDemoGun::RKeyAction(int inValue)
{
	return 0;
}

