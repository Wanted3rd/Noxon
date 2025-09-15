// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "NPCManager.generated.h"


enum class EDamageState : uint8;
enum class EMoveState : uint8;
class UStateAction;
enum class EPhase : uint8;
class AMainPlayer;
class ABaseNonPlayableCharacter;

// using with modified dist as squared dist
USTRUCT(BlueprintType)
struct FLODPropertiesForActivateNPC : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FLODPropertiesForActivateNPC()
		: visibleDist(500.f)
		, tickableDist(1000.f)
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

/**
 * 
 */
UCLASS()
class NOXON_API UNPCManager : public UTickableWorldSubsystem
{
	GENERATED_BODY()

	struct FProximityCheckContext
	{
		int32 index = -1;
		ENpcActivateType activeType;
	};
	
public:
	virtual TStatId GetStatId() const override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Deinitialize() override;

	UFUNCTION()
	FORCEINLINE void RegisterPlayers(APlayerController* playerController)
	{playerContainer.AddUnique(playerController);}
	UFUNCTION()
	FORCEINLINE void UnregisterPlayers(APlayerController* playerController)
	{playerContainer.RemoveSingleSwap(playerController, EAllowShrinking::Yes);}

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

private:
	void SaveNPCsTransformToJson(const FNPCsTransform& NPCData);
	bool LoadNPCsTransformFromJson(FNPCsTransform& output);
	void CreateActions();

	
protected:
	UPROPERTY()
	TArray<APlayerController*> playerContainer;
	
	UPROPERTY()
	TArray<ABaseNonPlayableCharacter*> npcContainer;

	UPROPERTY()
	TSet<ABaseNonPlayableCharacter*> activatedNpcContainer;

	UPROPERTY()
	FLODPropertiesForActivateNPC lodProperties = FLODPropertiesForActivateNPC();

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
