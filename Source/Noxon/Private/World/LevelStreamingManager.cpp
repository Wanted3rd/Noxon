#include "World/LevelStreamingManager.h"
#include "LevelInstance/LevelInstanceActor.h"
#include "LevelInstance/LevelInstanceSubsystem.h"
#include "Engine/World.h"
#include "World/RustLevelInstance.h"
#include "World/LevelGraph.h"
#include "GameFlow/GameMode/IngameGameState.h"
#include "Kismet/GameplayStatics.h"

ALevelStreamingManager::ALevelStreamingManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALevelStreamingManager::BeginPlay()
{
	Super::BeginPlay();
}

bool ALevelStreamingManager::LoadLevel(const FName& levelName)
{
	FManagedLevel managedLevel;
	if (!FindManagedLevel(levelName, managedLevel))
	{
		UE_LOG(LogTemp, Warning, TEXT("Level %s not found in managed levels"), *levelName.ToString());
		return false;
	}

	if (managedLevel.bIsLoaded)
	{
		UE_LOG(LogTemp, Warning, TEXT("Level %s is already loaded"), *levelName.ToString());
		return true;
	}

	if (!managedLevel.levelAsset.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Level asset for %s is not valid"), *levelName.ToString());
		return false;
	}

	ULevelInstanceSubsystem* levelInstanceSubsystem = GetWorld()->GetSubsystem<ULevelInstanceSubsystem>();
	if (!levelInstanceSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("Level Instance Subsystem not available"));
		return false;
	}

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ALevelInstance* newLevelInstance = GetWorld()->SpawnActor<ALevelInstance>(
		ALevelInstance::StaticClass(),
		managedLevel.levelTransform,
		spawnParams
	);

	if (newLevelInstance)
	{
		newLevelInstance->SetWorldAsset(managedLevel.levelAsset);

		// Update the managed level in the array
		FManagedLevel* actualManagedLevel = managedLevels.FindByPredicate([levelName](const FManagedLevel& level)
		{
			return level.levelName == levelName;
		});

		if (actualManagedLevel)
		{
			actualManagedLevel->levelInstance = newLevelInstance;
			actualManagedLevel->bIsLoaded = true;
			actualManagedLevel->bIsVisible = true;
		}

		UE_LOG(LogTemp, Log, TEXT("Level %s loaded successfully"), *levelName.ToString());
		return true;
	}

	UE_LOG(LogTemp, Error, TEXT("Failed to create level instance for %s"), *levelName.ToString());
	return false;
}

bool ALevelStreamingManager::UnloadLevel(const FName& levelName)
{
	FManagedLevel managedLevel;
	if (!FindManagedLevel(levelName, managedLevel))
	{
		UE_LOG(LogTemp, Warning, TEXT("Level %s not found in managed levels"), *levelName.ToString());
		return false;
	}

	if (!managedLevel.bIsLoaded)
	{
		UE_LOG(LogTemp, Warning, TEXT("Level %s is not loaded"), *levelName.ToString());
		return true;
	}

	// Find and update the actual managed level in the array
	FManagedLevel* actualManagedLevel = managedLevels.FindByPredicate([levelName](const FManagedLevel& level)
	{
		return level.levelName == levelName;
	});

	if (actualManagedLevel && actualManagedLevel->levelInstance)
	{
		actualManagedLevel->levelInstance->Destroy();
		actualManagedLevel->levelInstance = nullptr;
		actualManagedLevel->bIsLoaded = false;
		actualManagedLevel->bIsVisible = false;

		levelInstances.Remove(levelName);

		UE_LOG(LogTemp, Log, TEXT("Level %s unloaded successfully"), *levelName.ToString());
		return true;
	}

	return false;
}

bool ALevelStreamingManager::SetLevelVisibility(const FName& levelName, bool bVisible)
{
	FManagedLevel* actualManagedLevel = managedLevels.FindByPredicate([levelName](const FManagedLevel& level)
	{
		return level.levelName == levelName;
	});

	if (!actualManagedLevel || !actualManagedLevel->levelInstance)
	{
		return false;
	}

	actualManagedLevel->levelInstance->SetActorHiddenInGame(!bVisible);
	actualManagedLevel->bIsVisible = bVisible;

	UE_LOG(LogTemp, Log, TEXT("Level %s visibility set to %s"), *levelName.ToString(), bVisible ? TEXT("true") : TEXT("false"));
	return true;
}

