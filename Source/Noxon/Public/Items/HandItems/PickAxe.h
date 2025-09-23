// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HandItem.h"
#include "PickAxe.generated.h"

UCLASS()
class NOXON_API APickAxe : public AHandItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickAxe();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	UPROPERTY(EditAnywhere)
	class AGatherableRock* gatherableRock;

	
	// 레벨에 존재하는 모든 돌 찾기
	TArray<AActor*> FoundRocks;


public:
	UFUNCTION()
	virtual void DamageRock(AGatherableRock* hitRock, AActor* pickAxe);
};
