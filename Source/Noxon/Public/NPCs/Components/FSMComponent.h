#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FSMComponent.generated.h"



#pragma region Forward Declarations
enum class EPhase : uint8;
enum class EDamageState : uint8;
enum class EMoveState : uint8;
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
	FORCEINLINE EPhase GetCurrentPhase() const {return currentPhaseState;}	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EMoveState GetCurrentMoveState() const {return currentMoveState;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EDamageState GetCurrentDamagedState() const {return currentDamageState;}

	void ActivatePhaseState(const EPhase& inPhaseState);
	void DeactivatePhaseState(const EPhase& inPhase, const float& cooldownTime = -1.f);
	void ActivateMoveState(const EMoveState& inMoveState);
	void ActivateDamagedState(const EDamageState& inDamageState);

	void ResetStates();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPhase currentPhaseState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPhase lockedPhaseState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMoveState currentMoveState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageState currentDamageState;

	FTimerHandle lockTimer;

private:
	UPROPERTY()
	ACharacter* owner = nullptr;
};
