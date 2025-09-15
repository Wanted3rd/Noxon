// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseNonPlayableCharacter.h"
#include "Enemy.generated.h"

#pragma region Forward Declarations

class UFSMComponent;
class AHandItem;
class UStateAction;

#pragma endregion Forward Declarations


UCLASS()
class NOXON_API AEnemy : public ABaseNonPlayableCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	// item


	// coord in worldMap (table, rand(0~row end) )
};
