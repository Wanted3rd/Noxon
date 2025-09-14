#include "NPCs/BaseNonPlayableCharacter.h"

#include "Animations/NPCAnimInstance.h"
#include "Utility/FindHelper.h"


ABaseNonPlayableCharacter::ABaseNonPlayableCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	animFactory = FinderHelper::GetClassFromConstructor<UNPCAnimInstance>(TEXT("/Game/NPCs/ABP_NPC_C"));
	if (animFactory != nullptr)
	{
		GetMesh()->SetAnimInstanceClass(animFactory);
	}
	
}

void ABaseNonPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
