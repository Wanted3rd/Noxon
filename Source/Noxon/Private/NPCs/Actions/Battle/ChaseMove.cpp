// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Actions/Battle/ChaseMove.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "NPCs/BaseNonPlayableCharacter.h"
#include "NPCs/Components/FSMComponent.h"
#include "NPCs/Components/PerceptionComponent.h"
#include "NPCs/Datas/StateEnums.h"

void UChaseMove::OnBegin(ABaseNonPlayableCharacter* owner)
{
	owner->GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void UChaseMove::OnTick(ABaseNonPlayableCharacter* owner, float deltaTime)
{
	AActor* target = owner->GetPerceptionComp()->GetTarget();
	FVector directionToTarget = FVector::ZeroVector;
	float distanceToTarget = -1.f;
	if (IsValid(target))
	{
		directionToTarget = target->GetActorLocation() - owner->GetActorLocation();
		distanceToTarget = directionToTarget.Size();
		directionToTarget = directionToTarget.GetSafeNormal();
	}
	owner->AddMovementInput(directionToTarget);
	if (distanceToTarget > 0.f && distanceToTarget < owner->GetPerceptionComp()->GetPerceptionProperties().gunFireRange)
	{
		owner->GetFSMComp()->ActivatePhaseState(EPhase::HipFire);
	}
}

void UChaseMove::OnEnd(ABaseNonPlayableCharacter* owner)
{
	owner->GetFSMComp()->ActivateMoveState(EMoveState::Stop);
}
