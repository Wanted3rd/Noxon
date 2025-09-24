// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionComponent.generated.h"

class ABaseNonPlayableCharacter;
class UStateAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOXON_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UActionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void PostApplyToComponent() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	

	FORCEINLINE void SetPhaseAction(const TObjectPtr<UStateAction>& inAction) {currentPhaseAction = inAction;}
	FORCEINLINE void SetMoveAction(const TObjectPtr<UStateAction>& inAction) {currentMoveAction = inAction;}
	FORCEINLINE void SetDamagedAction(const TObjectPtr<UStateAction>& inAction) {currentDamagedAction = inAction;}
	void ResetActions();

	UFUNCTION(BlueprintPure)
	bool GetGoalLocation(FVector& outLocation);

	void RandPos();
	void TogglePatrolFlag() {patrolFlag = !patrolFlag;}

	UFUNCTION(BlueprintCallable)
	float GetLastShootTime() const { return lastShootTime; }
	UFUNCTION(BlueprintCallable)
	void SetLastShootTime(float time) { lastShootTime = time; }
	UFUNCTION(BlueprintCallable)
	void ResetLastShootTime() { lastShootTime = 0.0f; }

	UFUNCTION(BlueprintCallable)
	float GetWeaveTime() const { return weaveTime; }
	UFUNCTION(BlueprintCallable)
	void SetWeaveTime(float time) { weaveTime = time; }
	UFUNCTION(BlueprintCallable)
	float GetWeaveDuration() const { return weaveDuration; }
	UFUNCTION(BlueprintCallable)
	void SetWeaveDuration(float duration) { weaveDuration = duration; }
	UFUNCTION(BlueprintCallable)
	FVector GetWeaveDirection() const { return weaveDirection; }
	UFUNCTION(BlueprintCallable)
	void SetWeaveDirection(const FVector& direction) { weaveDirection = direction; }

	UFUNCTION(BlueprintCallable)
	float GetReloadTime() const { return reloadTime; }
	UFUNCTION(BlueprintCallable)
	void SetReloadTime(float time) { reloadTime = time; }
	UFUNCTION(BlueprintCallable)
	void ResetReloadTime() { reloadTime = 0.0f; }

protected:
	UFUNCTION(BlueprintCallable)
	void PhaseTick(float deltaTime);
	UFUNCTION(BlueprintCallable)
	void MoveTick(float deltaTime);
	UFUNCTION(BlueprintCallable)
	void DamagedTick(float deltaTime);

protected:
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

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ABaseNonPlayableCharacter> owner;
	UPROPERTY()
	FVector goalLocation = FVector::ZeroVector;
	bool patrolFlag = false;

	UPROPERTY()
	float lastShootTime = 0.0f;

	UPROPERTY()
	float weaveTime = 0.0f;
	UPROPERTY()
	float weaveDuration = 0.0f;
	UPROPERTY()
	FVector weaveDirection = FVector::ZeroVector;

	UPROPERTY()
	float reloadTime = 0.0f;
	
};
