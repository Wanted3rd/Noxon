// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Actions/Battle/WeaveMove.h"
#include "NPCs/BaseNonPlayableCharacter.h"
#include "NPCs/Components/FSMComponent.h"
#include "NPCs/Components/ActionComponent.h"
#include "NPCs/Datas/StateEnums.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

void UWeaveMove::OnBegin(ABaseNonPlayableCharacter* owner)
{
	if (!owner || !owner->GetActionComp()) return;

	owner->GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	owner->GetActionComp()->SetWeaveTime(0.0f);
	owner->GetActionComp()->SetWeaveDuration(FMath::RandRange(1.0f, 3.0f));

	FVector randomDirection = FVector(
		FMath::RandRange(-1.0f, 1.0f),
		FMath::RandRange(-1.0f, 1.0f),
		0.0f
	).GetSafeNormal();

	owner->GetActionComp()->SetWeaveDirection(randomDirection);
}

void UWeaveMove::OnTick(ABaseNonPlayableCharacter* owner, float deltaTime)
{
	if (!owner || !owner->GetActionComp()) return;

	float weaveTime = owner->GetActionComp()->GetWeaveTime();
	weaveTime += deltaTime;
	owner->GetActionComp()->SetWeaveTime(weaveTime);

	float weaveDuration = owner->GetActionComp()->GetWeaveDuration();

	if (weaveTime >= weaveDuration)
	{
		float randomChoice = FMath::RandRange(0.0f, 1.0f);
		if (randomChoice < 0.5f)
		{
			owner->GetFSMComp()->ActivateMoveState(EMoveState::Stop);
		}
		else
		{
			owner->GetFSMComp()->ActivateMoveState(EMoveState::Chase);
		}
		return;
	}

	FVector weaveDirection = owner->GetActionComp()->GetWeaveDirection();
	FVector movement = weaveDirection * FMath::Sin(weaveTime * 4.0f);
	owner->AddMovementInput(movement);
}

void UWeaveMove::OnEnd(ABaseNonPlayableCharacter* owner)
{
	if (!owner) return;

	owner->GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}
