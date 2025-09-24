// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/World/RustLevelInstance.h"

#include "GameFlow/GameMode/IngameGameState.h"
#include "GameFlow/GameMode/IngameGameMode.h"
#include "World/LevelStreamingManager.h"
#include "Kismet/GameplayStatics.h"


void ARustLevelInstance::BeginPlay()
{
	Super::BeginPlay();
	progressData.firstEnterTime = GetWorld()->GetTimeSeconds();

	if (AIngameGameMode* gameMode = Cast<AIngameGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (ALevelStreamingManager* streamingManager = gameMode->GetLevelStreamingManager())
		{
			streamingManager->RegisterLevelInstance(this);
		}
	}
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
		return true;
	}
	outConditions = progressData.completeConditions;
	for (FLevelCompleteCondition condition : progressData.completeConditions)
	{
		if (!condition.IsCompleted())
		{
			return false;
		}
	}
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
