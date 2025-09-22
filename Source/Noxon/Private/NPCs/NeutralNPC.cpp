// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/NeutralNPC.h"

#include "GameFlow/GameMode/IngameGameMode.h"
#include "NPCs/Components/FSMComponent.h"
#include "Utility/FindHelper.h"


ANeutralNPC::ANeutralNPC()
{
	PrimaryActorTick.bCanEverTick = false;

	USkeletalMesh* mesh = BASE_SKM;
	if (IsValid(mesh))
	{
		GetMesh()->SetSkeletalMeshAsset(mesh);
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	}
	
	fsmComponent = CreateDefaultSubobject<UFSMComponent>(TEXT("FSM"));
}

void ANeutralNPC::BeginPlay()
{
	Super::BeginPlay();

	AIngameGameMode* gm = GetWorld()->GetAuthGameMode<AIngameGameMode>();
	if (gm != nullptr)
	{
		gm->RegisterNpc(this);
	}
}
