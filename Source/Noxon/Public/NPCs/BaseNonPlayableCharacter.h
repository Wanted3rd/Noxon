// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseNonPlayableCharacter.generated.h"

class UFSMComponent;
class UPerceptionComponent;
class UActionComponent;
class AHandItem;
class AAIController;

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

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void DeadNPC();
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsDead() const {return deadTimer.IsValid();}

	UFUNCTION(BlueprintPure)
	AAIController* GetAIController();
	UFUNCTION()
	void EnableComponentTick(bool bActive);
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UFSMComponent* GetFSMComp() {return fsmComponent;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UPerceptionComponent* GetPerceptionComp() {return perceptionComponent;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UActionComponent* GetActionComp() {return actionComponent;}

	UFUNCTION(BlueprintPure)
	FORCEINLINE FTransform GetInitTransform() const {return initTransform;}

	UFUNCTION()
	AHandItem* GetHandItem() {return handItem;}
	UFUNCTION()
	void EquipHandItem(AHandItem* inHandItem) { handItem = inHandItem; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void Damage(float inDamage) {curHp -= inDamage;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetHp() {return curHp;}

public:
	ERelationship relationship = ERelationship::Default;

protected:
	FTimerHandle deadTimer;

	UPROPERTY()
	TObjectPtr<AAIController> aiController;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Anim", meta=(AllowPrivateAccess=true))
	TSubclassOf<UAnimInstance> animFactory;

	UPROPERTY(EditDefaultsOnly, Category = "Components", meta=(AllowPrivateAccess=true))
	TObjectPtr<UFSMComponent> fsmComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess=true))
	TObjectPtr<UPerceptionComponent> perceptionComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components", meta=(AllowPrivateAccess=true))
	TObjectPtr<UActionComponent> actionComponent;
	// convert to Status comp if we can
	UPROPERTY()
	float maxHp = 5.f;
	UPROPERTY()
	float curHp;

	UPROPERTY()
	FTransform initTransform = FTransform();
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TObjectPtr<AHandItem> handItem = nullptr;
};
