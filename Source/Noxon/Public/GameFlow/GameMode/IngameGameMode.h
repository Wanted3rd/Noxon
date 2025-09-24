// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IngameGameMode.generated.h"

/*
 IF it displays ERROR, Check about including "Kismet/GameplayStatics.h"
 OR, Check about Calling this Macro without World.
 */
#define GM_INGAME Cast<AIngameGameMode>(UGameplayStatics::GetGameMode(GetWorld()))

class UNPCManager;
class ABaseNonPlayableCharacter;
class ALevelStreamingManager;
class AGameFinishManager;
class ULevelGraph;

UCLASS()
class NOXON_API AIngameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AIngameGameMode();

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	void RegisterNpc(ABaseNonPlayableCharacter* npc) const;
	void UnregisterNpc(ABaseNonPlayableCharacter* npc) const;

	UFUNCTION(BlueprintPure)
	int32 GetEnemyCount() const;

	UFUNCTION(BlueprintPure)
	ALevelStreamingManager* GetLevelStreamingManager() const { return levelStreamingManager; }

	UFUNCTION(BlueprintPure)
	AGameFinishManager* GetGameFinishManager() const { return gameFinishManager; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Level System")
	TObjectPtr<ULevelGraph> defaultLevelGraph;

	UPROPERTY()
	TObjectPtr<UNPCManager> npcManager;

	UPROPERTY()
	TObjectPtr<ALevelStreamingManager> levelStreamingManager;

	UPROPERTY()
	TObjectPtr<AGameFinishManager> gameFinishManager;
};
