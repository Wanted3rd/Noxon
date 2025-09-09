#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utility/DebugHelper.h"
#include "FSMComponent.generated.h"

#pragma region State Enum

enum EFSMStatesMap : uint8
{
	Idle = 0,
	Moving = 1, 
	Weaving = 2,
	Attack = 3,
	Damaged = 4,
	Interact = 5,
	Dead = 6,
	End = 7 UMETA(Hidden),
};

UENUM(BlueprintType)
enum class EPhase : uint8
{
	Default = 0 UMETA(Hidden),
	OrdinaryPhase,
	BattlePhase,
	InteractPhase,
	DeathPhase
};

UENUM(BlueprintType, meta=(Bitmask))
enum class EDefaultsStates : uint8  // phase 별로 나눈 후에 state 나누는게..?
{
	Default = 0 << 0 UMETA(Hidden),
	Idle = 1 << 0,
	Move = 1 << 1,
	Weaving = 1 << 2,
	Attack = 1 << 3,
	Damaged = 1 << 4,
	Interact = 1 << 5,
	Dead = 1 << 6,
	End = 1 << 7 UMETA(Hidden),
};


UENUM(BlueprintType, meta=(Bitmask))
enum class EOrdinaryPhase : uint8
{
	Default = 0 << 0 UMETA(Hidden),
	Idle = 1 << 0,
	Patrol = 1 << 1,
	LookingAround = 1 << 2,
	Rest = 1 << 3,
	Damaged = 1 << 4,
	End = 1 << 5 UMETA(Hidden),
};
UENUM(BlueprintType, meta=(Bitmask))
enum class EBattlePhase : uint8
{
	Default = 0 << 0 UMETA(Hidden),
	Aiming = 1 << 0,
	Chasing = 1 << 1,
	Weaving = 1 << 2,
	Attack = 1 << 3,
	Damaged = 1 << 4,
	End = 1 << 7 UMETA(Hidden),
};

//template<typename EnumType = EDefaultsStates>
FORCEINLINE FName GetStateName(const uint8& stateMap) { return UEnum::GetValueAsName(static_cast<EDefaultsStates>(1 << stateMap)); }

//template<typename EnumType = EDefaultsStates>
FORCEINLINE EDefaultsStates operator|(const EDefaultsStates& L, const EDefaultsStates& R){ return static_cast<EDefaultsStates>(static_cast<uint8>(L) | static_cast<uint8>(R)); }
//template<typename EnumType = EDefaultsStates>
FORCEINLINE void operator|=(EDefaultsStates& L, const EDefaultsStates& R) { L = L | R; }
//template<typename EnumType = EDefaultsStates>
FORCEINLINE EDefaultsStates operator|(const EDefaultsStates& L, const uint8& R){ return static_cast<EDefaultsStates>(static_cast<uint8>(L) | static_cast<uint8>(1 << R)); }
//template<typename EnumType = EDefaultsStates>
FORCEINLINE void operator|=(EDefaultsStates& L, const uint8& R) { L = L | R; }

//template<typename EnumType = EDefaultsStates>
FORCEINLINE EDefaultsStates operator&(const EDefaultsStates& L, const EDefaultsStates& R){ return static_cast<EDefaultsStates>(static_cast<uint8>(L) & static_cast<uint8>(R)); }
//template<typename EnumType = EDefaultsStates>
FORCEINLINE void operator&=(EDefaultsStates& L, const EDefaultsStates& R) { L = L & R; }
//template<typename EnumType = EDefaultsStates>
FORCEINLINE EDefaultsStates operator&(const EDefaultsStates& L, const uint8& R){ return static_cast<EDefaultsStates>(static_cast<uint8>(L) & static_cast<uint8>(1 << R)); }
//template<typename EnumType = EDefaultsStates>
FORCEINLINE void operator&=(EDefaultsStates& L, const uint8& R) { L = L & R; }

