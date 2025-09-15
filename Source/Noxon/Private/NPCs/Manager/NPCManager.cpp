// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Manager/NPCManager.h"

#include "Serialization/JsonSerializer.h"

#include "Kismet/GameplayStatics.h"
#include "GameFlow/GameMode/IngameGameMode.h"

#include "NPCs/Enemy.h"
#include "NPCs/NeutralNPC.h"
#include "NPCs/Actions/ActionsPaths.h"
#include "NPCs/Components/FSMComponent.h"


TStatId UNPCManager::GetStatId() const
{
	return TStatId();
}

bool UNPCManager::ShouldCreateSubsystem(UObject* Outer) const
{
	UWorld* world = Cast<UWorld>(Outer);
	//AIngameGameMode* ingameGM = Cast<AIngameGameMode>(UGameplayStatics::GetGameMode(world));
	//if (!IsValid(world) || !IsValid(ingameGM))
	//{
	//	return false;
	//}
	
	return Super::ShouldCreateSubsystem(Outer);
}

void UNPCManager::OnWorldBeginPlay(UWorld& InWorld)
{
	CreateActions();
	ownerWorld = InWorld.GetWorld();
	lodProperties.tickableDist *= lodProperties.tickableDist;
	lodProperties.visibleDist *= lodProperties.visibleDist;
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
	APawn* tempPlayer = playerContainer[0]->GetPawn();
	for (ABaseNonPlayableCharacter* npc : activatedNpcContainer)
	{
		npc->SetPhaseAction(phaseActions[npc->GetFSMComponent()->GetCurrentPhase()]);
		if (FVector::Dist(npc->GetActorLocation(), tempPlayer->GetActorLocation()) < 500.f)
		{
			if (npc->GetActorForwardVector().Dot((
				tempPlayer->GetActorLocation() - npc->GetActorLocation()).GetSafeNormal()) < 0.3f)
				npc->GetFSMComponent()->ActivateMoveState(EMoveState::Chase);
		}
	}
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

	float visibleDistSquared = lodProperties.visibleDist;
	float tickableDistSquared = lodProperties.tickableDist;
	TArray<FProximityCheckContext> TaskContexts;
	ParallelForWithTaskContext(
		TaskContexts,
		npcPos.Num(),
	    [](int32 contextIndex, int32 NumContexts)
	    {
	    	FProximityCheckContext context = FProximityCheckContext();
	    	context.index = contextIndex;
		    return context;
	    },
		[visibleDistSquared, tickableDistSquared, npcPos, playerPos](FProximityCheckContext& Context, int32 NPCIndex)
		{
			const FVector npcLocation = npcPos[Context.index];
			ENpcActivateType activeType = ENpcActivateType::Default;
			for (const FVector& PlayerLocation : playerPos)
			{
				const float DistanceSquared = FVector::DistSquared(npcLocation, PlayerLocation);
				if (DistanceSquared < visibleDistSquared)
				{
					activeType = ENpcActivateType::Visible;
					break;
				}
				if (DistanceSquared < tickableDistSquared)
				{
					activeType = ENpcActivateType::Tickable;
					break;
				}
			}

			if (activeType > ENpcActivateType::Deactivated)
			{
				Context.activeType = activeType;
			}
			else
			{
				Context.activeType = ENpcActivateType::Deactivated;
			}
		}		
	);
	
	ownerWorld->GetTimerManager().SetTimerForNextTick(
		[&, TaskContexts, npcPos]() -> void
		{
			if (npcContainer.Num() != TaskContexts.Num())
			{
				return;
			}
			for (int32 i = 0; i < TaskContexts.Num(); ++i)
			{
				if (!npcContainer.IsValidIndex(i))
				{
					return;
				}
				ABaseNonPlayableCharacter* npc = npcContainer[i];
				switch (TaskContexts[i].activeType)
				{
				case ENpcActivateType::Deactivated:
					{
						npc->SetActorTickEnabled(false);
						npc->SetActorHiddenInGame(true);
						if (activatedNpcContainer.Contains(npc))
						{
							npc->SetTargetPawn(nullptr);
							activatedNpcContainer.Remove(npc);
						}
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
						// have to Modify. Hard Coding. What Keyword to find this line.
						npc->SetTargetPawn(playerContainer[0]->GetPawn());
						activatedNpcContainer.Add(npc);
					}
					break;
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
	if (activatedNpcContainer.Contains(npc))
	{
		activatedNpcContainer.Remove(npc);
	}
	npcContainer.RemoveSingleSwap(npc, EAllowShrinking::Yes);
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
	phaseActions.Add(EPhase::Idle, NewObject<UIdleAction>());
	phaseActions.Add(EPhase::HipFire, NewObject<UHipFireAction>());
	damagedActions.Add(EDamageState::SmallDamaged, NewObject<USmallDamaged>());
	damagedActions.Add(EDamageState::Death, NewObject<UDeadlyDamaged>());
	moveActions.Add(EMoveState::Stop, NewObject<UStopMove>());
	moveActions.Add(EMoveState::Patrol, NewObject<UPatrolMove>());
	moveActions.Add(EMoveState::Chase, NewObject<UChaseMove>());
	
}


