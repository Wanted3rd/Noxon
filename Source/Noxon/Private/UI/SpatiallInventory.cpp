#include "UI/SpatiallInventory.h"

#include "Components/CanvasPanel.h"
#include "Inventory/Inventory.h"
#include "UI/InventoryGrid.h"

void USpatiallInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Grid_BackPack)
	{
		// Ensure slot widgets are hosted on the canvas before any inventory binding occurs.
		Grid_BackPack->SetOwningCanvas(CanvasPanel);
	}
}

void USpatiallInventory::OnInventoryAssigned(UInventory* InInventory)
{
	Super::OnInventoryAssigned(InInventory);

	// Propagate the bound inventory to the grid so it can create and update slot widgets.
	if (Grid_BackPack)
	{
		Grid_BackPack->InitializeInventory(InInventory);
	}
}

