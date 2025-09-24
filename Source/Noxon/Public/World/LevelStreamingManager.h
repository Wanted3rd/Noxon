// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelInstance/LevelInstanceActor.h"
#include "LevelStreamingManager.generated.h"

class ARustLevelInstance;
class ULevelGraph;

USTRUCT(BlueprintType)
struct FManagedLevel
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName levelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> levelAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform levelTransform = FTransform::Identity;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ALevelInstance> levelInstance = nullptr;

	UPROPERTY(BlueprintReadOnly)
	bool bIsLoaded = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsVisible = false;
};

UCLASS()
class NOXON_API ALevelStreamingManager : public AActor
{
	GENERATED_BODY()

public:
	ALevelStreamingManager();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	bool LoadLevel(const FName& levelName);

	UFUNCTION(BlueprintCallable)
	bool UnloadLevel(const FName& levelName);

	UFUNCTION(BlueprintCallable)
	bool SetLevelVisibility(const FName& levelName, bool bVisible);

	UFUNCTION(BlueprintCallable)
	bool IsLevelLoaded(const FName& levelName) const;

	UFUNCTION(BlueprintCallable)
	bool IsLevelVisible(const FName& levelName) const;

	UFUNCTION(BlueprintCallable)
	TArray<FName> GetLoadedLevels() const;

	UFUNCTION(BlueprintCallable)
	void RegisterLevelInstance(ARustLevelInstance* levelInstance);

	UFUNCTION(BlueprintCallable)
	ARustLevelInstance* GetLevelInstance(const FName& levelName) const;

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerLocation(const FVector& playerLocation);

	UFUNCTION(BlueprintCallable)
	FName GetCurrentLevelName() const { return currentLevelName; }

	UFUNCTION(BlueprintCallable)
	void LoadAdjacentLevels(const FName& centerLevelName);

	UFUNCTION(BlueprintCallable)
	void UpdateLevelClearStatus(const FName& levelName, bool bCleared);

	UFUNCTION(BlueprintCallable)
	void SetLevelGraph(ULevelGraph* newLevelGraph);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Management")
	TArray<FManagedLevel> managedLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Graph")
	TObjectPtr<ULevelGraph> levelGraph;

private:
	UPROPERTY()
	TMap<FName, TObjectPtr<ARustLevelInstance>> levelInstances;

	UPROPERTY()
	TSet<FName> persistentLevels;

	FName currentLevelName;

	bool FindManagedLevel(const FName& levelName, FManagedLevel& outLevel);
	bool FindManagedLevel(const FName& levelName, FManagedLevel& outLevel) const;

	void DetermineCurrentLevel(const FVector& playerLocation);
	void LoadLevelFromGraph(int32 nodeID);
	void UnloadNonAdjacentLevels(const FName& centerLevelName);

	//UFUNCTION()
	//void OnLevelLoaded();
	//
	//UFUNCTION()
	//void OnLevelUnloaded();
};
