// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Actions/Ordinary/PatrolMove.h"

#include "AIController.h"
#include "NPCs/BaseNonPlayableCharacter.h"
#include "NPCs/Components/ActionComponent.h"
#include "NPCs/Components/FSMComponent.h"
#include "NPCs/Components/PerceptionComponent.h"
#include "NPCs/Datas/StateEnums.h"

void UPatrolMove::OnBegin(ABaseNonPlayableCharacter* owner)
{
	owner->GetActionComp()->RandPos();
}

void UPatrolMove::OnTick(ABaseNonPlayableCharacter* owner, float deltaTime)
{
	if (!owner) return;

	FHitResult hitResult;
	owner->MoveBlockedBy(hitResult);
	if (hitResult.ImpactPoint.Z > owner->GetActorLocation().Z + 60.f)
	{
		owner->GetFSMComp()->ActivateMoveState(EMoveState::Stop);
		return;
	}

	FVector goalLocation;
	if (owner->GetActionComp()->GetGoalLocation(goalLocation))
	{
		owner->GetAIController()->MoveToLocation(goalLocation);
		owner->GetActionComp()->TogglePatrolFlag();
		return;
	}
	owner->GetFSMComp()->ActivateMoveState(EMoveState::Stop);
}

void UPatrolMove::OnEnd(ABaseNonPlayableCharacter* owner)
{
}
