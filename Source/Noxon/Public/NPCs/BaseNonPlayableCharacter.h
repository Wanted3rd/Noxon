// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseNonPlayableCharacter.generated.h"

UCLASS()
class NOXON_API ABaseNonPlayableCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseNonPlayableCharacter();

	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

public:

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Anim", meta=(AllowPrivateAccess=true))
	TSubclassOf<UAnimInstance> animFactory;
	
	
};
