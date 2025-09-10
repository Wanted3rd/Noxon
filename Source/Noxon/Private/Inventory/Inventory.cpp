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

bool UInventory::GetSlot(int32 index, FItemSlot& outslot) const
{
	if (Slots.IsValidIndex(index))
	{
		outslot = Slots[index];
		return true;
	}
	else
	{
		outslot = FItemSlot();
		return false;
	}
	
}

bool UInventory::AddItemInven(FName key, int32 quantity)
{
	// if key != table key has no exist
	
	if (Slots.Num() <= HotbarSize)	// if slots not full
	{
		Slots.Add(FItemSlot(key, quantity));
		return true;
	}
	else
		return false;
	
}

bool UInventory::RemoveItem(int32 index, int32 quantity)
{
	return false;
}

bool UInventory::GetDefinition(FName key) const
{
	return false;
}

