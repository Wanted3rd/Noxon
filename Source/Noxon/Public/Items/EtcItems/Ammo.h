// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "Ammo.generated.h"

UENUM()
enum class ammoSize
{
	Pistol UMETA(DisplayName="Pistol"),
	Rifle UMETA(DisplayName="Rifle"),
	Shotgun UMETA(DisplayName="Shotgun"),
	RocketLauncher UMETA(DisplayName="RocketLauncher")
};

UENUM()
enum class ammoType
{
	Default UMETA(DisplayName="Default"),
	Explosive UMETA(DisplayName="Explosive"),
	HighVelocity UMETA(DisplayName="High Velocity"),
	Incendiary UMETA(DisplayName="Incendiary")
};


//인벤토리 상의 총알 정보만 작성
//실제 총알 효과는 총에서 결정

UCLASS()
class NOXON_API AAmmo : public ABaseItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAmmo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
