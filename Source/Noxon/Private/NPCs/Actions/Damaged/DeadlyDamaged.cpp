// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Actions/Damaged/DeadlyDamaged.h"
#include "NPCs/BaseNonPlayableCharacter.h"
#include "NPCs/Manager/NPCManager.h"
#include "NPCs/Components/FSMComponent.h"
#include "Utility/FindHelper.h"

UDeadlyDamaged::UDeadlyDamaged()
{
	deadMontage = FinderHelper::FindAssetFromConstructor<UAnimMontage>("");
}

void UDeadlyDamaged::OnBegin(ABaseNonPlayableCharacter* owner)
{
	owner->GetFSMComponent()->DeactivatePhase(
		EPhase::OrdinaryPhase | EPhase::BattlePhase | EPhase::InteractPhase,
		999.f
	);
	owner->SetPhaseAction(nullptr);
	owner->SetMoveAction(nullptr);
	if (IsValid(deadMontage))
	{
		owner->PlayAnimMontage(deadMontage);
	}
}

void UDeadlyDamaged::OnTick(ABaseNonPlayableCharacter* owner, float deltaTime)
{
	if (owner->GetMesh()->GetAnimInstance()->Montage_GetCurrentSection() == deadMontageEndSectionName)
	{
		owner->SetDamagedAction(nullptr);
	}
}

void UDeadlyDamaged::OnEnd(ABaseNonPlayableCharacter* owner)
{
	owner->DeadNPC();
}
