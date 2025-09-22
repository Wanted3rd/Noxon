// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Actions/Battle/ReloadingAction.h"

#include "NPCs/BaseNonPlayableCharacter.h"
#include "Items/HandItems/Gun.h"
#include "NPCs/Components/FSMComponent.h"
#include "NPCs/Datas/StateEnums.h"

void UReloadingAction::OnBegin(ABaseNonPlayableCharacter* owner)
{
	if (AGun* gun = Cast<AGun>(owner->GetHandItem()))
	{
		FItemProperty gunProperty = gun->GetItemProperty();
		owner->GetHandItem()->RKeyAction(gunProperty.maxMagazine);
	}
}

void UReloadingAction::OnTick(ABaseNonPlayableCharacter* owner, float deltaTime)
{
	owner->GetFSMComp()->ActivatePhaseState(EPhase::Idle);
}

void UReloadingAction::End(ABaseNonPlayableCharacter* owner)
{
}
