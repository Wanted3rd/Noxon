// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Actions/Ordinary/StopMove.h"

#include "NPCs/BaseNonPlayableCharacter.h"


void UStopMove::OnBegin(ABaseNonPlayableCharacter* owner)
{
}

void UStopMove::OnTick(ABaseNonPlayableCharacter* owner, float deltaTime)
{
	if (!owner->GetVelocity().IsNearlyZero())
	{
		owner->GetController()->StopMovement();
	}
}

void UStopMove::OnEnd(ABaseNonPlayableCharacter* owner)
{
}
