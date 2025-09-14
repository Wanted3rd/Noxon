// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Components/FSMComponent.h"

#include "GameFramework/Character.h"


UFSMComponent::UFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	owner = Cast<ACharacter>(GetOwner());
}

void UFSMComponent::BeginPlay()
{
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

}

void UFSMComponent::ActivatePhase(const EPhase& phase)
{
	if (phase == EPhase::End || phase == EPhase::Default)
	{
		return;
	}
	
	if (currentPhase > EPhase::InteractPhase)
	{
		if (EnumHasAnyFlags(lockedPhase, EPhase::InteractPhase))
		{
			return;
		}
		currentPhase = EPhase::InteractPhase;
	}
	else if (currentPhase > EPhase::BattlePhase)
	{
		if (EnumHasAnyFlags(lockedPhase, EPhase::BattlePhase))
		{
			return;
		}
		currentPhase = EPhase::BattlePhase;
	}
	else if (currentPhase > EPhase::OrdinaryPhase)
	{
		if (EnumHasAnyFlags(lockedPhase, EPhase::OrdinaryPhase))
		{
			return;
		}
		currentPhase = EPhase::OrdinaryPhase;
	}
	
	currentPhase |= phase;
}

void UFSMComponent::DeactivatePhase(const EPhase& phase, const float& cooldownTime)
{
	if (cooldownTime > 0.f)
	{
		lockedPhase |= phase;
		GetWorld()->GetTimerManager().SetTimer(lockTimer, [&, phase]()
		{
			lockedPhase &= ~phase;
		}, cooldownTime, false);
	}
	currentPhase &= ~phase;
}

void UFSMComponent::ActivateMoveState(const EMoveState& moveState)
{
	//if (currentPhase & EPhase)
}


void UFSMComponent::ResetStates()
{
	currentPhase = EPhase::Default;
}

