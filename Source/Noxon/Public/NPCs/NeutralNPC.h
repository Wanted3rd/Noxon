// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseNonPlayableCharacter.h"
#include "NeutralNPC.generated.h"

UCLASS()
class NOXON_API ANeutralNPC : public ABaseNonPlayableCharacter
{
	GENERATED_BODY()

public:
	ANeutralNPC();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
};