bool ALevelStreamingManager::IsLevelLoaded(const FName& levelName) const
{
	FManagedLevel managedLevel;
	if (FindManagedLevel(levelName, managedLevel))
	{
		return managedLevel.bIsLoaded;
	}
	return false;
}

bool ALevelStreamingManager::IsLevelVisible(const FName& levelName) const
{
	FManagedLevel managedLevel;
	if (FindManagedLevel(levelName, managedLevel))
	{
		return managedLevel.bIsVisible;
	}
	return false;
}

TArray<FName> ALevelStreamingManager::GetLoadedLevels() const
{
	TArray<FName> loadedLevels;
	for (const FManagedLevel& level : managedLevels)
	{
		if (level.bIsLoaded)
		{
			loadedLevels.Add(level.levelName);
		}
	}
	return loadedLevels;
}

void ALevelStreamingManager::RegisterLevelInstance(ARustLevelInstance* levelInstance)
{
	if (!levelInstance)
	{
		return;
	}

	FName levelName = FName(levelInstance->GetActorNameOrLabel());
	levelInstances.Add(levelName, levelInstance);

	UE_LOG(LogTemp, Log, TEXT("Registered level instance: %s"), *levelName.ToString());
}

ARustLevelInstance* ALevelStreamingManager::GetLevelInstance(const FName& levelName) const
{
	if (const TObjectPtr<ARustLevelInstance>* foundInstance = levelInstances.Find(levelName))
	{
		return *foundInstance;
	}
	return nullptr;
}

bool ALevelStreamingManager::FindManagedLevel(const FName& levelName, FManagedLevel& outLevel)
{
	FManagedLevel* foundLevel = managedLevels.FindByPredicate([levelName](const FManagedLevel& level)
	{
		return level.levelName == levelName;
	});

	if (foundLevel)
	{
		outLevel = *foundLevel;
		return true;
	}
	return false;
}

bool ALevelStreamingManager::FindManagedLevel(const FName& levelName, FManagedLevel& outLevel) const
{
	const FManagedLevel* foundLevel = managedLevels.FindByPredicate([levelName](const FManagedLevel& level)
	{
		return level.levelName == levelName;
	});

	if (foundLevel)
	{
		outLevel = *foundLevel;
		return true;
	}
	return false;
}

/*void ALevelStreamingManager::OnLevelLoaded()
{
	UE_LOG(LogTemp, Log, TEXT("Level loaded successfully"));
}*/

void ALevelStreamingManager::UpdatePlayerLocation(const FVector& playerLocation)
{
	DetermineCurrentLevel(playerLocation);

	if (!currentLevelName.IsNone())
	{
		LoadAdjacentLevels(currentLevelName);
	}
}

void ALevelStreamingManager::LoadAdjacentLevels(const FName& centerLevelName)
{
	if (!levelGraph)
	{
		UE_LOG(LogTemp, Warning, TEXT("Level graph not set"));
		return;
	}

	FLevelNode centerNode;
	if (!levelGraph->FindNodeByName(centerLevelName, centerNode))
	{
		UE_LOG(LogTemp, Warning, TEXT("Center level node not found: %s"), *centerLevelName.ToString());
		return;
	}

	TArray<int32> adjacentNodes = levelGraph->GetAccessibleNodes(centerNode.nodeID);

	for (int32 nodeID : adjacentNodes)
	{
		FLevelNode adjacentNode;
		if (levelGraph->FindNodeByID(nodeID, adjacentNode) && !adjacentNode.bIsCurrentlyLoaded)
		{
			LoadLevelFromGraph(nodeID);
		}
	}

	UnloadNonAdjacentLevels(centerLevelName);
}

