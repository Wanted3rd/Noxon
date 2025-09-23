// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GatherableTree.generated.h"

UCLASS()
class NOXON_API AGatherableTree : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGatherableTree();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
