// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Manager/NPCManager.h"

#include "Serialization/JsonSerializer.h"

#include "Kismet/GameplayStatics.h"
#include "GameFlow/GameMode/IngameGameMode.h"

#include "NPCs/Enemy.h"
#include "NPCs/NeutralNPC.h"


bool UNPCManager::ShouldCreateSubsystem(UObject* Outer) const
{
	UWorld* world = Cast<UWorld>(Outer);
	AIngameGameMode* ingameGM = Cast<AIngameGameMode>(UGameplayStatics::GetGameMode(world));
	if (!IsValid(world) || !IsValid(ingameGM))
	{
		return false;
	}
	
	return Super::ShouldCreateSubsystem(Outer);
}

void UNPCManager::OnWorldBeginPlay(UWorld& InWorld)
{
	ownerWorld = InWorld.GetWorld();
	Super::OnWorldBeginPlay(InWorld);
	PushNPCsTransformsForWorld();
	
	// this work is needed in main player class's begin play. for joining to started session.
	for (FConstPlayerControllerIterator iter = ownerWorld->GetPlayerControllerIterator(); iter; ++iter)
	{
		playerContainer.Add(iter->Get());
	}
}

void UNPCManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (batchDeltaTime > lodProperties.updateTime)
	{
		ProcessNPCsBatch();
		batchDeltaTime = 0.f;
		return;
	}
	batchDeltaTime += DeltaTime;
}

void UNPCManager::Deinitialize()
{
	ownerWorld = nullptr;
	playerContainer.Empty(0);
	npcContainer.Empty(0);
	activatedNpcContainer.Empty(0);
	Super::Deinitialize();
}

void UNPCManager::ProcessNPCsBatch()
{
	if (!IsValid(ownerWorld) || npcContainer.Num() == 0 || playerContainer.Num() == 0)
	{
		return;
	}

	//https://dev.epicgames.com/community/learning/tutorials/BdmJ/unreal-engine-multithreading-techniques
	int32 range = npcContainer.Num() * playerContainer.Num();
	TArray<FVector> playerPos;
	playerPos.Reserve(playerContainer.Num());
	TArray<FVector> npcPos;
	npcPos.Reserve(npcContainer.Num());
	for (const APlayerController* playerController : playerContainer)
	{
		if (IsValid(playerController))
		{
			playerPos.Emplace(playerController->GetFocalLocation());
			continue;
		}
		return;
	}
	for (const ABaseNonPlayableCharacter* npc : npcContainer)
	{
		if (IsValid(npc))
		{
			npcPos.Emplace(npc->GetActorLocation());
			continue;
		}
		return;
	}
	
	TArray<FProximityCheckContext> TaskContexts;
	ParallelForWithTaskContext(
		/*"NPCsDistToPlayer",*/
		TaskContexts,
		npcPos.Num(),
	    FProximityCheckContext(),
		[&](FProximityCheckContext& Context, int32 NPCIndex)
		{
			const FVector NPCLocation = npcPos[NPCIndex];

			ENpcActivateType activeType = ENpcActivateType::Default;
			for (const FVector& PlayerLocation : playerPos)
			{
				const float DistanceSquared = FVector::DistSquared(NPCLocation, PlayerLocation);
				if (DistanceSquared < lodProperties.visibleDist)
				{
					activeType = ENpcActivateType::Visible;
					break;
				}
				if (DistanceSquared < lodProperties.tickableDist)
				{
					activeType = ENpcActivateType::Tickable;
					break;
				}
			}

			if (activeType > ENpcActivateType::Default)
			{
				Context.NPCs.Add(NPCIndex, activeType);
			}
			else
			{
				Context.NPCs.Add(NPCIndex, ENpcActivateType::Default);
			}
		}
	);

	if (npcContainer.Num() != npcPos.Num())
	{
		return;
	}
	
	ownerWorld->GetTimerManager().SetTimerForNextTick(
		[&]() -> void
		{
			for (const auto& Context : TaskContexts)
			{
				for (auto pair: Context.NPCs)
				{
					if (npcContainer.IsValidIndex(pair.Key))
					{
						ABaseNonPlayableCharacter* npc = npcContainer[pair.Key];
						switch (pair.Value)
						{
						case ENpcActivateType::Default:
							{
								npc->SetActorTickEnabled(false);
								npc->SetActorHiddenInGame(true);
							}
							break;
						case ENpcActivateType::Tickable:
							{
								npc->SetActorTickEnabled(true);
								npc->SetActorHiddenInGame(true);
							}
							break;
						case ENpcActivateType::Visible:
							{
								npc->SetActorTickEnabled(true);
								npc->SetActorHiddenInGame(false);
							}
							break;
						}
					}
				}
			}
		}
	);	
}

