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
	FName lastActiveLevel;
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
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FName GetActiveLevelName() {return worldProgress.lastActiveLevel;}

	UFUNCTION(BlueprintCallable)
	void SetActiveLevelName(const FName& levelName);

	UFUNCTION(BlueprintCallable)
	bool GetCurrentLevelQuests(TArray<FLevelCompleteCondition>& outConditions) const;

	UFUNCTION(BlueprintCallable)
	bool IsAllLevelsCleared() const;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveLevelChanged, FName, newLevelName);
	UPROPERTY(BlueprintAssignable)
	FOnActiveLevelChanged OnActiveLevelChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllLevelsCleared);
	UPROPERTY(BlueprintAssignable)
	FOnAllLevelsCleared OnAllLevelsCleared;

protected:
	UPROPERTY()
	FWorldProgressData worldProgress;

private:
};
