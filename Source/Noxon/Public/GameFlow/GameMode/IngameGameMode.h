// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IngameGameMode.generated.h"


class UEnemyManager;
class UNeutralManager;

/**
 * 
 */
UCLASS()
class NOXON_API AIngameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AIngameGameMode();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UEnemyManager> EnemyManager;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNeutralManager> NeutralManager;
	
};
