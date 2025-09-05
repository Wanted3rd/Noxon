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


	UFUNCTION(BlueprintCallable)
	FORCEINLINE UEnemyManager* GetEnemyManager() const {return EnemyManager;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UNeutralManager* GetNeutralManager() const {return NeutralManager;}

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UEnemyManager> EnemyManager;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNeutralManager> NeutralManager;
	
};
