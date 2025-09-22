// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCs/Actions/StateAction.h"
#include "DeadlyDamaged.generated.h"

/**
 * 
 */
UCLASS()
class NOXON_API UDeadlyDamaged : public UStateAction
{
	GENERATED_BODY()

public:
	UDeadlyDamaged();
	
	virtual void OnBegin(ABaseNonPlayableCharacter* owner) override;
	virtual void OnTick(ABaseNonPlayableCharacter* owner, float deltaTime) override;
	virtual void OnEnd(ABaseNonPlayableCharacter* owner) override;

protected:
	UPROPERTY()
	TObjectPtr<UAnimMontage> deadMontage;

	UPROPERTY()
	FName deadMontageEndSectionName = FName("");
};
