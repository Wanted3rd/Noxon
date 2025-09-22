// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCs/Actions/StateAction.h"
#include "HipFireAction.generated.h"

/**
 * 
 */
UCLASS()
class NOXON_API UHipFireAction : public UStateAction
{
	GENERATED_BODY()
public:
	virtual void OnBegin(ABaseNonPlayableCharacter* owner) override;
	virtual void OnTick(ABaseNonPlayableCharacter* owner, float deltaTime) override;
	virtual void OnEnd(ABaseNonPlayableCharacter* owner) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TObjectPtr<UAnimMontage> hipFireMontage;
};
