// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFlow/GameMode/IngameGameMode.h"

#include "Player/MainPlayer.h"

#include "NPCs/Manager/EnemyManager.h"
#include "NPCs/Manager/NeutralManager.h"

AIngameGameMode::AIngameGameMode()
{
	static ConstructorHelpers::FClassFinder<AMainPlayer> tempPlayer(TEXT("/Game/Player/BP_MainPlayer"));
	if (tempPlayer.Succeeded())
	{
		DefaultPawnClass = tempPlayer.Class;
	}
	
	EnemyManager = CreateDefaultSubobject<UEnemyManager>("EnemyManager");
	NeutralManager = CreateDefaultSubobject<UNeutralManager>("NeutralManager");
	
}

void AIngameGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}
