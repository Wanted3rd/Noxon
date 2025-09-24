// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Actions/Damaged/SmallDamaged.h"
#include "NPCs/BaseNonPlayableCharacter.h"
#include "NPCs/Components/FSMComponent.h"
#include "NPCs/Datas/StateEnums.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

void USmallDamaged::OnBegin(ABaseNonPlayableCharacter* owner)
{
	if (!owner) return;

	if (owner->GetHp() <= 0)
	{
		owner->GetFSMComp()->ActivateDamagedState(EDamageState::Death);
		return;
	}

	FVector damageLocation = owner->GetActorLocation();

	if (bloodParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			owner->GetWorld(),
			bloodParticleSystem,
			damageLocation,
			FRotator::ZeroRotator
		);
	}
	if (hitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			owner->GetWorld(),
			hitSound,
			damageLocation,
			FRotator::ZeroRotator
			);
	}

	owner->GetFSMComp()->ActivateDamagedState(EDamageState::Default);
}

void USmallDamaged::OnTick(ABaseNonPlayableCharacter* owner, float deltaTime)
{
}

void USmallDamaged::OnEnd(ABaseNonPlayableCharacter* owner)
{
}
