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
	virtual void LeftAction() = 0;
	UFUNCTION()
	virtual void RightAction() = 0;
	UFUNCTION()
	virtual int RKeyAction(int inValue) = 0;
};


UCLASS()
class NOXON_API AHandItem : public ABaseItem, public IItemAction
{
	GENERATED_BODY()

public:
	AHandItem();

	virtual void LeftAction() override {}
	virtual void RightAction() override {}
	virtual int RKeyAction(int inValue) override { return 0; }

protected:
	virtual void BeginPlay() override;

protected:
	
};
