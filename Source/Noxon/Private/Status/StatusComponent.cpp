// Fill out your copyright notice in the Description page of Project Settings.


#include "Status/StatusComponent.h"

// Sets default values for this component's properties
UStatusComponent::UStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}

bool UStatusComponent::RegStatus(FName Name, float Max, float Current, bool bActive)
{
	// Check has already exist
	// UE_LOG(LogTemp, Warning, TEXT("Already Exist Status"));
    if (!StatusMap.Contains(Name))
    {
        Current < 0 ?
        StatusMap.Add(Name, FStatus{Max, Max, bActive}) :
        StatusMap.Add(Name, FStatus{Max, Current, bActive});
        return true;
    }
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Registering Status Error"));
		return false;
	}
	
}

FStatus UStatusComponent::GetStatus(FName Name) const
{
	return *(StatusMap.Find(Name));
}

void UStatusComponent::SetStatus(FName Name, float _Max, float _Current, bool NewActive)
{
	if ( StatusMap.Find(Name) != nullptr)
	{
		// -1 : maintains
		if ( _Max >= 0 )
			StatusMap.Find(Name)->Max = _Max;
		
		
		if ( _Current >= 0 )
			StatusMap.Find(Name)->Current = _Current;
		
		StatusMap.Find(Name)->bActive = NewActive;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("That Status isn't Exists"));
	}
}

bool UStatusComponent::ApplyStatusDelta(FName Name, float Delta)
{
	StatusMap.Find(Name)->Current += Delta;
	if (StatusMap.Find(Name)->Current > StatusMap.Find(Name)->Max)
	{
		StatusMap.Find(Name)->Current = StatusMap.Find(Name)->Max;
		return true;
	}

	else if (StatusMap.Find(Name)->Current > 0)
	{	// stats are Not Empty
		return true;
	}
	
	else
	{	// stats are empty
		StatusMap.Find(Name)->Current = 0.f;
		return false;
	}
}
