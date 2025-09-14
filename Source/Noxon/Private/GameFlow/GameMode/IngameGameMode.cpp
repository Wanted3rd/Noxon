// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFlow/GameMode/IngameGameMode.h"

#include "Player/MainPlayer.h"


AIngameGameMode::AIngameGameMode()
{
	static ConstructorHelpers::FClassFinder<AMainPlayer> tempPlayer(TEXT("/Game/Player/BP_MainPlayer"));
	if (tempPlayer.Succeeded())
	{
		DefaultPawnClass = tempPlayer.Class;
	}
	
}

void AIngameGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}
