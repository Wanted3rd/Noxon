// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCs/Manager/EnemyManager.h"

void UEnemyManager::PullNPCsTransformsFromWorld()
{
}

void UEnemyManager::SaveNPCToArr(ACharacter* npc)
{
	npcArr.AddUnique(npc);
}
