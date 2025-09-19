#include "NPCs/BaseNonPlayableCharacter.h"

#include "Animations/NPCAnimInstance.h"
#include "GameFlow/GameMode/IngameGameMode.h"
#include "NPCs/Actions/StateAction.h"
#include "NPCs/Components/ActionComponent.h"
#include "NPCs/Components/FSMComponent.h"
#include "NPcs/Datas/StateEnums.h"
#include "NPCs/Components/PerceptionComponent.h"
#include "NPCs/Manager/NPCManager.h"
#include "Utility/FindHelper.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"


ABaseNonPlayableCharacter::ABaseNonPlayableCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	animFactory = FinderHelper::GetClassFromConstructor<UNPCAnimInstance>(TEXT("/Game/NPCs/ABP_NPC_C"));
	if (animFactory != nullptr)
	{
		GetMesh()->SetAnimInstanceClass(animFactory);
	}
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ABaseNonPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
	initTransform = GetActorTransform();
	aiController = Cast<AAIController>(GetController());
	curHp = maxHp;
}

void ABaseNonPlayableCharacter::DeadNPC()
{
	if (deadTimer.IsValid())
	{
		return;
	}
	GetActionComp()->ResetActions();
	GetPerceptionComp()->ResetPerception();
	
	// choice drop item here.
	if (GetWorld()->GetAuthGameMode<AIngameGameMode>())
	{
		GetWorld()->GetTimerManager().SetTimer(deadTimer, [&]()->void
	   {
		   // Spawn drop item here.
		   GetWorld()->GetAuthGameMode<AIngameGameMode>()->UnregisterNpc(this);
		   Destroy();
	   }, 2.f, false);
	}
}

AAIController* ABaseNonPlayableCharacter::GetAIController()
{
	return IsValid(aiController) ? aiController : nullptr;
}

void ABaseNonPlayableCharacter::EnableComponentTick(bool bActive)
{
	fsmComponent->SetComponentTickEnabled(bActive);
	perceptionComponent->SetComponentTickEnabled(bActive);
	actionComponent->SetComponentTickEnabled(bActive);
}

