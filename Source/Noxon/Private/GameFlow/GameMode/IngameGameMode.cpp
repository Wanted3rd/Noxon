// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFlow/GameMode/IngameGameMode.h"
#include "GameFlow/GameMode/IngameGameState.h"
#include "NPCs/Manager/NPCManager.h"

#include "Player/MainPlayer.h"


AIngameGameMode::AIngameGameMode()
{
	static ConstructorHelpers::FClassFinder<AMainPlayer> tempPlayer(TEXT("/Game/Player/BP_MainPlayer"));
	if (tempPlayer.Succeeded())
	{
		DefaultPawnClass = tempPlayer.Class;
	}
	GameStateClass = AIngameGameState::StaticClass();
}

void AIngameGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
}

void AIngameGameMode::RegisterNpc(ABaseNonPlayableCharacter* npc)
{
	npcManager->RegisterNPC(npc);
}

void AIngameGameMode::UnregisterNpc(ABaseNonPlayableCharacter* npc)
{
	npcManager->DestroyNPC(npc);
}

void AIngameGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}
