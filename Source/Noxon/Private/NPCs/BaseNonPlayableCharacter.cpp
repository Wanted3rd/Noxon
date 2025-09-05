// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/BaseNonPlayableCharacter.h"


// Sets default values
ABaseNonPlayableCharacter::ABaseNonPlayableCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseNonPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ABaseNonPlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseNonPlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

