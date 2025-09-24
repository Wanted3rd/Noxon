// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Actions/Battle/ReloadingAction.h"

#include "NPCs/BaseNonPlayableCharacter.h"
#include "Items/HandItems/Gun.h"
#include "NPCs/Components/FSMComponent.h"
#include "NPCs/Components/ActionComponent.h"
#include "NPCs/Datas/StateEnums.h"
#include "Engine/World.h"

void UReloadingAction::OnBegin(ABaseNonPlayableCharacter* owner)
{
	if (!owner || !owner->GetActionComp()) return;

	AGun* gun = Cast<AGun>(owner->GetHandItem());
	if (!gun) return;

	const FItemProperty& gunProperty = gun->GetItemProperty();
	int32 ammoNeeded = gunProperty.maxMagazine - gunProperty.magazine;

	gun->RKeyAction(ammoNeeded);

	owner->GetActionComp()->ResetReloadTime();
}

void UReloadingAction::OnTick(ABaseNonPlayableCharacter* owner, float deltaTime)
{
	if (!owner || !owner->GetActionComp()) return;

	float reloadTime = owner->GetActionComp()->GetReloadTime();
	reloadTime += deltaTime;
	owner->GetActionComp()->SetReloadTime(reloadTime);

	const float reloadDuration = 3.0f;

	if (reloadTime >= reloadDuration)
	{
		owner->GetFSMComp()->ActivatePhaseState(EPhase::HipFire);
		owner->GetFSMComp()->ActivateMoveState(EMoveState::Chase);
	}
}

void UReloadingAction::OnEnd(ABaseNonPlayableCharacter* owner)
{
}
