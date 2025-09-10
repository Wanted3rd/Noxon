// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseNonPlayableCharacter.h"
#include "Enemy.generated.h"

#pragma region Forward Declarations

class UFSMComponent;
class AHandItem;

#pragma endregion Forward Declarations


UCLASS()
class NOXON_API AEnemy : public ABaseNonPlayableCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual void RegisterFSMActions() override;
private:


protected:
	// item
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TObjectPtr<AHandItem> handItem = nullptr;

	// coord in worldMap (table, rand(0~row end) )

private:
	float time = 0.f;
	uint8 count = 0;
};
