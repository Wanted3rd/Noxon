// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFlow/GameMode/IngameGameState.h"

#include "GameFlow/GameMode/IngameGameMode.h"
#include "GameFlow/LevelStatus.h"

void AIngameGameState::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(checkFinishTimer, [&]()->void
	{
		if (GetDefaultGameMode<AIngameGameMode>()->GetEnemyCount() <= 0)
		{
			
		}
	}, 1.f, true);
}

void AIngameGameState::UpdateLevelProgress(const FName& inLevelName, const FLevelProgressData& inProgressData)
{
	worldProgress.levelsStatus[inLevelName] = inProgressData;
}
