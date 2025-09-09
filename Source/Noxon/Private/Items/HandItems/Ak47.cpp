// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/HandItems/Ak47.h"

// Sets default values
AAk47::AAk47()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAk47::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAk47::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

