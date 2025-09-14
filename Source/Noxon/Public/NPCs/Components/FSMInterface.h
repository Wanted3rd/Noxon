// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSMInterface.generated.h"

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
#pragma endregion


//https://dev.epicgames.com/documentation/ko-kr/unreal-engine/interfaces-in-unreal-engine
UINTERFACE(MinimalAPI, Blueprintable)
class UFSMInterface : public UInterface
{
	GENERATED_BODY()
};

class IFSMInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Actions for State Machine")
	void OnEnter();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Actions for State Machine")
	void OnGoing();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Actions for State Machine")
	void OnExit();
/*#pragma region Functors
	typedef TFunction<void(float)> StateFunctor;
	//https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/uLangCore/uLang/Common/Containers/TFunction?application_version=5.6
	TMap<EPhase, TArray<StateFunctor>> stateFunctors;
	
#pragma endregion Functors

#pragma region Register Phase
	void RegisterStateFunction(const EPhase& phase, const uint8& state, const StateFunctor& function) { stateFunctors[phase][state] = function; }
	void BeginPlay()
	{
	
	//stateFunctors.Init(nullptr, EFSMStatesMap::End + 1);
		//stateStarts.AddZeroed(EFSMStatesMap::End + 1);
		//for (int i = 0; i < EFSMStatesMap::End + 1; ++i)
		//{
		//	stateStarts[i] = MakeShared<FOnStateStart>();
		//}
		//stateExits.AddZeroed(EFSMStatesMap::End + 1);
		//for (int i = 0; i < EFSMStatesMap::End + 1; ++i)
		//{
		//	stateExits[i] = MakeShared<FOnStateExit>();
		//}
		
		//stateStarts.Init(FOnStateStart(), EFSMStatesMap::End + 1);
		//stateExits.Init(FOnStateExit(), EFSMStatesMap::End + 1);
	}

#pragma endregion Register Phase
*/
};
