// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Components/ActionComponent.h"

#include "Items/HandItems/HandItem.h"
#include "NPCs/BaseNonPlayableCharacter.h"
#include "NPCs/Actions/StateAction.h"


UActionComponent::UActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UActionComponent::PostApplyToComponent()
{
	Super::PostApplyToComponent();
	owner = Cast<ABaseNonPlayableCharacter>(GetOwner());
}

void UActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	DamagedTick(DeltaTime);
	if (!owner->IsDead())
	{
		PhaseTick(DeltaTime);
		MoveTick(DeltaTime);
	}
}

void UActionComponent::ResetActions()
{
	goalLocation = FVector::ZeroVector;
	currentPhaseAction = nullptr;
	currentMoveAction = nullptr;
	currentDamagedAction = nullptr;
	prevPhaseAction = nullptr;
	prevMoveAction = nullptr;
	prevDamagedAction = nullptr;
}

bool UActionComponent::GetGoalLocation(FVector& outLocation)
{
	outLocation = goalLocation;
	return FVector::Dist(goalLocation, owner->GetActorLocation()) < 5.f;
}

void UActionComponent::PhaseTick(float deltaTime)
{
	if (currentPhaseAction != prevPhaseAction)
	{
		if (prevPhaseAction)
		{
			prevPhaseAction->OnEnd(owner);
		}
		prevPhaseAction = currentPhaseAction;
		if (currentPhaseAction)
		{
			currentPhaseAction->OnBegin(owner);
		}
		
		return;
	}

	if (currentPhaseAction)
	{
		currentPhaseAction->OnTick(owner, deltaTime);
	}
}

void UActionComponent::MoveTick(float deltaTime)
{
	if (currentMoveAction != prevMoveAction)
	{
		if (prevMoveAction)
		{
			prevMoveAction->OnEnd(owner);
		}
		prevMoveAction = currentMoveAction;
		if (currentMoveAction)
		{
			currentMoveAction->OnBegin(owner);
		}
		
		return;
	}

	if (currentMoveAction)
	{
		currentMoveAction->OnTick(owner, deltaTime);
	}
}

void UActionComponent::DamagedTick(float deltaTime)
{
	if (currentDamagedAction != prevDamagedAction)
	{
		if (prevDamagedAction)
		{
			prevDamagedAction->OnEnd(owner);
		}
		prevDamagedAction = currentDamagedAction;
		if (currentDamagedAction)
		{
			currentDamagedAction->OnBegin(owner);
		}
		return;
	}

	if (currentDamagedAction)
	{
		currentDamagedAction->OnTick(owner, deltaTime);
	}
}

void UActionComponent::RandPos()
{
	FVector2D randPos = FVector2D::ZeroVector;
	FVector2D initPos = {owner->GetInitTransform().GetLocation().X, owner->GetInitTransform().GetLocation().Y};
	FVector start = FVector::ZeroVector;
	FVector end = FVector::ZeroVector;
	FVector groundLocation = owner->GetActorLocation();
	
	FHitResult hitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	int threshold = 3;
	while (threshold > 0)
	{
		randPos = initPos + FMath::RandPointInCircle(600.f);
		start = FVector(randPos.X, randPos.Y, 10000.0f);
		end = FVector(randPos.X, randPos.Y, -10000.0f);
		
		if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_WorldStatic, QueryParams))
		{
			if (FMath::Abs(hitResult.Location.Z - owner->GetActorLocation().Z) < 600.f)
			{
				groundLocation = hitResult.Location;
				break;
			}
		}
		--threshold;
	}
	goalLocation = groundLocation;
}