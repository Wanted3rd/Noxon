// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/NPCAnimInstance.h"
#include "NPCs/BaseNonPlayableCharacter.h"

void UNPCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	ownerNPC = Cast<ABaseNonPlayableCharacter>(TryGetPawnOwner());
}

void UNPCAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	
}
