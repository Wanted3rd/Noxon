// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DoveTestCtrl.generated.h"

/**
 * 
 */
UCLASS()
class NOXON_API ADoveTestCtrl : public APlayerController
{
    GENERATED_BODY()
protected:
    virtual void BeginPlay() override;
};
