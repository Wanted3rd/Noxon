// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NPCManager.generated.h"


class UStateAction;

enum class EPhase : uint8;
enum class EDamageState : uint8;
enum class EMoveState : uint8;
class ABaseNonPlayableCharacter;

// using with modified dist as squared dist
USTRUCT(BlueprintType)
struct FLODPropertiesForActivateNPC : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FLODPropertiesForActivateNPC()
		: visibleDist(1000.f)
		, tickableDist(2000.f)
		, updateTime(0.5f)
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=LOD)
	float visibleDist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=LOD)
	float tickableDist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Timer)
	float updateTime;
};

USTRUCT(BlueprintType)
struct FNPCsTransform
{
	GENERATED_USTRUCT_BODY()
public:
	FNPCsTransform()
		:levelName("")
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Enemy)
	FString levelName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Enemy)
	TArray<FTransform> enemiesTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Neutral)
	TArray<FTransform> neutralsTransform;
};

UENUM()
enum class ENpcActivateType : uint8
{
	Default = 0 UMETA(Hidden),
	Deactivated,
	Tickable,
	Visible,
	End UMETA(Hidden)
};


// convert to Array or set in multiplay game 
USTRUCT(BlueprintType)
struct FProximityCheckContext
{
	GENERATED_USTRUCT_BODY()
	
	float dist = 0.f;
	FVector direction = FVector::ZeroVector;
	UPROPERTY()
	APawn* target = nullptr;
};

/**
 * 
 */
UCLASS()
class NOXON_API UNPCManager : public UActorComponent
{
	GENERATED_BODY()

	
public:
	UNPCManager();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	FORCEINLINE void RegisterPlayers(APlayerController* playerController)
	{playerContainer.Add(playerController);}
	UFUNCTION()
	FORCEINLINE void UnregisterPlayers(const APlayerController* playerController)
	{playerContainer.Remove(playerController);}

	UFUNCTION()
	void RegisterNPC(ABaseNonPlayableCharacter* npc)
	{npcContainer.Add(npc);}
	UFUNCTION()
	void DestroyNPC(ABaseNonPlayableCharacter* npc);
	
	UFUNCTION(CallInEditor)
	void PullNPCsTransformsFromWorld();
	UFUNCTION(CallInEditor)
	void PushNPCsTransformsForWorld();

protected:
	UFUNCTION()
	void ProcessNPCsBatch();
	void ForEachNPCsBatch(ABaseNonPlayableCharacter* npc);
	void ParallelForNPCsBatch();
	

private:
	void SaveNPCsTransformToJson(const FNPCsTransform& NPCData);
	bool LoadNPCsTransformFromJson(FNPCsTransform& output);
	void CreateActions();

	
protected:
	UPROPERTY()
	TSet<APlayerController*> playerContainer;
	
	UPROPERTY()
	TSet<ABaseNonPlayableCharacter*> npcContainer;

	TMap<ABaseNonPlayableCharacter*, FProximityCheckContext> activatedNpcContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=LOD)
	FLODPropertiesForActivateNPC lodProperties;

	float batchDeltaTime = 0.f;

private:
	UPROPERTY()
	TObjectPtr<UWorld> ownerWorld = nullptr;
	UPROPERTY()
	TMap<EPhase, TObjectPtr<UStateAction>> phaseActions;
	UPROPERTY()
	TMap<EMoveState, TObjectPtr<UStateAction>> moveActions;
	UPROPERTY()
	TMap<EDamageState, TObjectPtr<UStateAction>> damagedActions;

	const FString npcsTransformsFilePath = FPaths::ProjectDir() + "Data/NPCsTransforms.json";
};
