// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/NeutralNPC.h"


// Sets default values
ANeutralNPC::ANeutralNPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANeutralNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANeutralNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

