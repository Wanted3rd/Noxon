// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISocketProvider.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UISocketProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NOXON_API IISocketProvider
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:


	public:
		virtual FTransform GetSocketTransform(FName SocketName) const = 0;
	

};
