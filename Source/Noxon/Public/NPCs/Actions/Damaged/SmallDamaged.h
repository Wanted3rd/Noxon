// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCs/Actions/StateAction.h"
#include "SmallDamaged.generated.h"

class UParticleSystem;
class USoundCue;

/**
 * 
 */
UCLASS()
class NOXON_API USmallDamaged : public UStateAction
{
	GENERATED_BODY()
public:
	virtual void OnBegin(ABaseNonPlayableCharacter* owner) override;
	virtual void OnTick(ABaseNonPlayableCharacter* owner, float deltaTime) override;
	virtual void OnEnd(ABaseNonPlayableCharacter* owner) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	TObjectPtr<UParticleSystem> bloodParticleSystem;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	TObjectPtr<USoundBase> hitSound;
};
