// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFlow/GameMode/IngameGameState.h"

#include "GameFlow/GameMode/IngameGameMode.h"
#include "GameFlow/LevelStatus.h"
#include "World/LevelStreamingManager.h"
#include "World/RustLevelInstance.h"

void AIngameGameState::BeginPlay()
{
	Super::BeginPlay();

}

void AIngameGameState::UpdateLevelProgress(const FName& inLevelName, const FLevelProgressData& inProgressData)
{
	worldProgress.levelsStatus[inLevelName] = inProgressData;

	if (AIngameGameMode* gameMode = GetWorld()->GetAuthGameMode<AIngameGameMode>())
	{
		if (ALevelStreamingManager* streamingManager = gameMode->GetLevelStreamingManager())
		{
			bool bLevelCleared = true;
			for (const FLevelCompleteCondition& condition : inProgressData.completeConditions)
			{
				if (!condition.IsCompleted())
				{
					bLevelCleared = false;
					break;
				}
			}

			streamingManager->UpdateLevelClearStatus(inLevelName, bLevelCleared);

			if (bLevelCleared && IsAllLevelsCleared())
			{
				OnAllLevelsCleared.Broadcast();
			}
		}
	}
}

void AIngameGameState::SetActiveLevelName(const FName& levelName)
{
	if (worldProgress.lastActiveLevel != levelName)
	{
		worldProgress.lastActiveLevel = levelName;
		OnActiveLevelChanged.Broadcast(levelName);
	}
}

bool AIngameGameState::GetCurrentLevelQuests(TArray<FLevelCompleteCondition>& outConditions) const
{
	outConditions.Empty();

	if (worldProgress.lastActiveLevel.IsNone())
	{
		return false;
	}

	if (AIngameGameMode* gameMode = GetWorld()->GetAuthGameMode<AIngameGameMode>())
	{
		if (ALevelStreamingManager* streamingManager = gameMode->GetLevelStreamingManager())
		{
			if (ARustLevelInstance* levelInstance = streamingManager->GetLevelInstance(worldProgress.lastActiveLevel))
			{
				return levelInstance->GetLevelCompleteConditions(outConditions);
			}
		}
	}

	return false;
}

bool AIngameGameState::IsAllLevelsCleared() const
{
	if (AIngameGameMode* gameMode = GetWorld()->GetAuthGameMode<AIngameGameMode>())
	{
		if (ALevelStreamingManager* streamingManager = gameMode->GetLevelStreamingManager())
		{
			// This would need to check the level graph for all quest levels
			// For now, we'll check all levels in our progress data
			for (const auto& levelPair : worldProgress.levelsStatus)
			{
				const FLevelProgressData& progressData = levelPair.Value;
				if (!progressData.completeConditions.IsEmpty())
				{
					for (const FLevelCompleteCondition& condition : progressData.completeConditions)
					{
						if (!condition.IsCompleted())
						{
							return false;
						}
					}
				}
			}
		}
	}

	return true;
}
