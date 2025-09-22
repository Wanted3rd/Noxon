// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ammo.h"
#include "RifleDefaultAmmo.generated.h"

UCLASS()
class NOXON_API ARifleDefaultAmmo : public AAmmo
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARifleDefaultAmmo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	// 활성화 세팅 함수
	virtual void SetActive(bool bActive) override;
};
