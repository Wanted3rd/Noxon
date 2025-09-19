// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PerceptionComponent.generated.h"


class ABaseNonPlayableCharacter;
struct FProximityCheckContext;

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
	UPROPERTY()
	float viewAngleLimit = 60.f;
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOXON_API UPerceptionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPerceptionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void PostApplyToComponent() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FPerceptionProperties GetPerceptionProperties() {return perceptionProperties;}
	UFUNCTION()
	void ReceiveContext(const FProximityCheckContext& context);
	UFUNCTION(BlueprintCallable)
	APawn* GetTarget() {return targetPawn;}
	UFUNCTION(BlueprintCallable)
	void ResetPerception();

protected:
	bool Look(const FProximityCheckContext& context);
	
protected:
	UPROPERTY()
	TObjectPtr<ABaseNonPlayableCharacter> owner;
	UPROPERTY()
	TObjectPtr<APawn> targetPawn = nullptr;

	UPROPERTY()
	FTransform view = FTransform();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Perception", meta=(AllowPrivateAccess=true))
	FPerceptionProperties perceptionProperties = FPerceptionProperties();
};
