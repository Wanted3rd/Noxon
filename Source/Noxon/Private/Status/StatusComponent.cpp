// Fill out your copyright notice in the Description page of Project Settings.


#include "Status/StatusComponent.h"

// Sets default values for this component's properties
UStatusComponent::UStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UStatusComponent::RegStatus(FName Name, float Current, float Max, bool bActive)
{
	// Check has already exist
	// UE_LOG(LogTemp, Warning, TEXT("Already Exist Status"));
	
	StatusMap.Add(Name, FStatus(Max, Current, bActive));
	if (StatusMap.Contains(Name))
		return true;
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Status Error"));
		return false;
	}
	
}

FStatus UStatusComponent::GetStatus(FName Name) const
{
	return *(StatusMap.Find(Name));
}



