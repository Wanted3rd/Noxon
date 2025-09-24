// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Actions/Battle/HipFireAction.h"

#include "Items/HandItems/HandItem.h"
#include "Items/HandItems/Gun.h"
#include "NPCs/BaseNonPlayableCharacter.h"
#include "NPCs/Enemy.h"
#include "NPCs/Components/FSMComponent.h"
#include "NPCs/Components/ActionComponent.h"
#include "NPCs/Datas/StateEnums.h"
#include "Engine/World.h"

void UHipFireAction::OnBegin(ABaseNonPlayableCharacter* owner)
{
	if (!owner || !owner->GetHandItem())
	{
		return;
	}

	AGun* gun = Cast<AGun>(owner->GetHandItem());
	if (!gun)
	{
		return;
	}

	if (gun->GetItemProperty().magazine <= 0)
	{
		owner->GetFSMComp()->ActivatePhaseState(EPhase::Reloading);
		return;
	}
}

void UHipFireAction::OnTick(ABaseNonPlayableCharacter* owner, float deltaTime)
{
	if (!owner || !owner->GetHandItem())
	{
		return;
	}

	AGun* gun = Cast<AGun>(owner->GetHandItem());
	if (!gun)
	{
		return;
	}

	if (gun->GetItemProperty().magazine <= 0)
	{
		owner->GetFSMComp()->ActivatePhaseState(EPhase::Reloading);
		return;
	}

	owner->GetHandItem()->LeftAction();

	float randomMoveChance = FMath::RandRange(0.0f, 1.0f);
	if (randomMoveChance < 0.3f)
	{
		owner->GetFSMComp()->ActivateMoveState(EMoveState::Weave);
	}
	else if (randomMoveChance < 0.6f)
	{
		owner->GetFSMComp()->ActivateMoveState(EMoveState::Stop);
	}
}

void UHipFireAction::OnEnd(ABaseNonPlayableCharacter* owner)
{
}
