#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FSMComponent.generated.h"

#pragma region State Enum

// need to limit actions count under 15
// need to divide from this file


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

	virtual void ActivateState(const uint8& inPhase) {}
	virtual void DeactivateState(const EPhase& inPhase, const float& cooldownTime = -1.f);
	void ActivateMoveState(const EMoveState& inMoveState);
	void ActivateDamagedState(const EDamageState& inDamageState);

	void ResetStates();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 currentState = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 lockedState = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMoveState currentMoveState = EMoveState::Default;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageState currentDamageState = EDamageState::Default;

	FTimerHandle lockTimer;

private:
	UPROPERTY()
	ACharacter* owner = nullptr;
};
