// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Actions/Ordinary/StopMove.h"

#include "NPCs/BaseNonPlayableCharacter.h"
#include "NPCs/Components/FSMComponent.h"
#include "NPCs/Datas/StateEnums.h"
#include "Engine/World.h"


void UStopMove::OnBegin(ABaseNonPlayableCharacter* owner)
{
	if (!owner) return;

	owner->GetController()->StopMovement();

	float waitTime = FMath::RandRange(5.0f, 12.0f);

	FTimerHandle timerHandle;
	owner->GetWorld()->GetTimerManager().SetTimer(
		timerHandle,
		FTimerDelegate::CreateLambda([owner]()
		{
			if (owner && owner->GetFSMComp())
			{
				if (owner->GetFSMComp()->GetCurrentMoveState() == EMoveState::Stop &&
					(owner->GetFSMComp()->GetCurrentPhase() & EPhase::OrdinaryPhase) > EPhase::Default)
				{
					owner->GetFSMComp()->ActivateMoveState(EMoveState::Patrol);
				}
			}
		}),
		waitTime,
		false
	);
}

void UStopMove::OnTick(ABaseNonPlayableCharacter* owner, float deltaTime)
{
	if (!owner) return;

	if (!owner->GetVelocity().IsNearlyZero())
	{
		owner->GetController()->StopMovement();
	}
}

void UStopMove::OnEnd(ABaseNonPlayableCharacter* owner)
{
	if (!owner) return;
}
