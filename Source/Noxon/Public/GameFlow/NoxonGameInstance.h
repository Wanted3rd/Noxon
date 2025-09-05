// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NoxonGameInstance.generated.h"

#define GI GetGameInstance<UNoxonGameInstance>();

/**
 * 
 */
UCLASS()
class NOXON_API UNoxonGameInstance : public UGameInstance
{
	GENERATED_BODY()
};
