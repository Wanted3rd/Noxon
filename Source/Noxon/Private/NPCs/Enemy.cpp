// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Enemy.h"

#include "GameFlow/GameMode/IngameGameMode.h"
#include "NPCs/Components/FSMComponent.h"
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
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	AIngameGameMode* gm = GetWorld()->GetAuthGameMode<AIngameGameMode>();
	if (gm != nullptr)
	{
		gm->RegisterNpc(this);
	}
}

