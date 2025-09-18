#include "NPCs/BaseNonPlayableCharacter.h"

#include "Animations/NPCAnimInstance.h"
#include "GameFlow/GameMode/IngameGameMode.h"
#include "NPCs/Actions/StateAction.h"
#include "NPCs/Components/FSMComponent.h"
#include "NPCs/Manager/NPCManager.h"
#include "Utility/FindHelper.h"


ABaseNonPlayableCharacter::ABaseNonPlayableCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	animFactory = FinderHelper::GetClassFromConstructor<UNPCAnimInstance>(TEXT("/Game/NPCs/ABP_NPC_C"));
	if (animFactory != nullptr)
	{
		GetMesh()->SetAnimInstanceClass(animFactory);
	}
	
}

void ABaseNonPlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DamagedTick(DeltaTime);
	if (fsmComponent->GetCurrentDamagedState() == EDamageState::Death)
	{
		return;
	}
	MoveTick(DeltaTime);
	PhaseTick(DeltaTime);
}

void ABaseNonPlayableCharacter::DeadNPC()
{
	if (deadTimer.IsValid())
	{
		return;
	}
	currentPhaseAction = nullptr;
	currentMoveAction = nullptr;
	currentDamagedAction = nullptr;
	prevPhaseAction = nullptr;
	prevMoveAction = nullptr;
	prevDamagedAction = nullptr;
	// choice drop item here.
	if (!GetWorld()->GetAuthGameMode<AIngameGameMode>())
	{
		GetWorld()->GetTimerManager().SetTimer(deadTimer, [&]()->void
	   {
		   // Spawn drop item here.
		   GetWorld()->GetAuthGameMode<AIngameGameMode>()->UnregisterNpc(this);
		   Destroy();
	   }, 2.f, false);
	}
}

void ABaseNonPlayableCharacter::ResetPerception()
{
	goalLocation = FVector::ZeroVector;
	targetPawn = nullptr;
}

void ABaseNonPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseNonPlayableCharacter::PhaseTick(float deltaTime)
{
	if (currentPhaseAction != prevPhaseAction)
	{
		if (prevPhaseAction)
		{
			prevPhaseAction->OnEnd(this);
		}
		prevPhaseAction = currentPhaseAction;
		if (currentPhaseAction)
		{
			currentPhaseAction->OnBegin(this);
		}
		
		return;
	}

	if (currentPhaseAction)
	{
		currentPhaseAction->OnTick(this, deltaTime);
	}
}

void ABaseNonPlayableCharacter::MoveTick(float deltaTime)
{
	if (currentMoveAction != prevMoveAction)
	{
		if (prevMoveAction)
		{
			prevMoveAction->OnEnd(this);
		}
		prevMoveAction = currentMoveAction;
		if (currentMoveAction)
		{
			currentMoveAction->OnBegin(this);
		}
		
		return;
	}

	if (currentMoveAction)
	{
		currentMoveAction->OnTick(this, deltaTime);
	}
}

void ABaseNonPlayableCharacter::DamagedTick(float deltaTime)
{
	if (currentDamagedAction != prevDamagedAction)
	{
		if (prevDamagedAction)
		{
			prevDamagedAction->OnEnd(this);
		}
		prevDamagedAction = currentDamagedAction;
		if (currentDamagedAction)
		{
			currentDamagedAction->OnBegin(this);
		}
		return;
	}

	if (currentDamagedAction)
	{
		currentDamagedAction->OnTick(this, deltaTime);
	}
}
