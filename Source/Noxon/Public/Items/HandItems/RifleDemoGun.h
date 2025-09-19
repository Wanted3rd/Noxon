// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "RifleDemoGun.generated.h"

UCLASS()
class NOXON_API ARifleDemoGun : public AGun
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARifleDemoGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
};
