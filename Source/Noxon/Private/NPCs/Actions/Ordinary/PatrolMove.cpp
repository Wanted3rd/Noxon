// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Actions/Ordinary/PatrolMove.h"
#include "NPCs/BaseNonPlayableCharacter.h"
#include "NPCs/Components/FSMComponent.h"

void UPatrolMove::OnBegin(ABaseNonPlayableCharacter* owner)
{
	float zValue = owner->GetActorLocation().Z;
	FHitResult hitResult;
	FVector2D randPos = FVector2D::ZeroVector;
	FVector start = FVector::ZeroVector;
	FVector end = FVector::ZeroVector;
	FVector groundLocation = FVector::ZeroVector;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	while (FMath::Abs(zValue - owner->GetActorLocation().Z) > 600.f)
	{
		randPos = FMath::RandPointInCircle(600.f);
		start = FVector(randPos.X, randPos.Y, 10000.0f); // 높은 곳에서 시작
		end = FVector(randPos.X, randPos.Y, -10000.0f);  // 아래쪽으로
		
		if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_WorldStatic, QueryParams))
		{
			groundLocation = hitResult.Location;
			zValue = groundLocation.Z;
		}
	}
	owner->SetGoalLocation(groundLocation);
}

void UPatrolMove::OnTick(ABaseNonPlayableCharacter* owner, float deltaTime)
{
	FHitResult hitResult;
	owner->MoveBlockedBy(hitResult);
	if (hitResult.ImpactPoint.Z > owner->GetActorLocation().Z + 60.f)
	{
		owner->GetFSMComponent()->ActivatePhase(EPhase::Idle);
	}
	owner->GetGoalLocation();
}

void UPatrolMove::OnEnd(ABaseNonPlayableCharacter* owner)
{
}
