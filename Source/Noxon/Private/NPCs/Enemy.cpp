// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Enemy.h"

#include "GameFlow/GameMode/IngameGameMode.h"
#include "Items/HandItems/RifleDemoGun.h"
#include "NPCs/Components/ActionComponent.h"
#include "NPCs/Components/FSMComponent.h"
#include "NPCs/Components/PerceptionComponent.h"
#include "Utility/DebugHelper.h"
#include "Utility/FindHelper.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = false;
	relationship = ERelationship::Hostile;
	
	USkeletalMesh* mesh = BASE_SKM;
	if (IsValid(mesh))
	{
		GetMesh()->SetSkeletalMeshAsset(mesh);
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	}
	
	fsmComponent = CreateDefaultSubobject<UFSMComponent>(TEXT("FSM"));
	perceptionComponent = CreateDefaultSubobject<UPerceptionComponent>(TEXT("Perception"));
	actionComponent = CreateDefaultSubobject<UActionComponent>(TEXT("Action"));
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	ARifleDemoGun* gun = GetWorld()->SpawnActor<ARifleDemoGun>();
	EquipHandItem(gun);
	if (AIngameGameMode* gm = GetWorld()->GetAuthGameMode<AIngameGameMode>())
	{
		gm->RegisterNpc(this);
	}
}

