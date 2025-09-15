// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/StateAction.h"
#include "GameFramework/Character.h"
#include "BaseNonPlayableCharacter.generated.h"

class UStateAction;
class UFSMComponent;
class AHandItem;

UENUM(BlueprintType)
enum class ERelationship : uint8
{
	Default = 0 UMETA(Hidden),
	Neutral = 1,
	Hostile = 2,
	Friendly = 3,
	End = 4 UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FPerceptionProperties
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	float noticeRange = 1000.f;
	UPROPERTY()
	float meleeAttackRange = 200.f;
	UPROPERTY()
	float gunFireRange = 600.f;
};


UCLASS()
class NOXON_API ABaseNonPlayableCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseNonPlayableCharacter();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void DeadNPC();

	
#pragma region Target
	UFUNCTION(BlueprintCallable)
	FORCEINLINE APawn* GetTargetPawn() {return targetPawn;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetTargetPawn(APawn* inTarget) {targetPawn = inTarget;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector GetGoalLocation() {return goalLocation;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetGoalLocation(FVector inLocation) {goalLocation = inLocation;}
#pragma endregion Target
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UFSMComponent* GetFSMComponent() {return fsmComponent;}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FPerceptionProperties GetPerceptionProperties() {return perceptionProperties;}

	FORCEINLINE void SetPhaseAction(const TObjectPtr<UStateAction>& inAction) {currentPhaseAction = inAction;}
	FORCEINLINE void SetMoveAction(const TObjectPtr<UStateAction>& inAction) {currentMoveAction = inAction;}
	FORCEINLINE void SetDamagedAction(const TObjectPtr<UStateAction>& inAction) {currentDamagedAction = inAction;}
	
protected:
	virtual void BeginPlay() override;

	void PhaseTick(float deltaTime);
	void MoveTick(float deltaTime);
	void DamagedTick(float deltaTime);

public:
	ERelationship relationship = ERelationship::Default;

protected:
	FTimerHandle deadTimer;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Anim", meta=(AllowPrivateAccess=true))
	TSubclassOf<UAnimInstance> animFactory;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UFSMComponent> fsmComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Action|Current")
	TObjectPtr<UStateAction> currentPhaseAction = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Action|Current")
	TObjectPtr<UStateAction> currentMoveAction = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Action|Current")
	TObjectPtr<UStateAction> currentDamagedAction = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Action|Prev")
	TObjectPtr<UStateAction> prevPhaseAction = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Action|Prev")
	TObjectPtr<UStateAction> prevMoveAction = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Action|Prev")
	TObjectPtr<UStateAction> prevDamagedAction = nullptr;

	UPROPERTY()
	TObjectPtr<APawn> targetPawn = nullptr;

	UPROPERTY()
	FVector goalLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Perception", meta=(AllowPrivateAccess=true))
	FPerceptionProperties perceptionProperties = FPerceptionProperties();

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TObjectPtr<AHandItem> handItem = nullptr;
};
