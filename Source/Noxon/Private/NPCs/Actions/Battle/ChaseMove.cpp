// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Actions/Battle/ChaseMove.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "NPCs/BaseNonPlayableCharacter.h"
#include "NPCs/Components/FSMComponent.h"
#include "NPCs/Components/PerceptionComponent.h"
#include "NPCs/Datas/StateEnums.h"
#include "NPCs/Enemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "Player/MainPlayer.h"

void UChaseMove::OnBegin(ABaseNonPlayableCharacter* owner)
{
	owner->GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void UChaseMove::OnTick(ABaseNonPlayableCharacter* owner, float deltaTime)
{
	if (!owner) return;

	AEnemy* enemy = Cast<AEnemy>(owner);
	if (!enemy) return;

	AActor* target = nullptr;

	if (UAIPerceptionComponent* aiPerception = enemy->GetAIPerceptionComponent())
	{
		TArray<AActor*> perceivedActors;
		aiPerception->GetCurrentlyPerceivedActors(nullptr, perceivedActors);

		for (AActor* actor : perceivedActors)
		{
			if (Cast<AMainPlayer>(actor))
			{
				target = actor;
				break;
			}
		}
	}

	if (!target) return;

	FVector directionToTarget = target->GetActorLocation() - owner->GetActorLocation();
	float distanceToTarget = directionToTarget.Size();
	directionToTarget = directionToTarget.GetSafeNormal();

	owner->AddMovementInput(directionToTarget);

	const float shootingRange = 800.0f;
	if (distanceToTarget <= shootingRange)
	{
		owner->GetFSMComp()->ActivatePhaseState(EPhase::HipFire);
	}
}

void UChaseMove::OnEnd(ABaseNonPlayableCharacter* owner)
{
	owner->GetFSMComp()->ActivateMoveState(EMoveState::Stop);
}
