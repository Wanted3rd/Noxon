// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Actions/Battle/HideMove.h"

#include "NPCs/BaseNonPlayableCharacter.h"

void UHideMove::OnBegin(ABaseNonPlayableCharacter* owner)
{
}

void UHideMove::OnTick(ABaseNonPlayableCharacter* owner, float deltaTime)
{
	owner->AddMovementInput(-owner->GetActorForwardVector());
}

void UHideMove::OnEnd(ABaseNonPlayableCharacter* owner)
{
}
