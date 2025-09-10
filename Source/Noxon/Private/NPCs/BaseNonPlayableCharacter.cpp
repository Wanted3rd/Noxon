#include "NPCs/BaseNonPlayableCharacter.h"

#include <NPCs/Components/FSMComponent.h>

#include "Utility/DebugHelper.h"
#include "Utility/FindHelper.h"


ABaseNonPlayableCharacter::ABaseNonPlayableCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	animFactory = FinderHelper::GetClassFromConstructor<UAnimInstance>(TEXT("/Game/Assets/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed_C"));
	if (animFactory != nullptr)
	{
		GetMesh()->SetAnimInstanceClass(animFactory);
	}
}

void ABaseNonPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseNonPlayableCharacter::RegisterFSMActions()
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


// Called every frame
void ABaseNonPlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

