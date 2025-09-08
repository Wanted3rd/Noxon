// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Components/FSMComponent.h"

#include "GameFramework/Character.h"
#include "Utility/DebugHelper.h"


UFSMComponent::UFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	owner = Cast<ACharacter>(GetOwner());
	stateFunctors.Init(nullptr, EFSMStatesMap::End + 1);
}

void UFSMComponent::BeginPlay()
{
	currentStates = EDefaultsStates::Idle;
	Super::BeginPlay();
}

void UFSMComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ResetStates();
	Super::EndPlay(EndPlayReason);
}

void UFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TickType == LEVELTICK_PauseTick)
	{
		return;
	}

	for (uint8 state = 0; state < EFSMStatesMap::End; ++state)
	{
		if (!stateFunctors[state].IsSet())
		{
			LOG_TEXT(TEXT("%s is not Bound"), *(GetStateName(state).ToString()));
		}
		else if (CheckBitFlagIsMasked(currentStates, state) && !CheckBitFlagIsMasked(lockedStates, state))
		{
			stateFunctors[state](DeltaTime);
		}
	}
}

void UFSMComponent::RegisterStateFunction(const EFSMStatesMap& state, TFunction<void(float)> function)
{
	stateFunctors[state].Reset();
	stateFunctors[state] = function;
}

void UFSMComponent::UnregisterStateFunction(const EFSMStatesMap& state)
{
	stateFunctors[state].Reset();
	stateFunctors[state] = nullptr;
}

void UFSMComponent::ActivateState(const EFSMStatesMap& state)
{
	if (CheckBitFlagIsMasked(currentStates, state))
	{
		return;
	}
	currentStates |= state;
	stateStarts[state].ExecuteIfBound();
}

void UFSMComponent::DeactivateState(const EFSMStatesMap& state, const float& cooldownTime)
{
	if (cooldownTime > 0.f)
	{
		lockedStates |= state;
		GetWorld()->GetTimerManager().SetTimer(lockTimer, [&, state]()
		{
			lockedStates &= ~state;
		}, cooldownTime, false);
	}
	currentStates &= ~state;
	if (currentStates == EDefaultsStates::Default
		|| currentStates == EDefaultsStates::End)
	{
		currentStates = EDefaultsStates::Idle;
	}
	stateExits[state].ExecuteIfBound();
}


void UFSMComponent::ResetStates()
{
	currentStates = EDefaultsStates::Default;
	for (StateFunctor function : stateFunctors)
	{
		if (function.IsSet())
		{
			function.Reset();
		}
	}
}

