// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Enemy.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "NPCs/Components/FSMInterface.h"
#include "NPCs/Components/FSMComponent.h"
#include "Utility/DebugHelper.h"
#include "Utility/FindHelper.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
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
	if (count > EFSMStatesMap::End)
	{
		return;		
	}
	
	if (time < 3.f)
	{
		time += DeltaTime;
		return;
	}
	//PRINT_TEXT(10, 1.f, TEXT("counting is %s"), *GetStateName(count).ToString());
	time = 0.f;
	fsmComponent->ActivateState(static_cast<EFSMStatesMap>(count));
	if (count > 0)
	{
		fsmComponent->DeactivateState(static_cast<EFSMStatesMap>(count - 1), 2);
	}
	count++;
}

void AEnemy::RegisterFSMFunctions()
{
	fsmComponent->RegisterStateFunction(EFSMStatesMap::Idle, [&](float DeltaTime) -> void
	{
		PRINT_TEXT(11, 2.f, TEXT("On Going Idle"));
	});
	fsmComponent->GetStartDelegates(EFSMStatesMap::Idle)->BindLambda([&]()->void
	{
		PRINT_TEXT(12, 2.f, TEXT("Start Idle"));
	});
	fsmComponent->GetExitDelegates(EFSMStatesMap::Idle)->BindLambda([&]()->void
	{
		PRINT_TEXT(13, 2.f, TEXT("Exit Idle"));
	});
	fsmComponent->RegisterStateFunction(EFSMStatesMap::Moving, [&](float DeltaTime) -> void
	{
		PRINT_TEXT(11, 2.f, TEXT("On Going Move"));
	});
	fsmComponent->GetStartDelegates(EFSMStatesMap::Moving)->BindLambda([&]()->void
	{
		PRINT_TEXT(12, 2.f, TEXT("Start Moving"));
	});
	fsmComponent->GetExitDelegates(EFSMStatesMap::Moving)->BindLambda([&]()->void
	{
		PRINT_TEXT(13, 2.f, TEXT("Exit Move"));
	});
	fsmComponent->RegisterStateFunction(EFSMStatesMap::Weaving, [&](float DeltaTime) -> void
	{
		PRINT_TEXT(11, 2.f, TEXT("On Going Weaving"));
	});
	fsmComponent->GetStartDelegates(EFSMStatesMap::Weaving)->BindLambda([&]()->void
	{
		PRINT_TEXT(12, 2.f, TEXT("Start Weaving"));
	});
	fsmComponent->GetExitDelegates(EFSMStatesMap::Weaving)->BindLambda([&]()->void
	{
		PRINT_TEXT(13, 2.f, TEXT("Exit Weaving"));
	});
	fsmComponent->RegisterStateFunction(EFSMStatesMap::Attack, [&](float DeltaTime) -> void
	{
		PRINT_TEXT(11, 2.f, TEXT("On Going Attack"));
	});
	fsmComponent->GetStartDelegates(EFSMStatesMap::Attack)->BindLambda([&]()->void
	{
		PRINT_TEXT(12, 2.f, TEXT("Start Attack"));
	});
	fsmComponent->GetExitDelegates(EFSMStatesMap::Attack)->BindLambda([&]()->void
	{
		PRINT_TEXT(13, 2.f, TEXT("Exit Attack"));
	});
	fsmComponent->RegisterStateFunction(EFSMStatesMap::Damaged, [&](float DeltaTime) -> void
	{
		PRINT_TEXT(11, 2.f, TEXT("On Going Damaged"));
	});
	fsmComponent->GetStartDelegates(EFSMStatesMap::Damaged)->BindLambda([&]()->void
	{
		PRINT_TEXT(12, 2.f, TEXT("Start Damaged"));
	});
	fsmComponent->GetExitDelegates(EFSMStatesMap::Damaged)->BindLambda([&]()->void
	{
		PRINT_TEXT(13, 2.f, TEXT("Exit Damaged"));
	});
	fsmComponent->RegisterStateFunction(EFSMStatesMap::Interact, [&](float DeltaTime) -> void
	{
		PRINT_TEXT(11, 2.f, TEXT("On Going Interact"));
	});
	fsmComponent->GetStartDelegates(EFSMStatesMap::Interact)->BindLambda([&]()->void
	{
		PRINT_TEXT(12, 2.f, TEXT("Start Interact"));
	});
	fsmComponent->GetExitDelegates(EFSMStatesMap::Interact)->BindLambda([&]()->void
	{
		PRINT_TEXT(13, 2.f, TEXT("Exit Interact"));
	});
	fsmComponent->GetStartDelegates(EFSMStatesMap::Idle)->ExecuteIfBound();
	fsmComponent->RegisterStateFunction(EFSMStatesMap::Dead, [&](float DeltaTime) -> void
	{
		PRINT_TEXT(11, 2.f, TEXT("On Going Dead"));
	});
	fsmComponent->GetStartDelegates(EFSMStatesMap::Dead)->BindLambda([&]()->void
	{
		PRINT_TEXT(12, 2.f, TEXT("Start Dead"));
	});
	fsmComponent->GetExitDelegates(EFSMStatesMap::Dead)->BindLambda([&]()->void
	{
		PRINT_TEXT(13, 2.f, TEXT("Exit Dead"));
	});
}
