// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameFlow/LevelStatus.h"
#include "IngameGameState.generated.h"

class UNPCManager;

USTRUCT()
struct FWorldProgressData
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FName, FLevelProgressData> levelsStatus;

	UPROPERTY()
	FVector lastPlayerLocation;

	UPROPERTY()
	FString lastActiveLevel;
};

UCLASS()
class NOXON_API AIngameGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateLevelProgress(const FName& inLevelName, const FLevelProgressData& inProgressData);

protected:
	UPROPERTY()
	FWorldProgressData worldProgress;
	UPROPERTY(BlueprintReadOnly)
	FName currentLevel;

private:
	FTimerHandle checkFinishTimer;
};
