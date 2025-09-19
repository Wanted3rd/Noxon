// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCs/Actions/StateAction.h"
#include "ReloadingAction.generated.h"

/**
 * 
 */
UCLASS()
class NOXON_API UReloadingAction : public UStateAction
{
	GENERATED_BODY()

public:
	virtual void OnBegin(ABaseNonPlayableCharacter* owner) override;
	virtual void OnTick(ABaseNonPlayableCharacter* owner, float deltaTime) override;
	virtual void End(ABaseNonPlayableCharacter* owner);
};
