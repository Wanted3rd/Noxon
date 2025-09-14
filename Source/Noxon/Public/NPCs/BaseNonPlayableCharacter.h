// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseNonPlayableCharacter.generated.h"

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

UCLASS()
class NOXON_API ABaseNonPlayableCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseNonPlayableCharacter();

#pragma region Target
	UFUNCTION(BlueprintCallable)
	FORCEINLINE AActor* GetTargetActor() {return targetActor;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetTargetActor(AActor* inTarget) {targetActor = inTarget;}
#pragma endregion Target

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UFSMComponent* GetFSMComponent() {return fsmComponent;}

	void Activate(bool bActive);
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void RegisterFSMActions() PURE_VIRTUAL(ABaseNonPlayableCharacter::RegisterFSMActions, );

public:
	ERelationship relationship = ERelationship::Default;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Anim", meta=(AllowPrivateAccess=true))
	TSubclassOf<UAnimInstance> animFactory;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UFSMComponent> fsmComponent;
	
	UPROPERTY()
	TObjectPtr<AActor> targetActor = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TObjectPtr<AHandItem> handItem = nullptr;
};
