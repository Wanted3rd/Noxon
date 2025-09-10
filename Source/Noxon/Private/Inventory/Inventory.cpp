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

bool UInventory::GetSlot(int32 index, FItemSlot& outSlot) const
{
	if (Slots[index] == outSlot)
	{
		
	}
}

