// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StateAction.generated.h"

/**
 * 
 */
UCLASS()
class NOXON_API UStateAction : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnBegin() PURE_VIRTUAL(UStateAction::OnStart, )

	UFUNCTION()
	virtual void OnTick(float deltaTime) PURE_VIRTUAL(UStateAction::OnTick, )

	UFUNCTION()
	virtual void OnEnd() PURE_VIRTUAL(UStateAction::OnEnd, )
};
