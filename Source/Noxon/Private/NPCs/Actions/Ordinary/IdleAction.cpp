// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Actions/Ordinary/IdleAction.h"
#include "NPCs/BaseNonPlayableCharacter.h"
#include "NPCs/Components/FSMComponent.h"
#include "NPCs/Datas/StateEnums.h"
#include "Engine/World.h"

void UIdleAction::OnBegin(ABaseNonPlayableCharacter* owner)
{
	if (!owner) return;
}

void UIdleAction::OnTick(ABaseNonPlayableCharacter* owner, float deltaTime)
{
}

void UIdleAction::OnEnd(ABaseNonPlayableCharacter* owner)
{
}
