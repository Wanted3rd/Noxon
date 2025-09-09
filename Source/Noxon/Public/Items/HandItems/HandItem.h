// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "HandItem.generated.h"

UINTERFACE()
class UItemAction : public UInterface
{
	GENERATED_BODY()
};

class IItemAction
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void LeftAction();
	UFUNCTION()
	virtual void RightAction();
};


UCLASS()
class NOXON_API AHandItem : public ABaseItem, public IItemAction
{
	GENERATED_BODY()

public:
	AHandItem();

	virtual void LeftAction() override {}
	virtual void RightAction() override {}

protected:
	virtual void BeginPlay() override;

protected:
	
};
