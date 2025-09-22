// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Components/PerceptionComponent.h"

#include "NPCs/BaseNonPlayableCharacter.h"
#include "NPCs/Components/FSMComponent.h"
#include "NPCs/Datas/StateEnums.h"
#include "NPCs/Manager/NPCManager.h"


UPerceptionComponent::UPerceptionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPerceptionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPerceptionComponent::PostApplyToComponent()
{
	Super::PostApplyToComponent();
	owner = Cast<ABaseNonPlayableCharacter>(GetOwner());
}


// Called every frame
void UPerceptionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsValid(owner))
	{
		view =  owner->GetMesh()->GetSocketTransform("head");
	}
	// ...
}

void UPerceptionComponent::ReceiveContext(const FProximityCheckContext& context)
{
	if (context.dist < perceptionProperties.noticeRange)
	{
		if (Look(context))
		{
			FHitResult hitResult;
			FVector start = view.GetLocation();
			FVector end = start + view.GetUnitAxis(EAxis::X) * perceptionProperties.noticeRange;
			if (owner->ActorLineTraceSingle(hitResult, start, end, ECollisionChannel::ECC_WorldDynamic, FCollisionQueryParams()))
			{
				if (hitResult.GetActor() == context.target) 
				{
					targetPawn = context.target;
				}
			}
		}
	}
	else if (targetPawn)
	{
		if (context.dist < perceptionProperties.meleeAttackRange)
		{
			owner->GetFSMComp()->ActivateMoveState(EMoveState::Hide);
		}
		else if (context.dist < perceptionProperties.gunFireRange)
		{
			owner->GetFSMComp()->ActivatePhaseState(EPhase::HipFire);
		}
		else
		{
			owner->GetFSMComp()->ActivateMoveState(EMoveState::Chase);
		}
	}
}

void UPerceptionComponent::ResetPerception()
{
	targetPawn = nullptr;
}

bool UPerceptionComponent::Look(const FProximityCheckContext& context)
{
	return view.GetUnitAxis(EAxis::X).Dot(context.direction) < perceptionProperties.viewAngleLimit;
}

