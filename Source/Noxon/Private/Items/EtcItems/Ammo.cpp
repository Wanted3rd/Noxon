// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/EtcItems/Ammo.h"


// Sets default values
AAmmo::AAmmo()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//아이템 기본값 설정
	property.bQuantity = true;
	property.maxQuantity = 120;
	property.quantity = 0;
	property.bDurability = true;
	property.durability = 0.f;
	property.bMagazine = false;
	property.magazine = 30;
	property.deal = 10.f;
	property.heal = 0.f;
	
}

// Called when the game starts or when spawned
void AAmmo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

