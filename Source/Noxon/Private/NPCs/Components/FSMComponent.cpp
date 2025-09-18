// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Components/FSMComponent.h"

#include "GameFramework/Character.h"


UFSMComponent::UFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	owner = Cast<ACharacter>(GetOwner());
}

void UFSMComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ResetStates();
	Super::EndPlay(EndPlayReason);
}

void UFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (currentDamageState == EDamageState::Death)
	{
		return;
	}
}



void UFSMComponent::DeactivatePhase(const EPhase& inPhase, const float& cooldownTime)
{
	if (cooldownTime > 0.f)
	{
		lockedPhase |= inPhase;
		GetWorld()->GetTimerManager().SetTimer(lockTimer, [&, inPhase]()
		{
			lockedPhase &= ~inPhase;
		}, cooldownTime, false);
	}
	currentPhase &= ~inPhase;
}

void UFSMComponent::ActivateMoveState(const EMoveState& inMoveState)
{
	if (currentDamageState == EDamageState::Death || inMoveState == EMoveState::End)
	{
		return;
	}
	uint8 stateValue = static_cast<uint8>(inMoveState);
	if (EnumHasAnyFlags(currentPhase, EPhase::BattlePhase))
	{
		if (stateValue > static_cast<uint8>(EMoveState::BattlePhase))
		{
			currentMoveState = inMoveState;
		}
		return;
	}
	if (EnumHasAnyFlags(currentPhase, EPhase::OrdinaryPhase))
	{
		if (stateValue > static_cast<uint8>(EMoveState::OrdinaryPhase))
		{
			currentMoveState = inMoveState;
		}
		return;
	}
	if (EnumHasAnyFlags(currentPhase, EPhase::InteractPhase))
	{
		currentMoveState = EMoveState::Stop;
	}
}

void UFSMComponent::ActivateDamagedState(const EDamageState& inDamageState)
{
	if (currentDamageState == EDamageState::Death)
	{
		return;
	}
	currentDamageState = inDamageState;
}


void UFSMComponent::ResetStates()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	currentPhase = EPhase::Default;
	lockedPhase = EPhase::Default;
}

