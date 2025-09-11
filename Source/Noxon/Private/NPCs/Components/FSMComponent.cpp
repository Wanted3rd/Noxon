// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Components/FSMComponent.h"

#include "Engine/PlatformInterfaceBase.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"


UFSMComponent::UFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	owner = Cast<ACharacter>(GetOwner());
	stateFunctors.Init(nullptr, EFSMStatesMap::End + 1);
	stateStarts.AddZeroed(EFSMStatesMap::End + 1);
	for (int i = 0; i < EFSMStatesMap::End + 1; ++i)
	{
		stateStarts[i] = MakeShared<FOnStateStart>();
	}
	stateExits.AddZeroed(EFSMStatesMap::End + 1);
	for (int i = 0; i < EFSMStatesMap::End + 1; ++i)
	{
		stateExits[i] = MakeShared<FOnStateExit>();
	}
	
	//stateStarts.Init(FOnStateStart(), EFSMStatesMap::End + 1);
	//stateExits.Init(FOnStateExit(), EFSMStatesMap::End + 1);
}

void UFSMComponent::BeginPlay()
{
	Super::BeginPlay();
	currentStates = EDefaultsStates::Idle;
	lockedStates = EDefaultsStates::Default;
	
}

void UFSMComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ResetStates();
	Super::EndPlay(EndPlayReason);
}

void UFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{	
	for (uint8 state = 0; state < EFSMStatesMap::End; ++state)
	{
		if (!stateFunctors[state].IsSet())
		{
			PRINT_TEXT(18888, 1.f, TEXT("%s is not Bound"), *(GetStateName(state).ToString()));
		}
		else if (CheckBitFlagIsMasked(currentStates, state) && !CheckBitFlagIsMasked(lockedStates, state))
		{
			stateFunctors[state](DeltaTime);
		}
	}
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UFSMComponent::RegisterStateFunction(const EFSMStatesMap& state, TFunction<void(float)> function)
{
	stateFunctors[state] = function;
}

void UFSMComponent::UnregisterStateFunction(const EFSMStatesMap& state)
{
	stateFunctors[state] = nullptr;
}


void UFSMComponent::ActivateState(const EFSMStatesMap& state)
{
	//PRINT_TEXT(18, 1.f, TEXT("Activate State"));
	if (CheckBitFlagIsMasked(currentStates, state))
	{
		return;
	}
	//PRINT_TEXT(181818, 1.f, TEXT("State is Started"));
	currentStates |= state;
	if (stateStarts[state])
	{
		stateStarts[state]->ExecuteIfBound();
	}
	//stateStarts[state].ExecuteIfBound();
}

void UFSMComponent::DeactivateState(const EFSMStatesMap& state, const float& cooldownTime)
{
	if (!CheckBitFlagIsMasked(currentStates, state))
	{
		return;
	}
	if (cooldownTime > 0.f)
	{
		lockedStates |= state;
		GetWorld()->GetTimerManager().SetTimer(lockTimer, [&, state]()
		{
			lockedStates &= ~state;
		}, cooldownTime, false);
	}
	currentStates &= ~state;
	if (stateExits[state])
	{
		stateExits[state]->ExecuteIfBound();
		
	}
	//stateExits[state].ExecuteIfBound();
	
	if (currentStates == EDefaultsStates::Default
		|| currentStates == EDefaultsStates::End)
	{
		currentStates = EDefaultsStates::Idle;
	}
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
	stateStarts.Empty();
	stateExits.Empty();
}

