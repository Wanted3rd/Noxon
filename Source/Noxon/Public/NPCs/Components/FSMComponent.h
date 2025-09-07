// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FSMComponent.generated.h"

#pragma region Forward Declarations

typedef TFunction<void(float)> StateFunctor;

class ACharacter;

#pragma endregion Forward Declarations

#pragma region State Enum

enum EFSMStatesMap : uint8
{
	Idle = 0,
	Patrol = 1,
	Chase = 2,
	Attack = 3,
	Damaged = 4,
	Interact = 5,
	Dead = 6,
	End = 7 UMETA(Hidden),
};

UENUM(BlueprintType, meta=(Bitmask))
enum class EDefaultsStates : uint8  // phase 별로 나눈 후에 state 나누자
{
	Default = 0 << 0 UMETA(Hidden),
	Idle = 1 << 0,
	Patrol = 1 << 1,
	Chase = 1 << 2,
	Attack = 1 << 3,
	Damaged = 1 << 4,
	Interact = 1 << 5,
	Dead = 1 << 6,
	End = 1 << 7 UMETA(Hidden),
};
FORCEINLINE FName GetStateName(const uint8& stateMap)
{ return UEnum::GetValueAsName(static_cast<EDefaultsStates>(1 << stateMap)); }
FORCEINLINE EDefaultsStates operator|(const EDefaultsStates& L, const EDefaultsStates& R)
{ return static_cast<EDefaultsStates>(static_cast<uint8>(L) | static_cast<uint8>(R)); }
FORCEINLINE EDefaultsStates operator|(const EDefaultsStates& L, const uint8& R)
{ return static_cast<EDefaultsStates>(static_cast<uint8>(L) | static_cast<uint8>(1 << R)); }
FORCEINLINE void operator|=(EDefaultsStates& L, const EDefaultsStates& R)
{ L = L | R; }
FORCEINLINE void operator|=(EDefaultsStates& L, const uint8& R)
{ L = L | R; }
FORCEINLINE EDefaultsStates operator&(const EDefaultsStates& L, const EDefaultsStates& R)
{ return static_cast<EDefaultsStates>(static_cast<uint8>(L) & static_cast<uint8>(R)); }
FORCEINLINE EDefaultsStates operator&(const EDefaultsStates& L, const uint8& R)
{ return static_cast<EDefaultsStates>(static_cast<uint8>(L) & static_cast<uint8>(1 << R)); }
FORCEINLINE void operator&=(EDefaultsStates& L, const EDefaultsStates& R)
{ L = L & R; }
FORCEINLINE void operator&=(EDefaultsStates& L, const uint8& R)
{ L = L & R; }
FORCEINLINE bool IsActivated(const EDefaultsStates& stateBitFlag, const EDefaultsStates& stateBitMask)
{ return (stateBitFlag & stateBitMask) > EDefaultsStates::Default; }
FORCEINLINE bool IsActivated(const EDefaultsStates& stateBitFlag, const uint8& stateBitMask)
{ return (stateBitFlag & stateBitMask) > EDefaultsStates::Default; }

#pragma endregion State Enum


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOXON_API UFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFSMComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	#pragma region Functors
	
	virtual void RegisterStateFunction(const EFSMStatesMap& state, TFunction<void(float)> function);
	virtual void UnregisterStateFunction(const EFSMStatesMap& state, TFunction<void(float)> function);
	virtual void DeactivateState(const EFSMStatesMap& state, const float& cooldownTime = 0.f);
	#pragma endregion Functors
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void ResetStates();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDefaultsStates currentStates = EDefaultsStates::Default;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDefaultsStates previousStates = EDefaultsStates::Default;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDefaultsStates lockedStates = EDefaultsStates::Default;

	FTimerHandle lockTimer;
	
	#pragma region Functors
	//https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/uLangCore/uLang/Common/Containers/TFunction?application_version=5.6
	TArray<StateFunctor> stateFunctors;	
	/*TFunction<void(float)> IdleState;
	TFunction<void(float)> PatrolState;	
	TFunction<void(float)> ChaseState;
	TFunction<void(float)> AttackState;
	TFunction<void(float)> DamagedState;	
	TFunction<void(float)> InteractState;
	TFunction<void(float)> DeadState;*/
	#pragma endregion Functors

private:
	UPROPERTY()
	ACharacter* owner;
};
