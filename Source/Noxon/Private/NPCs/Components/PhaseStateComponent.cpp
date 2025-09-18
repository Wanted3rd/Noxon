// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Components/PhaseStateComponent.h"


// Sets default values for this component's properties
UPhaseStateComponent::UPhaseStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPhaseStateComponent::BeginPlay()
{
	Super::BeginPlay();

	currentState = static_cast<uint8>(EPhase::Default);
}


// Called every frame
void UPhaseStateComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (currentPhase == EPhase::Default)
	{
		ActivateState(EPhase::Idle);
	}
	// ...
}

void UPhaseStateComponent::ActivateState(const EPhase& inPhase)
{
	if (inPhase == EPhase::Default)
	{
		return;
	}

	if (currentPhase > EPhase::InteractPhase)
	{
		if (EnumHasAnyFlags(lockedPhase, EPhase::InteractPhase))
		{
			return;
		}
	}
	else if (currentPhase > EPhase::BattlePhase)
	{
		if (EnumHasAnyFlags(lockedPhase, EPhase::BattlePhase))
		{
			return;
		}
	}
	else if (currentPhase > EPhase::OrdinaryPhase)
	{
		if (EnumHasAnyFlags(lockedPhase, EPhase::OrdinaryPhase))
		{
			return;
		}
	}
	
	currentPhase |= inPhase;
}

