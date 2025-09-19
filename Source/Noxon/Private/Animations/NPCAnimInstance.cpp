// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/NPCAnimInstance.h"
#include "NPCs/BaseNonPlayableCharacter.h"
#include "NPCs/Components/FSMComponent.h"

void UNPCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	ownerNPC = Cast<ABaseNonPlayableCharacter>(TryGetPawnOwner());
}

void UNPCAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (LIKELY(IsValid(ownerNPC)))
	{
		phase = static_cast<uint8>(ownerNPC->GetFSMComp()->GetCurrentPhase());
		moveState = static_cast<uint8>(ownerNPC->GetFSMComp()->GetCurrentMoveState());
		damagedState = static_cast<uint8>(ownerNPC->GetFSMComp()->GetCurrentDamagedState());
		localVelocity = ownerNPC->GetTransform().InverseTransformVector(ownerNPC->GetVelocity());
	}
}
