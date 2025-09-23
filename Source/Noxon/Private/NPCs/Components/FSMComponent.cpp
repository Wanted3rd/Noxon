// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Components/FSMComponent.h"
#include "NPCs/Datas/StateEnums.h"

#include "GameFramework/Character.h"


UFSMComponent::UFSMComponent()
	:currentPhaseState(EPhase::Default), lockedPhaseState(EPhase::Default)
	, currentMoveState(EMoveState::Default), currentDamageState(EDamageState::Default)
{
	PrimaryComponentTick.bCanEverTick = true;

	owner = Cast<ACharacter>(GetOwner());
}

void UFSMComponent::BeginPlay()
{
	Super::BeginPlay();
	currentPhaseState = EPhase::Default;
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
	if (currentPhaseState == EPhase::Default)
	{
		ActivatePhaseState(EPhase::Idle);
	}
}


void UFSMComponent::ActivatePhaseState(const EPhase& inPhaseState)
{
	if (inPhaseState == EPhase::Default)
	{
		return;
	}

	if (currentPhaseState > EPhase::InteractPhase)
	{
		if (EnumHasAnyFlags(lockedPhaseState, EPhase::InteractPhase))
		{
			return;
		}
	}
	else if (currentPhaseState > EPhase::BattlePhase)
	{
		if (EnumHasAnyFlags(lockedPhaseState, EPhase::BattlePhase))
		{
			return;
		}
	}
	else if (currentPhaseState > EPhase::OrdinaryPhase)
	{
		if (EnumHasAnyFlags(lockedPhaseState, EPhase::OrdinaryPhase))
		{
			return;
		}
	}
	
	currentPhaseState |= inPhaseState;
}

void UFSMComponent::DeactivatePhaseState(const EPhase& inPhase, const float& cooldownTime)
{
	if (cooldownTime > 0.f)
	{
		lockedPhaseState |= inPhase;
		GetWorld()->GetTimerManager().SetTimer(lockTimer, [&, inPhase]()
		{
			lockedPhaseState &= ~inPhase;
		}, cooldownTime, false);
	}
	currentPhaseState &= ~inPhase;
}

void UFSMComponent::ActivateMoveState(const EMoveState& inMoveState)
{
	if (currentDamageState == EDamageState::Death || inMoveState == EMoveState::End)
	{
		return;
	}
	uint8 stateValue = static_cast<uint8>(inMoveState);
	if (EnumHasAnyFlags(currentPhaseState, EPhase::BattlePhase))
	{
		if (stateValue > static_cast<uint8>(EMoveState::BattlePhase))
		{
			currentMoveState = inMoveState;
		}
		return;
	}
	if (EnumHasAnyFlags(currentPhaseState, EPhase::OrdinaryPhase))
	{
		if (stateValue > static_cast<uint8>(EMoveState::OrdinaryPhase))
		{
			currentMoveState = inMoveState;
		}
		return;
	}
	if (EnumHasAnyFlags(currentPhaseState, EPhase::InteractPhase))
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
	currentPhaseState = EPhase::End;
	lockedPhaseState = EPhase::End;
	currentMoveState = EMoveState::End;
	currentDamageState = EDamageState::End;
}



