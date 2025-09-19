// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Actions/Damaged/DeadlyDamaged.h"
#include "NPCs/BaseNonPlayableCharacter.h"
#include "NPCs/Components/ActionComponent.h"
#include "NPCs/Manager/NPCManager.h"
#include "NPCs/Components/FSMComponent.h"
#include "NPCs/Datas/StateEnums.h"
#include "Utility/FindHelper.h"

UDeadlyDamaged::UDeadlyDamaged()
{
	deadMontage = FinderHelper::FindAssetFromConstructor<UAnimMontage>("");
}

void UDeadlyDamaged::OnBegin(ABaseNonPlayableCharacter* owner)
{
	owner->GetFSMComp()->DeactivatePhaseState(
		EPhase::OrdinaryPhase | EPhase::BattlePhase | EPhase::InteractPhase,
		999.f
	);
	owner->GetActionComp()->SetPhaseAction(nullptr);
	owner->GetActionComp()->SetMoveAction(nullptr);
	if (IsValid(deadMontage))
	{
		owner->PlayAnimMontage(deadMontage);
	}
}

void UDeadlyDamaged::OnTick(ABaseNonPlayableCharacter* owner, float deltaTime)
{
	if (owner->GetMesh()->GetAnimInstance()->Montage_GetCurrentSection() == deadMontageEndSectionName)
	{
		owner->GetActionComp()->SetDamagedAction(nullptr);
	}
}

void UDeadlyDamaged::OnEnd(ABaseNonPlayableCharacter* owner)
{
	owner->DeadNPC();
}
