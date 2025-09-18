// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSMComponent.h"
#include "PhaseStateComponent.generated.h"

UENUM(BlueprintType, meta=(BitFlag))
enum class EPhase : uint8
{
	Default = 0 UMETA(Hidden),
	OrdinaryPhase = 1 << 4,
	Idle,
	LookAround,
	Rest,
	BattlePhase = 1 << 5,
	MeleeAttack,
	HipFire,
	AimingFire,
	InteractPhase = 1 << 6,
	Interact,
	End = 1 << 7 UMETA(Hidden)
};
ENUM_CLASS_FLAGS(EPhase)


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOXON_API UPhaseStateComponent : public UFSMComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPhaseStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void ActivateState(const EPhase& inPhase);

protected:
	
};