void ALevelStreamingManager::UpdateLevelClearStatus(const FName& levelName, bool bCleared)
{
	if (levelGraph)
	{
		// We need to directly modify the node in the array, so we'll find it directly
		FLevelNode* node = levelGraph->levelNodes.FindByPredicate([levelName](const FLevelNode& node)
		{
			return node.levelName == levelName;
		});

		if (node)
		{
			node->bIsCleared = bCleared;
			UE_LOG(LogTemp, Log, TEXT("Level %s clear status updated to: %s"),
				*levelName.ToString(), bCleared ? TEXT("true") : TEXT("false"));
		}
	}
}

void ALevelStreamingManager::DetermineCurrentLevel(const FVector& playerLocation)
{
	if (!levelGraph)
	{
		return;
	}

	FName newCurrentLevel;
	float closestDistance = FLT_MAX;

	for (const FLevelNode& node : levelGraph->levelNodes)
	{
		if (node.bIsCurrentlyLoaded)
		{
			float distance = FVector::Dist(playerLocation, node.levelTransform.GetLocation());
			if (distance < closestDistance)
			{
				closestDistance = distance;
				newCurrentLevel = node.levelName;
			}
		}
	}

	if (newCurrentLevel != currentLevelName)
	{
		currentLevelName = newCurrentLevel;
		UE_LOG(LogTemp, Log, TEXT("Player entered level: %s"), *currentLevelName.ToString());

		if (AIngameGameState* gameState = GetWorld()->GetGameState<AIngameGameState>())
		{
			gameState->SetActiveLevelName(currentLevelName);
		}
	}
}

void ALevelStreamingManager::LoadLevelFromGraph(int32 nodeID)
{
	if (!levelGraph)
	{
		return;
	}

	FLevelNode node;
	if (!levelGraph->FindNodeByID(nodeID, node))
	{
		return;
	}

	if (LoadLevel(node.levelName))
	{
		// Update the actual node in the graph
		FLevelNode* actualNode = levelGraph->levelNodes.FindByPredicate([nodeID](const FLevelNode& n)
		{
			return n.nodeID == nodeID;
		});

		if (actualNode)
		{
			actualNode->bIsCurrentlyLoaded = true;

			if (actualNode->bRequiresActorPersistence)
			{
				persistentLevels.Add(actualNode->levelName);
				UE_LOG(LogTemp, Log, TEXT("Level %s marked as persistent"), *actualNode->levelName.ToString());
			}
		}
	}
}

void ALevelStreamingManager::UnloadNonAdjacentLevels(const FName& centerLevelName)
{
	if (!levelGraph)
	{
		return;
	}

	FLevelNode centerNode;
	if (!levelGraph->FindNodeByName(centerLevelName, centerNode))
	{
		return;
	}

	TArray<int32> adjacentNodes = levelGraph->GetConnectedNodes(centerNode.nodeID);
	adjacentNodes.Add(centerNode.nodeID);

	for (FLevelNode& node : levelGraph->levelNodes)
	{
		if (node.bIsCurrentlyLoaded && !adjacentNodes.Contains(node.nodeID))
		{
			if (!persistentLevels.Contains(node.levelName))
			{
				if (UnloadLevel(node.levelName))
				{
					node.bIsCurrentlyLoaded = false;
					UE_LOG(LogTemp, Log, TEXT("Unloaded non-adjacent level: %s"), *node.levelName.ToString());
				}
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Keeping persistent level loaded: %s"), *node.levelName.ToString());
			}
		}
	}
}

void ALevelStreamingManager::SetLevelGraph(ULevelGraph* newLevelGraph)
{
	levelGraph = newLevelGraph;

	if (levelGraph)
	{
		managedLevels.Empty();

		for (const FLevelNode& node : levelGraph->levelNodes)
		{
			FManagedLevel managedLevel;
			managedLevel.levelName = node.levelName;
			managedLevel.levelAsset = node.levelAsset;
			managedLevel.levelTransform = node.levelTransform;
			managedLevel.bIsLoaded = false;
			managedLevel.bIsVisible = false;
			managedLevel.levelInstance = nullptr;

			managedLevels.Add(managedLevel);
		}

		UE_LOG(LogTemp, Log, TEXT("LevelGraph assigned with %d levels"), managedLevels.Num());
	}
}


