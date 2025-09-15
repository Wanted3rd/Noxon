#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utility/DebugHelper.h"
#include "FSMComponent.generated.h"

#pragma region State Enum

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

#pragma endregion State Enum

#pragma region Forward Declarations
class ACharacter;
class UStateAction;

#pragma endregion Forward Declarations


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOXON_API UFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFSMComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EPhase GetCurrentPhase() const {return currentPhase;}	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EMoveState GetCurrentMoveState() const {return currentMoveState;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EDamageState GetCurrentDamagedState() const {return currentDamageState;}

	void ActivatePhase(const EPhase& inPhase);
	void DeactivatePhase(const EPhase& inPhase, const float& cooldownTime = -1.f);
	void ActivateMoveState(const EMoveState& inMoveState);
	void ActivateDamagedState(const EDamageState& inDamageState);

	void ResetStates();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPhase currentPhase = EPhase::Default;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPhase lockedPhase = EPhase::Default;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMoveState currentMoveState = EMoveState::Default;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageState currentDamageState = EDamageState::Default;

	FTimerHandle lockTimer;

private:
	UPROPERTY()
	ACharacter* owner = nullptr;
};
