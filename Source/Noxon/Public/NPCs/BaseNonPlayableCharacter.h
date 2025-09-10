// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseNonPlayableCharacter.generated.h"

class UFSMComponent;

UENUM(BlueprintType, meta=(Bitmask))
enum class ERelationship : uint8
{
	Default = 0 << 0 UMETA(Hidden),
	Neutral = 1 << 0,
	Hostile = 1 << 1,
	Friendly = 1 << 2,
	End = 1 << 3 UMETA(Hidden)
};

UCLASS()
class NOXON_API ABaseNonPlayableCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseNonPlayableCharacter();

	virtual void Tick(float DeltaTime) override;

#pragma region Target
	UFUNCTION(BlueprintCallable)
	FORCEINLINE AActor* GetTargetActor() {return targetActor;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetTargetActor(AActor* inTarget) {targetActor = inTarget;}
#pragma endregion Target

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UFSMComponent* GetFSMComponent() {return fsmComponent;}
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void RegisterFSMActions();

public:
	ERelationship relationship = ERelationship::Default;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Anim", meta=(AllowPrivateAccess=true))
	TSubclassOf<UAnimInstance> animFactory;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UFSMComponent> fsmComponent;
	
	UPROPERTY()
	TObjectPtr<AActor> targetActor = nullptr;
};
