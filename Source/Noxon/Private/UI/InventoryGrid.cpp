#include "UI/InventoryGrid.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/InventoryGridSlot.h"
#include "Utility/DebugHelper.h"

void UInventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GridConstruct();
}

void UInventoryGrid::GridConstruct()
{
	GridSlots.Reserve(Rows * Cols);
	for (int32 j = 0; j < Rows; ++j)
	{
		for (int32 i = 0; i < Cols; ++i)
		{
			UInventoryGridSlot* slot = CreateWidget<UInventoryGridSlot>(this, GridSlotClass);
			CanvasPanel->AddChild(slot);

			int32 index = j * Cols + i;
			slot->SetSlotIndex(index);

			UCanvasPanelSlot* GridCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(slot);
			GridCPS->SetSize(FVector2D(SlotSize));

			FIntPoint SlotPos(i, j);
			GridCPS->SetPosition(SlotPos * SlotSize);
			LOG_TEXT(TEXT("slot : %d, %d, %f"), i, j, SlotSize);
			GridSlots.Add(slot);
			LOG_TEXT(TEXT("%d"), GridSlots.Num());
		}
	}
}

void UInventoryGrid::SetOwningCanvas(class UCanvasPanel* OwningCanvas)
{
	OwningCanvasPanel = OwningCanvas;
}
