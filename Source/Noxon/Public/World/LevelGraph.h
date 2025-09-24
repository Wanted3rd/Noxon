#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelGraph.generated.h"

class ARustLevelInstance;

USTRUCT(BlueprintType)
struct FLevelNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName levelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> levelAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform levelTransform = FTransform::Identity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int32 nodeID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasQuestConditions = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRequiresActorPersistence = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsCurrentlyLoaded = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsCleared = false;
};

USTRUCT(BlueprintType)
struct FLevelEdge
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 fromNodeID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 toNodeID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float transitionDistance = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector connectionPoint = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRequiresClearance = false;
};

UCLASS(BlueprintType)
class NOXON_API ULevelGraph : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Graph")
	TArray<FLevelNode> levelNodes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Graph")
	TArray<FLevelEdge> levelEdges;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Graph")
	int32 startingNodeID = 0;

	UFUNCTION(BlueprintCallable)
	bool FindNodeByID(int32 nodeID, FLevelNode& outNode);

	UFUNCTION(BlueprintCallable)
	bool FindNodeByName(const FName& levelName, FLevelNode& outNode);

	bool FindNodeByID(int32 nodeID, FLevelNode& outNode) const;
	bool FindNodeByName(const FName& levelName, FLevelNode& outNode) const;

	UFUNCTION(BlueprintCallable)
	TArray<int32> GetConnectedNodes(int32 nodeID) const;

	UFUNCTION(BlueprintCallable)
	TArray<int32> GetAccessibleNodes(int32 fromNodeID) const;

	UFUNCTION(BlueprintCallable)
	bool AreNodesConnected(int32 fromNodeID, int32 toNodeID) const;

	UFUNCTION(BlueprintCallable)
	float GetConnectionDistance(int32 fromNodeID, int32 toNodeID) const;

	UFUNCTION(BlueprintCallable)
	bool IsAllLevelsCleared() const;

	UFUNCTION(BlueprintCallable)
	TArray<FName> GetLevelsWithQuests() const;
};