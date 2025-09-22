// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/World/RustLevelInstance.h"

#include "GameFlow/GameMode/IngameGameState.h"


void ARustLevelInstance::BeginPlay()
{
	Super::BeginPlay();
	progressData.firstEnterTime = GetWorld()->GetTimeSeconds();
}

bool ARustLevelInstance::CheckCompleteConditions()
{
	for (const FLevelCompleteCondition& completeInfo : progressData.completeConditions)
	{
		if (!completeInfo.IsCompleted())
		{
			return false;
		}
	}

	if (!bCompleted)
	{
		//if (OnLevelComplete.IsBound())
		//{
		//	OnLevelComplete.Broadcast();
		//}
		CompleteLevel();
	}
	
	return bCompleted;
}

bool ARustLevelInstance::GetLevelCompleteConditions(TArray<FLevelCompleteCondition>& outConditions)
{
	if (progressData.completeConditions.IsEmpty())
	{
		return false;
	}
	outConditions = progressData.completeConditions;
	return true;
}

void ARustLevelInstance::CompleteLevel()
{
	bCompleted = true;
	progressData.clearTime = GetWorld()->GetTimeSeconds();
	if (AIngameGameState* gameState = GetWorld()->GetGameState<AIngameGameState>())
	{
		gameState->UpdateLevelProgress(FName(GetActorNameOrLabel()), progressData);
	}
}
