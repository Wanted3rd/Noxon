#pragma once

#include "CoreMinimal.h"

// need to limit actions count under 15
// need to divide from this file
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
	Reloading,
	InteractPhase = 1 << 6,
	Interact,
	End = 1 << 7 UMETA(Hidden)
};
ENUM_CLASS_FLAGS(EPhase)

UENUM(BlueprintType)
enum class EDamageState : uint8
{
	Default = 0 UMETA(Hidden),
	SmallDamaged,
	LargeDamaged,
	Death,
	End UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMoveState : uint8
{
	Default = 0 UMETA(Hidden),
	OrdinaryPhase = 1 << 4,
	Stop,
	Patrol,
	BattlePhase = 1 << 5,
	Chase,
	Weave,
	Hide,
	End UMETA(Hidden)
};

