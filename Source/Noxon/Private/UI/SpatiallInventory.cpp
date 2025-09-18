#include "UI/SpatiallInventory.h"

#include "Components/CanvasPanel.h"
#include "UI/InventoryGrid.h"

void USpatiallInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Grid_BackPack->SetOwningCanvas(CanvasPanel);
}
