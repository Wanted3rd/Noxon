// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelInstance/LevelInstanceActor.h"
#include "GameFlow/LevelStatus.h"
#include "RustLevelInstance.generated.h"

//DECLARE_MULTICAST_DELEGATE(FOnLevelComplete);

UCLASS()
class NOXON_API ARustLevelInstance : public ALevelInstance
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	UFUNCTION(BlueprintCallable)
	bool CheckCompleteConditions();
	UFUNCTION(BlueprintCallable)
	bool GetLevelCompleteConditions(TArray<FLevelCompleteCondition>& outConditions);
	
private:
	void CompleteLevel();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLevelProgressData progressData;

	UPROPERTY(BlueprintReadOnly)
	bool bCompleted = false;

	//FOnLevelComplete OnLevelComplete;
};
