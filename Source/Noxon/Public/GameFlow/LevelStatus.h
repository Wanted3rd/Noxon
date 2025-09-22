#pragma once

#include "CoreMinimal.h"
#include "LevelStatus.generated.h"

UENUM()
enum class ELevelCompleteCondition
{
	Default = 0 UMETA(Hidden),
	ReachToGoal,
	FarmingMaterials,
	CraftWeapons,
	EliminateAllEnemy,
	End UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FLevelCompleteCondition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELevelCompleteCondition conditionType = ELevelCompleteCondition::Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString conditionDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 requiredCount = 1;

	UPROPERTY(BlueprintReadOnly)
	int32 currentCount = 0;

	bool IsCompleted() const { return currentCount >= requiredCount; }
};

USTRUCT(BlueprintType)
struct FLevelProgressData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLevelCompleteCondition> completeConditions;

	UPROPERTY(BlueprintReadOnly)
	FDateTime firstEnterTime = FDateTime::MinValue();

	UPROPERTY(BlueprintReadOnly)
	FDateTime lastTime =  FDateTime::MinValue();

	UPROPERTY(BlueprintReadOnly)
	FDateTime clearTime = FDateTime::MinValue();
	
	bool GetTimeDiff(FTimespan& outDiff);
};
