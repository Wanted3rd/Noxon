// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Enemy.h"

#include "GameFlow/GameMode/IngameGameMode.h"
#include "Items/HandItems/RifleDemoGun.h"
#include "NPCs/Components/ActionComponent.h"
#include "NPCs/Components/FSMComponent.h"
#include "NPCs/Components/PerceptionComponent.h"
#include "NPCs/Datas/StateEnums.h"
#include "Player/MainPlayer.h"
#include "Utility/DebugHelper.h"
#include "Utility/FindHelper.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "AITypes.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = false;
	relationship = ERelationship::Hostile;
	
	if (USkeletalMesh* mesh = FinderHelper::FindAssetFromConstructor<USkeletalMesh>("/Game/Assets/RustAsset/Hazmet/source/Hazmet.Hazmet"))
	{
		GetMesh()->SetSkeletalMeshAsset(mesh);
		
		if (TSubclassOf<UAnimInstance> animInstanceClass = FinderHelper::GetClassFromConstructor<UAnimInstance>("/Game/Animation/AnimInstance"))
		{
			GetMesh()->SetAnimInstanceClass(animInstanceClass);
		}
	}
	
	fsmComponent = CreateDefaultSubobject<UFSMComponent>(TEXT("FSM"));
	perceptionComponent = CreateDefaultSubobject<UPerceptionComponent>(TEXT("Perception"));
	actionComponent = CreateDefaultSubobject<UActionComponent>(TEXT("Action"));

	aiPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetupAIPerception();
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	ARifleDemoGun* gun = GetWorld()->SpawnActor<ARifleDemoGun>();
	EquipHandItem(gun);

	if (aiPerceptionComponent)
	{
		aiPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemy::OnTargetPerceptionUpdated);
		aiPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemy::OnPerceptionUpdated);
	}

	if (AIngameGameMode* gm = GetWorld()->GetAuthGameMode<AIngameGameMode>())
	{
		gm->RegisterNpc(this);
	}

	fsmComponent->ActivatePhaseState(EPhase::Idle);
	fsmComponent->ActivateMoveState(EMoveState::Stop);
}

void AEnemy::SetupAIPerception()
{
	if (!aiPerceptionComponent)
		return;

	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if (sightConfig)
	{
		sightConfig->SightRadius = 1500.0f;
		sightConfig->LoseSightRadius = 2000.0f;
		sightConfig->PeripheralVisionAngleDegrees = 60.0f;
		sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		sightConfig->DetectionByAffiliation.bDetectEnemies = true;
		sightConfig->SetMaxAge(5.0f);

		aiPerceptionComponent->ConfigureSense(*sightConfig);
	}

	hearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	if (hearingConfig)
	{
		hearingConfig->HearingRange = 2000.0f;
		hearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
		hearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
		hearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		hearingConfig->SetMaxAge(3.0f);

		aiPerceptionComponent->ConfigureSense(*hearingConfig);
	}

	aiPerceptionComponent->SetDominantSense(sightConfig->GetSenseImplementation());
}

void AEnemy::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* actor : UpdatedActors)
	{
		if (AMainPlayer* player = Cast<AMainPlayer>(actor))
		{
			// Enemy는 기본적으로 Player를 적대시
			if (relationship == ERelationship::Hostile &&
				(fsmComponent->GetCurrentPhase() & EPhase::OrdinaryPhase) > EPhase::Default)
			{
				fsmComponent->ActivatePhaseState(EPhase::HipFire);
				fsmComponent->ActivateMoveState(EMoveState::Chase);
				UE_LOG(LogTemp, Warning, TEXT("Enemy detected player! Entering battle phase"));
			}
		}
	}
}

void AEnemy::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (AMainPlayer* player = Cast<AMainPlayer>(Actor))
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			// Enemy는 기본적으로 Player를 적대시
			if (relationship == ERelationship::Hostile &&
				(fsmComponent->GetCurrentPhase() & EPhase::OrdinaryPhase) > EPhase::Default)
			{
				fsmComponent->ActivatePhaseState(EPhase::HipFire);
				fsmComponent->ActivateMoveState(EMoveState::Chase);
				UE_LOG(LogTemp, Warning, TEXT("Enemy sensed player! Entering battle phase"));
			}
		}
	}
}

