// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseNPCManager.h"
#include "EnemyManager.generated.h"

/**
 * 
 */
UCLASS()
class NOXON_API UEnemyManager : public UBaseNPCManager
{
	GENERATED_BODY()

public:
	virtual void PullNPCsTransformsFromWorld() override;
	virtual void SaveNPCToArr(ACharacter* npc) override;
};
