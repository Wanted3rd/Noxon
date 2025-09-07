// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Enemy.h"

#include "NPCs/Components/FSMInterface.h"
#include "NPCs/Components/FSMComponent.h"
#include "Utility/FindHelper.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	relationship = ERelationship::Hostile;

	USkeletalMesh* mesh = BASE_SKM;
	if (IsValid(mesh))
	{
		GetMesh()->SetSkeletalMesh(mesh);
	}
	
	fsmComponent = CreateDefaultSubobject<UFSMComponent>(TEXT("FSM"));
	
}

void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	RegisterFSMFunctions();
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::RegisterFSMFunctions()
{
}
