// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Actions/Damaged/SmallDamaged.h"
#include "NPCs/BaseNonPlayableCharacter.h"
#include "NPCs/Components/FSMComponent.h"
#include "NPcs/Datas/StateEnums.h"

void USmallDamaged::OnBegin(ABaseNonPlayableCharacter* owner)
{
	if (owner->GetHp() <= 0)
	{
		owner->GetFSMComp()->ActivateDamagedState(EDamageState::Death);
		return;
	}
	owner->GetFSMComp()->ActivateDamagedState(EDamageState::Default);
}

void USmallDamaged::OnTick(ABaseNonPlayableCharacter* owner, float deltaTime)
{
}

void USmallDamaged::OnEnd(ABaseNonPlayableCharacter* owner)
{
}
