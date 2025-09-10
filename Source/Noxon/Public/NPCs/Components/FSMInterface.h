// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSMInterface.generated.h"


//https://dev.epicgames.com/documentation/ko-kr/unreal-engine/interfaces-in-unreal-engine
UINTERFACE(MinimalAPI, Blueprintable)
class UFSMInterface : public UInterface
{
	GENERATED_BODY()
};

class IFSMInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Actions for State Machine")
	void OnEnter();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Actions for State Machine")
	void OnGoing();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Actions for State Machine")
	void OnExit();

};
