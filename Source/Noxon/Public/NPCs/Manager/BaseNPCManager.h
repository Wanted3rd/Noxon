// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseNPCManager.generated.h"

/**
 * 
 */
UCLASS()
class NOXON_API UBaseNPCManager : public UObject
{
	GENERATED_BODY()
	
protected:
	UFUNCTION()
	virtual void PullPositionsFromWorld() PURE_VIRTUAL(UBaseNPCManager::PullPositionsFromWorld(), );
	UFUNCTION()
	virtual void SaveToDataTable() PURE_VIRTUAL(UBaseNPCManager::PullPositionsFromWorld(), );

protected:
	UPROPERTY(EditAnywhere)
	TArray<ACharacter*> npcArr;
};