void UNPCManager::PullNPCsTransformsFromWorld()
{
	FNPCsTransform npcsTransform;
	npcsTransform.levelName = ownerWorld->GetCurrentLevel()->GetPackage()->GetName();
	
	TArray<AActor*> enemies;
	UGameplayStatics::GetAllActorsOfClass(ownerWorld, AEnemy::StaticClass(), enemies);
	for (AActor* enemy : enemies)
	{
		npcsTransform.enemiesTransform.Emplace(enemy->GetTransform());
	}
	TArray<AActor*> neutrals;
	UGameplayStatics::GetAllActorsOfClass(ownerWorld, ANeutralNPC::StaticClass(), neutrals);
	for (AActor* neutral : neutrals)
	{
		npcsTransform.neutralsTransform.Emplace(neutral->GetTransform());
	}
	
	SaveNPCsTransformToJson(npcsTransform);
	
}

// Before Begin play... how..?
void UNPCManager::PushNPCsTransformsForWorld()
{
	FNPCsTransform npcsTransform;
	
	
}

void UNPCManager::DestroyNPC(ABaseNonPlayableCharacter* npc)
{
	if (activatedNpcContainer.RemoveSingleSwap(npc, EAllowShrinking::Yes) == 0)
	{
		npcContainer.RemoveSingleSwap(npc, EAllowShrinking::Yes);
	}
	npc->Destroy();
}

void UNPCManager::SaveNPCsTransformToJson(const FNPCsTransform& NPCData)
{
	TSharedPtr<FJsonObject> rowKeyJsonObject;

	FString ExistingJson;
	if (FFileHelper::LoadFileToString(ExistingJson, *npcsTransformsFilePath))
	{
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ExistingJson);
		FJsonSerializer::Deserialize(Reader, rowKeyJsonObject);
	}

	if (!rowKeyJsonObject.IsValid())
	{
		rowKeyJsonObject = MakeShared<FJsonObject>();
	}

	TSharedPtr<FJsonObject> levelData = MakeShared<FJsonObject>();
    
	TArray<TSharedPtr<FJsonValue>> EnemiesArray;
	for (const FTransform& Transform : NPCData.enemiesTransform)
	{
		TSharedPtr<FJsonObject> TransformObject = MakeShared<FJsonObject>();
		TSharedPtr<FJsonObject> LocationObject = MakeShared<FJsonObject>();
		LocationObject->SetNumberField(TEXT("X"), Transform.GetLocation().X);
		LocationObject->SetNumberField(TEXT("Y"), Transform.GetLocation().Y);
		LocationObject->SetNumberField(TEXT("Z"), Transform.GetLocation().Z);
		TransformObject->SetObjectField(TEXT("Location"), LocationObject);
		
		TSharedPtr<FJsonObject> RotationObject = MakeShared<FJsonObject>();
		FQuat Rotation = Transform.GetRotation();
		RotationObject->SetNumberField(TEXT("X"), Rotation.X);
		RotationObject->SetNumberField(TEXT("Y"), Rotation.Y);
		RotationObject->SetNumberField(TEXT("Z"), Rotation.Z);
		RotationObject->SetNumberField(TEXT("W"), Rotation.W);
		TransformObject->SetObjectField(TEXT("Rotation"), RotationObject);
		
		TSharedPtr<FJsonObject> ScaleObject = MakeShared<FJsonObject>();
		ScaleObject->SetNumberField(TEXT("X"), Transform.GetScale3D().X);
		ScaleObject->SetNumberField(TEXT("Y"), Transform.GetScale3D().Y);
		ScaleObject->SetNumberField(TEXT("Z"), Transform.GetScale3D().Z);
		TransformObject->SetObjectField(TEXT("Scale"), ScaleObject);
    
		EnemiesArray.Add(MakeShared<FJsonValueObject>(TransformObject));
	}
    
	TArray<TSharedPtr<FJsonValue>> NeutralsArray;
	for (const FTransform& Transform : NPCData.neutralsTransform)
	{
		TSharedPtr<FJsonObject> TransformObject = MakeShared<FJsonObject>();
    
		TSharedPtr<FJsonObject> LocationObject = MakeShared<FJsonObject>();
		LocationObject->SetNumberField(TEXT("X"), Transform.GetLocation().X);
		LocationObject->SetNumberField(TEXT("Y"), Transform.GetLocation().Y);
		LocationObject->SetNumberField(TEXT("Z"), Transform.GetLocation().Z);
		TransformObject->SetObjectField(TEXT("Location"), LocationObject);
    
		TSharedPtr<FJsonObject> RotationObject = MakeShared<FJsonObject>();
		FQuat Rotation = Transform.GetRotation();
		RotationObject->SetNumberField(TEXT("X"), Rotation.X);
		RotationObject->SetNumberField(TEXT("Y"), Rotation.Y);
		RotationObject->SetNumberField(TEXT("Z"), Rotation.Z);
		RotationObject->SetNumberField(TEXT("W"), Rotation.W);
		TransformObject->SetObjectField(TEXT("Rotation"), RotationObject);
    
		TSharedPtr<FJsonObject> ScaleObject = MakeShared<FJsonObject>();
		ScaleObject->SetNumberField(TEXT("X"), Transform.GetScale3D().X);
		ScaleObject->SetNumberField(TEXT("Y"), Transform.GetScale3D().Y);
		ScaleObject->SetNumberField(TEXT("Z"), Transform.GetScale3D().Z);
		TransformObject->SetObjectField(TEXT("Scale"), ScaleObject);
    
		NeutralsArray.Add(MakeShareable(new FJsonValueObject(TransformObject)));
	}
    
	levelData->SetArrayField(TEXT("enemiesTransform"), EnemiesArray);
	levelData->SetArrayField(TEXT("neutralsTransform"), NeutralsArray);
	levelData->SetStringField(TEXT("timestamp"), FDateTime::Now().ToString());

	rowKeyJsonObject->SetObjectField(ownerWorld->GetCurrentLevel()->GetPackage()->GetName(), levelData);
    
	FString outputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&outputString);
	FJsonSerializer::Serialize(rowKeyJsonObject.ToSharedRef(), Writer);
    
	FFileHelper::SaveStringToFile(outputString, *npcsTransformsFilePath);
}


