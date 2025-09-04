// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFlow/GameMode/IngameGameMode.h"

#include "NPCs/Manager/EnemyManager.h"
#include "NPCs/Manager/NeutralManager.h"
#include "Utility/FindHelper.h"

AIngameGameMode::AIngameGameMode()
{
	//DefaultPawnClass = ;
	
	EnemyManager = CreateDefaultSubobject<UEnemyManager>("EnemyManager");
	NeutralManager = CreateDefaultSubobject<UNeutralManager>("NeutralManager");
	
}

void AIngameGameMode::BeginPlay()
{
	Super::BeginPlay();

}
