// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "StateAction.generated.h"

class UAnimMontage;
class ABaseNonPlayableCharacter;

/**
 * 
 */
UCLASS()
class NOXON_API UStateAction : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnBegin(ABaseNonPlayableCharacter* owner) PURE_VIRTUAL(UStateAction::OnStart, )

	UFUNCTION()
	virtual void OnTick(ABaseNonPlayableCharacter* owner, float deltaTime) PURE_VIRTUAL(UStateAction::OnTick, )

	UFUNCTION()
	virtual void OnEnd(ABaseNonPlayableCharacter* owner) PURE_VIRTUAL(UStateAcBaseNonPlayableCharactern::OnEnd, )
};