bool UNPCManager::LoadNPCsTransformFromJson(FNPCsTransform& output)
{
    FString fromJson;
    if (!FFileHelper::LoadFileToString(fromJson, *npcsTransformsFilePath))
    {
        return false;
    }

    TSharedPtr<FJsonObject> jsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(fromJson);

    if (!FJsonSerializer::Deserialize(Reader, jsonObject))
    {
        return false;
    }
	
	const TSharedPtr<FJsonObject> levelData = jsonObject->GetObjectField(ownerWorld->GetCurrentLevel()->GetPackage()->GetName());
	if (!levelData.IsValid())
	{
		return false;
	}


    const TArray<TSharedPtr<FJsonValue>>* EnemiesArray;
    if (levelData->TryGetArrayField(TEXT("enemiesTransform"), EnemiesArray))
    {
        for (const auto& JsonValue : *EnemiesArray)
        {
            TSharedPtr<FJsonObject> TransformObject = JsonValue->AsObject();
            if (TransformObject.IsValid())
            {
                FTransform Transform;

                if (const auto& LocationObject = TransformObject->GetObjectField(TEXT("Location")))
                {
                    FVector Location;
                    Location.X = LocationObject->GetNumberField(TEXT("X"));
                    Location.Y = LocationObject->GetNumberField(TEXT("Y"));
                    Location.Z = LocationObject->GetNumberField(TEXT("Z"));
                    Transform.SetLocation(Location);
                }

                if (const auto& RotationObject = TransformObject->GetObjectField(TEXT("Rotation")))
                {
                    FQuat Rotation;
                    Rotation.X = RotationObject->GetNumberField(TEXT("X"));
                    Rotation.Y = RotationObject->GetNumberField(TEXT("Y"));
                    Rotation.Z = RotationObject->GetNumberField(TEXT("Z"));
                    Rotation.W = RotationObject->GetNumberField(TEXT("W"));
                    Transform.SetRotation(Rotation);
                }

                if (const auto& ScaleObject = TransformObject->GetObjectField(TEXT("Scale")))
                {
                    FVector Scale;
                    Scale.X = ScaleObject->GetNumberField(TEXT("X"));
                    Scale.Y = ScaleObject->GetNumberField(TEXT("Y"));
                    Scale.Z = ScaleObject->GetNumberField(TEXT("Z"));
                    Transform.SetScale3D(Scale);
                }
            	
            	output.enemiesTransform.Emplace(Transform);
            }
        }
    }

    const TArray<TSharedPtr<FJsonValue>>* neutralsArray;
    if (levelData->TryGetArrayField(TEXT("neutralsTransform"), neutralsArray))
    {
        for (auto& JsonValue : *neutralsArray)
        {
            TSharedPtr<FJsonObject> TransformObject = JsonValue->AsObject();
            if (TransformObject.IsValid())
            {
                FTransform Transform;

                if (const auto& LocationObject = TransformObject->GetObjectField(TEXT("Location")))
                {
                    FVector Location;
                    Location.X = LocationObject->GetNumberField(TEXT("X"));
                    Location.Y = LocationObject->GetNumberField(TEXT("Y"));
                    Location.Z = LocationObject->GetNumberField(TEXT("Z"));
                    Transform.SetLocation(Location);
                }

                if (const auto& RotationObject = TransformObject->GetObjectField(TEXT("Rotation")))
                {
                    FQuat Rotation;
                    Rotation.X = RotationObject->GetNumberField(TEXT("X"));
                    Rotation.Y = RotationObject->GetNumberField(TEXT("Y"));
                    Rotation.Z = RotationObject->GetNumberField(TEXT("Z"));
                    Rotation.W = RotationObject->GetNumberField(TEXT("W"));
                    Transform.SetRotation(Rotation);
                }

                if (const auto& ScaleObject = TransformObject->GetObjectField(TEXT("Scale")))
                {
                    FVector Scale;
                    Scale.X = ScaleObject->GetNumberField(TEXT("X"));
                    Scale.Y = ScaleObject->GetNumberField(TEXT("Y"));
                    Scale.Z = ScaleObject->GetNumberField(TEXT("Z"));
                    Transform.SetScale3D(Scale);
                }
            	
            	output.neutralsTransform.Emplace(Transform);
            }
        }
    }
	return true;
}

void UNPCManager::CreateActions()
{
	
}


