#include "Inventory/Inventory.h"

UInventory::UInventory()
{
	PrimaryComponentTick.bCanEverTick = true;

	
}

void UInventory::BeginPlay()
{
	Super::BeginPlay();

	
}

void UInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}