//template<typename EnumType = EDefaultsStates>
FORCEINLINE EDefaultsStates operator~(const EDefaultsStates& stateBitFlag) { return static_cast<EDefaultsStates>(~static_cast<uint8>(stateBitFlag)); }

//template<typename EnumType = EDefaultsStates>
FORCEINLINE bool CheckBitFlagIsMasked(const EDefaultsStates& stateBitFlag, const EDefaultsStates& stateBitMask) { return (stateBitFlag & stateBitMask) > EDefaultsStates::Default; }
//template<typename EnumType = EDefaultsStates>
FORCEINLINE bool CheckBitFlagIsMasked(const EDefaultsStates& stateBitFlag, const uint8& stateBitMask)
{
	//LOG_TEXT(TEXT("%d & %d : %d"), stateBitFlag, stateBitMask, stateBitFlag & stateBitMask);
	return (stateBitFlag & stateBitMask) > EDefaultsStates::Default;
}



#pragma endregion State Enum

#pragma region Forward Declarations

DECLARE_DELEGATE(FOnStateStart)
DECLARE_DELEGATE(FOnStateExit)

typedef TFunction<void(float)> StateFunctor;

class ACharacter;

#pragma endregion Forward Declarations


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOXON_API UFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFSMComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	#pragma region Functors
	virtual void RegisterStateFunction(const EFSMStatesMap& state, TFunction<void(float)> function);
	virtual void UnregisterStateFunction(const EFSMStatesMap& state);
	#pragma endregion Functors

	#pragma region On/Off Delegates
	TSharedPtr<FOnStateStart> GetStartDelegates(const EFSMStatesMap& state) {return stateStarts[state];}
	TSharedPtr<FOnStateExit> GetExitDelegates(const EFSMStatesMap& state) {return stateExits[state];}
	virtual void ActivateState(const EFSMStatesMap& state);
	virtual void DeactivateState(const EFSMStatesMap& state, const float& cooldownTime = 0.f);
	#pragma endregion On/Off Delegates
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void ResetStates();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDefaultsStates currentStates = EDefaultsStates::Default;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDefaultsStates lockedStates = EDefaultsStates::Default;

	FTimerHandle lockTimer;
	
	#pragma region Functors
	//https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/uLangCore/uLang/Common/Containers/TFunction?application_version=5.6
	TArray<StateFunctor> stateFunctors;	  // Phase별로 나뉘게 되면 Map<pair<phase,state>, StateFunctor> 생각도...
	/*TFunction<void(float)> IdleState;
	TFunction<void(float)> PatrolState;	
	TFunction<void(float)> ChaseState;
	TFunction<void(float)> AttackState;
	TFunction<void(float)> DamagedState;	
	TFunction<void(float)> InteractState;
	TFunction<void(float)> DeadState;*/
	#pragma endregion Functors
public:
	#pragma region On/Off Delegates
	TArray<TSharedPtr<FOnStateStart>> stateStarts;
	TArray<TSharedPtr<FOnStateExit>> stateExits;
	//TArray<FOnStateStart> stateStarts;
	//TArray<FOnStateExit> stateExits;
	#pragma endregion On/Off Delegates
private:
	UPROPERTY()
	ACharacter* owner;

	FORCEINLINE void DebugBitFlag(const FString& identifier, const uint8& state = 127) const
	{
		FString strByte = "";
		for (int i = 0; i < 8; ++i) (currentStates & i) > EDefaultsStates::Default ? strByte += "1" : strByte += "0";
		//PRINT_TEXT(1888, 2.f, TEXT("After Deactivate : %s"), *strByte);
		LOG_TEXT(TEXT("BitFlag_%s :  %s"), *identifier, *strByte);
		if (state != 127)
		{
			strByte = "";
			const uint8 stateByte = 1 << state;
			for (int i = 0; i < 8; ++i) (stateByte & (1 << i)) > 0 ? strByte += "1" : strByte += "0";
			LOG_TEXT(TEXT("%s : %d->%d %s"), *identifier, state, stateByte, *strByte);
		}
	}
};
