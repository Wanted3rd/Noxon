// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Actions/Battle/HipFireAction.h"

#include "Items/HandItems/HandItem.h"
#include "NPCs/BaseNonPlayableCharacter.h"

void UHipFireAction::OnBegin(ABaseNonPlayableCharacter* owner)
{
	if (!owner->GetHandItem())
	{
		return;
	}
}

void UHipFireAction::OnTick(ABaseNonPlayableCharacter* owner, float deltaTime)
{
	if (!owner->GetHandItem())
	{
		return;
	}
	owner->GetHandItem()->LeftAction();
}

void UHipFireAction::OnEnd(ABaseNonPlayableCharacter* owner)
{
}
