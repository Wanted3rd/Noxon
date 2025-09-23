#include "UI/InventoryWidgetBase.h"

#include "Inventory/Inventory.h"

void UInventoryWidgetBase::InitializeInventory(UInventory* InInventory)
{
    // Early-out when we already point at the requested inventory to avoid redundant delegate binding work.
    if (BoundInventory.Get() == InInventory)
    {
        return;
    }

    BoundInventory = InInventory;
    OnInventoryAssigned(InInventory);
}

void UInventoryWidgetBase::OnInventoryAssigned(UInventory* InInventory)
{
    // Default implementation intentionally left blank; child widgets bind to the inventory in overrides.
}
