// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFlow/GameMode/IngameGameMode.h"
#include "GameFlow/GameMode/IngameGameState.h"
#include "NPCs/Manager/NPCManager.h"
#include "World/LevelStreamingManager.h"
#include "GameFlow/GameFinishManager.h"
#include "World/LevelGraph.h"

#include "Player/MainPlayer.h"
#include "Utility/DebugHelper.h"
#include "Utility/FindHelper.h"


AIngameGameMode::AIngameGameMode()
{
	static ConstructorHelpers::FClassFinder<AMainPlayer> tempPlayer(TEXT("/Game/Player/BP_MainPlayer"));
	if (tempPlayer.Succeeded())
	{
		DefaultPawnClass = tempPlayer.Class;
	}
	GameStateClass = AIngameGameState::StaticClass();
	defaultLevelGraph = FinderHelper::FindAssetFromConstructor<ULevelGraph>(TEXT("/Game/Maps/Game/Data/DA_LevelGraph.DA_LevelGraph"));
	
}

void AIngameGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
}

void AIngameGameMode::RegisterNpc(ABaseNonPlayableCharacter* npc) const
{
	npcManager->RegisterNPC(npc);
}

void AIngameGameMode::UnregisterNpc(ABaseNonPlayableCharacter* npc) const
{
	npcManager->DestroyNPC(npc);
}

int32 AIngameGameMode::GetEnemyCount() const
{
	return npcManager->GetEnemyCount();
}

void AIngameGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!levelStreamingManager)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Name = TEXT("LevelStreamingManager");
		levelStreamingManager = GetWorld()->SpawnActor<ALevelStreamingManager>(spawnParams);

		if (levelStreamingManager)
		{
			LOG_TEXT(TEXT("Level Streaming Manager created successfully"));

			if (defaultLevelGraph)
			{
				levelStreamingManager->SetLevelGraph(defaultLevelGraph);
				LOG_TEXT(TEXT("LevelGraph assigned to LevelStreamingManager"));
			}
			else
			{
				LOG_TEXT(TEXT("Warning: defaultLevelGraph is not set in IngameGameMode"));
			}
		}
	}

	if (!gameFinishManager)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Name = TEXT("GameFinishManager");
		gameFinishManager = GetWorld()->SpawnActor<AGameFinishManager>(spawnParams);

		if (gameFinishManager)
		{
			LOG_TEXT(TEXT("Game Finish Manager created successfully"));
		}
	}
}
