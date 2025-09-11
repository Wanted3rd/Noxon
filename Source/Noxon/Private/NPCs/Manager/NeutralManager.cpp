// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Manager/NeutralManager.h"

void UNeutralManager::PullNPCsTransformsFromWorld()
{
}

void UNeutralManager::SaveNPCToArr(ACharacter* npc)
{
	npcArr.AddUnique(npc);
}
