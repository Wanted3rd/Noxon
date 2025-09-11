// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseNPCManager.generated.h"
class ACharacter;


/**
 * 
 */
UCLASS()
class NOXON_API UBaseNPCManager : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void PullNPCsTransformsFromWorld() PURE_VIRTUAL(UBaseNPCManager::PullPositionsFromWorld, );
	virtual void PushNPCsTransformsForWorld() PURE_VIRTUAL(UBaseNPCManager::PushPositionsForWorld, );
	virtual void SaveNPCToArr(ACharacter* npc) PURE_VIRTUAL(UBaseNPCManager::PullPositionsFromWorld, );

protected:
	UPROPERTY(EditAnywhere)
	TArray<ACharacter*> npcArr;
};
